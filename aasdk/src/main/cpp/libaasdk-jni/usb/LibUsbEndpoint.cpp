#include <error/Error.hpp>
#include <Log.h>
#include "LibUsbEndpoint.h"


namespace aasdk
{
namespace usb
{

LibUsbEndpoint::LibUsbEndpoint(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService, const libusb_endpoint_descriptor* endpoint)
        : libUsbDevice_(libUsbDevice)
        , strand_(ioService)
        , endpointAddress_(endpoint->bEndpointAddress)
{
    if(Log::isVerbose()) Log_v("    Endpoint: \n");
    if(Log::isVerbose()) Log_v("        bEndpointAddress:    %02xh\n", endpoint->bEndpointAddress);
    if(Log::isVerbose()) Log_v("        bmAttributes:        %02xh\n", endpoint->bmAttributes);
    if(Log::isVerbose()) Log_v("        wMaxPacketSize:      %u\n", endpoint->wMaxPacketSize);
    if(Log::isVerbose()) Log_v("        bInterval:           %u\n", endpoint->bInterval);
    if(Log::isVerbose()) Log_v("        bRefresh:            %u\n", endpoint->bRefresh);
    if(Log::isVerbose()) Log_v("        bSynchAddress:       %u\n", endpoint->bSynchAddress);
}

void LibUsbEndpoint::controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) {
    if(endpointAddress_ != 0) {
        promise->reject(error::Error(error::ErrorCode::USB_INVALID_TRANSFER_METHOD));
    } else {
        auto* transfer = libusb_alloc_transfer(0);
        if(transfer == nullptr) {
            promise->reject(error::Error(error::ErrorCode::USB_TRANSFER_ALLOCATION));
        } else {
            libusb_fill_control_transfer(transfer, libUsbDevice_->handle(), buffer.data, reinterpret_cast<libusb_transfer_cb_fn>(&LibUsbEndpoint::transferHandler), this, timeout);
            this->transfer(transfer, std::move(promise));
        }
    }
}

void LibUsbEndpoint::interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) {
    if(endpointAddress_ == 0) {
        promise->reject(error::Error(error::ErrorCode::USB_INVALID_TRANSFER_METHOD));
    } else {
        auto* transfer = libusb_alloc_transfer(0);
        if(transfer == nullptr) {
            promise->reject(error::Error(error::ErrorCode::USB_TRANSFER_ALLOCATION));
        } else {
            libusb_fill_interrupt_transfer(transfer, libUsbDevice_->handle(), endpointAddress_, buffer.data, buffer.size, reinterpret_cast<libusb_transfer_cb_fn>(&LibUsbEndpoint::transferHandler), this, timeout);
            this->transfer(transfer, std::move(promise));
        }
    }
}

void LibUsbEndpoint::bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) {
    if(endpointAddress_ == 0) {
        promise->reject(error::Error(error::ErrorCode::USB_INVALID_TRANSFER_METHOD));
    } else {
        auto* transfer = libusb_alloc_transfer(0);
        if(transfer == nullptr) {
            Log_e("libusb_alloc_transfer error");
            promise->reject(error::Error(error::ErrorCode::USB_TRANSFER_ALLOCATION));
        } else {
            if (Log::isVerbose()) Log_v("libusb_fill_bulk_transfer %p", transfer);
            libusb_fill_bulk_transfer(transfer, libUsbDevice_->handle(), endpointAddress_, buffer.data, buffer.size, reinterpret_cast<libusb_transfer_cb_fn>(&LibUsbEndpoint::transferHandler), this, timeout);
            this->transfer(transfer, std::move(promise));
        }
    }
}

void LibUsbEndpoint::transfer(libusb_transfer *transfer, Promise::Pointer promise) {
    strand_->dispatch([this, self = shared_from_this(), transfer, promise = std::move(promise)]() mutable {
        // A completion callback can run on libusb's event thread as soon as the
        // transfer is submitted. Register it first: otherwise an immediately
        // completed transfer is discarded by transferHandler(), leaving the
        // protocol stream one frame behind (which later surfaces as SSL_READ).
        {
            std::lock_guard<std::mutex> lock(transfersMutex_);
            transfers_.insert(std::make_pair(transfer, PendingTransfer{std::move(promise), std::move(self)}));
        }

        if (Log::isVerbose()) Log_v("libusb_submit_transfer");
        auto submitResult = libusb_submit_transfer(transfer);
        if (Log::isVerbose()) Log_v("libusb_submit_transfer %d", submitResult);

        if(submitResult != LIBUSB_SUCCESS) {
            Promise::Pointer pendingPromise;
            {
                std::lock_guard<std::mutex> lock(transfersMutex_);
                auto pendingIt = transfers_.find(transfer);
                if (pendingIt != transfers_.end()) {
                    pendingPromise = std::move(pendingIt->second.promise);
                    transfers_.erase(pendingIt);
                }
            }

            libusb_free_transfer(transfer);
            if (pendingPromise) {
                pendingPromise->reject(error::Error(error::ErrorCode::USB_TRANSFER, submitResult));
            }
        }
    });
}

uint8_t LibUsbEndpoint::getAddress()
{
    return endpointAddress_;
}

void LibUsbEndpoint::cancelTransfers()
{
    if (Log::isDebug()) Log_d("cancel transfers");

    // Synchronous, and deliberately not routed through strand_.
    //
    // The caller is the session teardown, which releases the interface and closes
    // the device handle immediately afterwards, and which also stops the
    // io_service moments later. Posting the cancels onto the strand meant they
    // raced with that stop and were frequently never issued at all, leaving
    // transfers submitted against a handle that was then closed - a use-after-free
    // inside libusb's own event thread. Waiting for the strand instead would
    // deadlock, since teardown is itself reached from io_service threads.
    //
    // libusb_cancel_transfer only flags the transfer; the completion callback
    // still arrives later on libusb's thread, so holding the mutex here cannot
    // deadlock against transferHandler().
    std::lock_guard<std::mutex> lock(transfersMutex_);
    for(const auto& transfer : transfers_) {
        libusb_cancel_transfer(transfer.first);
    }
}

void LibUsbEndpoint::transferHandler(libusb_transfer *transfer) {
    if (Log::isVerbose()) Log_v("transferHandler %p", transfer);
    auto* endpoint = reinterpret_cast<LibUsbEndpoint*>(transfer->user_data);

    Promise::Pointer promise;
    // Holds the endpoint alive for the rest of this function even if the entry we
    // just removed was the last thing referencing it.
    std::shared_ptr<LibUsbEndpoint> self;

    {
        std::lock_guard<std::mutex> lock(endpoint->transfersMutex_);
        auto pendingIt = endpoint->transfers_.find(transfer);
        if(pendingIt == endpoint->transfers_.end()) {
            if (Log::isWarn()) Log_w("transfer not found in list");
            return;
        }

        promise = std::move(pendingIt->second.promise);
        self = std::move(pendingIt->second.self);
        endpoint->transfers_.erase(pendingIt);
    }

    const auto status = transfer->status;
    const auto actualLength = transfer->actual_length;
    libusb_free_transfer(transfer);

    // resolve()/reject() post onto the strand the promise was deferred on, so
    // this is safe to call straight from libusb's event thread. Nothing here
    // needs the io_service to still be running, which is what guarantees the
    // bookkeeping above always happens and the self-reference is always released.
    if(status == LIBUSB_TRANSFER_COMPLETED) {
        promise->resolve(actualLength);
    } else {
        // Deliberately no resubmit-on-error here. Re-submitting a bulk transfer
        // that already moved part of its buffer duplicates those bytes on the
        // wire, which desynchronises the framing the AA protocol (and its SSL
        // layer) depends on. Errors are propagated so the session can be torn
        // down and started cleanly instead.
        auto error = status == LIBUSB_TRANSFER_CANCELLED
                      ? error::Error(error::ErrorCode::OPERATION_ABORTED)
                      : error::Error(error::ErrorCode::USB_TRANSFER, status);
        promise->reject(error);
    }
}

}
}
