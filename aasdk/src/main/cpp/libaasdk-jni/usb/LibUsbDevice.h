#pragma once

#include <JNIBase.h>
#include <libusb.h>
#include <unordered_map>
#include <vector>
#include <list>
#include <stdint.h>
#include <mutex>

namespace aasdk {
namespace usb {

typedef libusb_device_handle* DeviceHandle;
typedef std::list<libusb_device*> DeviceList;
typedef DeviceList* DeviceListHandle;
typedef libusb_config_descriptor* ConfigDescriptorHandle;

class LibUsbDevice {
public:
    typedef LibUsbDevice* Pointer;

    LibUsbDevice(libusb_context *ctx);

    int open(int fd);
    void close();
    void resetDevice();
    DeviceHandle handle();

    int claimInterface(uint8_t bInterfaceNumber);
    int releaseInterface(uint8_t bInterfaceNumber);

    ConfigDescriptorHandle configDescriptor();
    int deviceDescriptor(libusb_device_descriptor &descriptor);

    const libusb_interface* interface(const ConfigDescriptorHandle& configDescriptorHandle);
    const libusb_interface_descriptor* interfaceDescriptor(const libusb_interface* interface);

    unsigned char product[256];
    unsigned char serial[256];
    unsigned char manufacter[256];

private:
    DeviceHandle handle_;
    libusb_context *ctx_;
};

}
}
