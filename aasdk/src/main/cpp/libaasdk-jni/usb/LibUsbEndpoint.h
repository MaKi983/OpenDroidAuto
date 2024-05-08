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
    LibUsbEndpoint(LibUsbDevice::Pointer libUsbDevice, boost::asio::io_service& ioService, const libusb_endpoint_descriptor* endpoint);

    void controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    uint8_t getAddress() override;
    void cancelTransfers() override;

private:
    typedef std::unordered_map<libusb_transfer*, Promise::Pointer> Transfers;

    void transfer(libusb_transfer *transfer, Promise::Pointer promise);
    static void transferHandler(libusb_transfer *transfer);

    LibUsbDevice::Pointer libUsbDevice_;
    boost::asio::io_service::strand strand_;
    uint8_t endpointAddress_;
    Transfers transfers_;
};

}
}
