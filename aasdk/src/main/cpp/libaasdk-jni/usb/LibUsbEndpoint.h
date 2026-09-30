#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>
#include <usb/IUSBEndpoint.hpp>
#include "LibUsbDevice.h"


namespace aasdk
{
namespace usb
{

class LibUsbEndpoint: public IUSBEndpoint, public std::enable_shared_from_this<LibUsbEndpoint>, boost::noncopyable
{
public:
    LibUsbEndpoint(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService, const libusb_endpoint_descriptor* endpoint);

    void controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    uint8_t getAddress() override;
    void cancelTransfers() override;

private:
    struct PendingTransfer {
        Promise::Pointer promise;
        // Keeps this endpoint alive for as long as the transfer is in flight.
        // libusb invokes transferHandler() from its own event thread with a raw
        // pointer to us, so the object must outlive every submitted transfer even
        // if the owning AOAPDevice is destroyed first. The reference cycle this
        // creates is broken when the entry is erased, which transferHandler()
        // always does - it runs on libusb's thread and needs no io_service, so it
        // still happens after the session's io_service has been stopped.
        std::shared_ptr<LibUsbEndpoint> self;
    };

    typedef std::unordered_map<libusb_transfer*, PendingTransfer> Transfers;

    void transfer(libusb_transfer *transfer, Promise::Pointer promise);
    static void transferHandler(libusb_transfer *transfer);

    LibUsbDevice::Pointer libUsbDevice_;
    aasdk::io::strand strand_;
    uint8_t endpointAddress_;
    Transfers transfers_;
    // transfers_ is reached from three threads: the strand (submit), libusb's
    // event thread (completion) and whichever thread tears the session down
    // (cancel). A plain mutex rather than the strand, so cancelling cannot depend
    // on an io_service that is about to stop - see cancelTransfers().
    std::mutex transfersMutex_;
};

}
}
