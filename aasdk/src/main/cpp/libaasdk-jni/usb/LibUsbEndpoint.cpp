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
    strand_->dispatch([this, transfer, promise = std::move(promise)]() mutable {
        if (Log::isVerbose()) Log_v("libusb_submit_transfer");
        auto submitResult = libusb_submit_transfer(transfer);
        if (Log::isVerbose()) Log_v("libusb_submit_transfer %d", submitResult);

        if(submitResult == LIBUSB_SUCCESS) {
            transfers_.insert(std::make_pair(transfer, PendingTransfer{std::move(promise), 0}));
        } else {
            promise->reject(error::Error(error::ErrorCode::USB_TRANSFER, submitResult));
            libusb_free_transfer(transfer);
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
    for(const auto& transfer : transfers_) {
        libusb_cancel_transfer(transfer.first);
    }
}

void LibUsbEndpoint::transferHandler(libusb_transfer *transfer) {
    if (Log::isVerbose()) Log_v("transferHandler %p", transfer);
    auto self = reinterpret_cast<LibUsbEndpoint*>(transfer->user_data);

    self->strand_->dispatch([self, transfer]() mutable {
        auto pendingIt = self->transfers_.find(transfer);
        if(pendingIt == self->transfers_.end()) {
            if (Log::isWarn()) Log_w("transfer not found in list");
            return;
        }

        if(transfer->status == LIBUSB_TRANSFER_COMPLETED) {
            auto promise(std::move(pendingIt->second.promise));
            self->transfers_.erase(pendingIt);
            auto actualLength = transfer->actual_length;
            libusb_free_transfer(transfer);
            promise->resolve(actualLength);
            return;
        }

        const bool isTransientTimeout = transfer->status == LIBUSB_TRANSFER_TIMED_OUT;
        const bool isTransientError = transfer->status == LIBUSB_TRANSFER_ERROR;
        const uint32_t maxRetriesForStatus = isTransientTimeout ? cMaxTransferRetries
                                            : isTransientError   ? cMaxErrorRetries
                                                                  : 0;

        if((isTransientTimeout || isTransientError) &&
           pendingIt->second.retryCount < maxRetriesForStatus) {
            ++pendingIt->second.retryCount;
            if (Log::isWarn()) {
                Log_w("USB transfer %s, retry %u/%u",
                      isTransientTimeout ? "timeout" : "error",
                      pendingIt->second.retryCount, maxRetriesForStatus);
            }

            auto submitResult = libusb_submit_transfer(transfer);
            if(submitResult != LIBUSB_SUCCESS) {
                auto promise(std::move(pendingIt->second.promise));
                self->transfers_.erase(pendingIt);
                libusb_free_transfer(transfer);
                promise->reject(error::Error(error::ErrorCode::USB_TRANSFER, submitResult));
            }
            return;
        }

        auto promise(std::move(pendingIt->second.promise));
        self->transfers_.erase(pendingIt);
        auto error = transfer->status == LIBUSB_TRANSFER_CANCELLED
                      ? error::Error(error::ErrorCode::OPERATION_ABORTED)
                      : error::Error(error::ErrorCode::USB_TRANSFER, transfer->status);
        libusb_free_transfer(transfer);
        promise->reject(error);
    });
}

}
}
