#include "LibUsbDevice.h"
#include <Log.h>
#include <iomanip>
#include <sstream>
#include <endian.h>
#include <error/Error.hpp>

namespace aasdk
{
namespace usb
{

LibUsbDevice::LibUsbDevice(libusb_context* ctx) :
    ctx_(ctx)
    , handle_(nullptr){

}

int LibUsbDevice::open(int fd) {
    int result = libusb_wrap_sys_device(ctx_, fd, &handle_);
    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_wrap_sys_device failed: (%d / %s)", result, libusb_error_name(result));
        handle_ = nullptr;
    }
//    result = libusb_set_auto_detach_kernel_driver(handle_, 1);
//    if (result < LIBUSB_SUCCESS) {
//        Log_e("libusb_set_auto_detach_kernel_driver failed: (%d / %s)", result, libusb_error_name(result));
//        handle_ = nullptr;
//    }

    return result;
}

void LibUsbDevice::close() {
    if (handle_) {
        libusb_close(handle_);
        handle_ = nullptr;
    }
}

DeviceHandle LibUsbDevice::handle() {
    return handle_;
}

ConfigDescriptorHandle LibUsbDevice::configDescriptor() {
    libusb_device* device = libusb_get_device(handle_);
    ConfigDescriptorHandle config;

    int result = libusb_get_config_descriptor(device, 0, &config);

    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_get_config_descriptor failed: (%d / %s)", result, libusb_error_name(result));
        throw error::Error(error::ErrorCode::USB_OBTAIN_CONFIG_DESCRIPTOR, result);
    }

    if(config == nullptr) {
        throw error::Error(error::ErrorCode::USB_INVALID_CONFIG_DESCRIPTOR, result);
    }

    if (Log::isVerbose()){
        Log_v("    ConfigDescriptor:\n");
        Log_v("      bConfigurationValue:      %u\n", config->bConfigurationValue);
        Log_v("      bDescriptorType:          %u\n", config->bDescriptorType);
        Log_v("      bNumInterfaces:           %u\n", config->bNumInterfaces);
        Log_v("      iConfiguration:           %u\n", config->iConfiguration);
        Log_v("      MaxPower:                 %u\n", config->MaxPower);
        Log_v("      bLength:                  %u\n", config->bLength);
        Log_v("      extra_length:             %d\n", config->extra_length);
        Log_v("      wTotalLength:             %d\n", config->wTotalLength);
        Log_v("      bmAttributes:             %02xh\n", config->bmAttributes);
    }

    return config;
}

int LibUsbDevice::deviceDescriptor(libusb_device_descriptor &descriptor) {
    libusb_device* device = libusb_get_device(handle_);
    int result = libusb_get_device_descriptor(device, &descriptor);
    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_get_device_descriptor failed: (%d / %s)", result, libusb_error_name(result));
    }

    if (descriptor.iManufacturer) {
        libusb_get_string_descriptor_ascii(handle_, descriptor.iManufacturer, manufacter, sizeof(manufacter));
        if(Log::isVerbose()) Log_v("manufacter: %s", manufacter);
    }

    if (descriptor.iProduct) {
        libusb_get_string_descriptor_ascii(handle_, descriptor.iProduct, product, sizeof(product));
        if(Log::isVerbose()) Log_v("product: %s", product);
    }

    if (descriptor.iSerialNumber) {
        libusb_get_string_descriptor_ascii(handle_, descriptor.iSerialNumber, serial, sizeof(serial));
        if(Log::isVerbose()) Log_v("serial: %s", serial);
    }

    return result;
}

const libusb_interface* LibUsbDevice::interface(const ConfigDescriptorHandle& configDescriptorHandle){
    if(Log::isVerbose()) Log_v("interface: %s", serial);
    if(configDescriptorHandle->bNumInterfaces == 0) {
        throw error::Error(error::ErrorCode::USB_EMPTY_INTERFACES);
    }

    return &(configDescriptorHandle->interface[0]);
}

const libusb_interface_descriptor* LibUsbDevice::interfaceDescriptor(const libusb_interface* interface) {
    if(interface->num_altsetting == 0) {
        throw error::Error(error::ErrorCode::USB_OBTAIN_INTERFACE_DESCRIPTOR);
    }

    return &interface->altsetting[0];
}

int LibUsbDevice::claimInterface(uint8_t bInterfaceNumber){

    int result = libusb_claim_interface(handle_, bInterfaceNumber);
    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_claim_interface failed:(%d / %s)", result, libusb_error_name(result));
    }

    return result;
}

int LibUsbDevice::releaseInterface(uint8_t bInterfaceNumber){

    int result = libusb_release_interface(handle_, bInterfaceNumber);
    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_release_interface failed:(%d / %s)", result, libusb_error_name(result));
    }

    return result;
}

void LibUsbDevice::resetDevice() {
    int result = libusb_reset_device(handle_);
    if (result < LIBUSB_SUCCESS) {
        Log_e("libusb_reset_device failed:(%d / %s)", result, libusb_error_name(result));
    }
}

}
}