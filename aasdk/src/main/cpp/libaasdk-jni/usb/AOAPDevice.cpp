#include <stdexcept>
#include <usb/AOAPDevice.hpp>
#include <error/Error.hpp>
#include <Log.h>
#include "LibUsbEndpoint.h"

namespace aasdk
{
namespace usb
{

AOAPDevice::AOAPDevice(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService, const libusb_interface_descriptor* interfaceDescriptor)
    : libUsbDevice_(libUsbDevice)
    , interfaceDescriptor_(interfaceDescriptor)
{
    if((interfaceDescriptor->endpoint[0].bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
        inEndpoint_ = std::make_shared<LibUsbEndpoint>(libUsbDevice_, ioService, &interfaceDescriptor_->endpoint[0]);
        outEndpoint_ = std::make_shared<LibUsbEndpoint>(libUsbDevice_, ioService, &interfaceDescriptor_->endpoint[1]);
    } else {
        inEndpoint_ = std::make_shared<LibUsbEndpoint>(libUsbDevice_, ioService, &interfaceDescriptor_->endpoint[1]);
        outEndpoint_ = std::make_shared<LibUsbEndpoint>(libUsbDevice_, ioService, &interfaceDescriptor_->endpoint[0]);
    }
}

AOAPDevice::~AOAPDevice()
{
    inEndpoint_->cancelTransfers();
    outEndpoint_->cancelTransfers();

    libUsbDevice_->releaseInterface(interfaceDescriptor_->bInterfaceNumber);
    libUsbDevice_->resetDevice();

    inEndpoint_.reset();
    outEndpoint_.reset();
}

IUSBEndpoint& AOAPDevice::getInEndpoint()
{
    return *inEndpoint_;
}

IUSBEndpoint& AOAPDevice::getOutEndpoint()
{
    return *outEndpoint_;
}

IAOAPDevice::Pointer AOAPDevice::create(LibUsbDevice::Pointer libUsbDevice, aasdk::io::ioService& ioService) {

    auto configDescriptor = libUsbDevice->configDescriptor();
    auto interface = libUsbDevice->interface(configDescriptor);
    auto interfaceDescriptor = libUsbDevice->interfaceDescriptor(interface);

    if(Log::isVerbose()) Log_v("    Interface:\n");
    if(Log::isVerbose()) Log_v("      bInterfaceNumber:      %u\n", interfaceDescriptor->bInterfaceNumber);
    if(Log::isVerbose()) Log_v("      bAlternateSetting:     %u\n", interfaceDescriptor->bAlternateSetting);
    if(Log::isVerbose()) Log_v("      bNumEndpoints:         %u\n", interfaceDescriptor->bNumEndpoints);
    if(Log::isVerbose()) Log_v("      bInterfaceClass:       %u\n", interfaceDescriptor->bInterfaceClass);
    if(Log::isVerbose()) Log_v("      bInterfaceSubClass:    %u\n", interfaceDescriptor->bInterfaceSubClass);
    if(Log::isVerbose()) Log_v("      bInterfaceProtocol:    %u\n", interfaceDescriptor->bInterfaceProtocol);
    if(Log::isVerbose()) Log_v("      iInterface:            %u\n", interfaceDescriptor->iInterface);

    if(interfaceDescriptor->bNumEndpoints < 2) {
        throw error::Error(error::ErrorCode::USB_INVALID_DEVICE_ENDPOINTS);
    }

    auto result = libUsbDevice->claimInterface(interfaceDescriptor->bInterfaceNumber);

    if(result != LIBUSB_SUCCESS) {
        throw error::Error(error::ErrorCode::USB_CLAIM_INTERFACE, result);
    }

//    return std::make_unique<AOAPDevice>(libUsbDevice, ioService, interfaceDescriptor);
    return new AOAPDevice(libUsbDevice, ioService, interfaceDescriptor);
}

}
}
