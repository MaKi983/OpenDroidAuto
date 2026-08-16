#pragma once

#include <unordered_map>
#include <memory>
#include <boost/asio.hpp>
#include <usb/IUSBEndpoint.hpp>
#include "LibUsbDevice.h"


namespace aasdk
{
namespace usb
{

class LibUsbEndpoint: public IUSBEndpoint, boost::noncopyable
{
public:
    LibUsbEndpoint(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService, const libusb_endpoint_descriptor* endpoint);

    void controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    uint8_t getAddress() override;
    void cancelTransfers() override;

private:
    static constexpr uint32_t cMaxTransferRetries = 2;

    static constexpr uint32_t cMaxErrorRetries = 1;

    struct PendingTransfer {
        Promise::Pointer promise;
        uint32_t retryCount = 0;
    };

    typedef std::unordered_map<libusb_transfer*, PendingTransfer> Transfers;

    void transfer(libusb_transfer *transfer, Promise::Pointer promise);
    static void transferHandler(libusb_transfer *transfer);

    LibUsbDevice::Pointer libUsbDevice_;
    aasdk::io::strand strand_;
    uint8_t endpointAddress_;
    Transfers transfers_;
};

}
}
