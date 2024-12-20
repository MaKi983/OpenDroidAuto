/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <utility>
#include <boost/asio.hpp>
#include <usb/IAOAPDevice.hpp>
#include "LibUsbDevice.h"


namespace aasdk
{
namespace usb
{

class AOAPDevice: public IAOAPDevice, boost::noncopyable
{
public:
    typedef AOAPDevice* Pointer;

    AOAPDevice(LibUsbDevice::Pointer libUsbDevice, boost::asio::io_service& ioService, const libusb_interface_descriptor* interfaceDescriptor);
    ~AOAPDevice() override;

    IUSBEndpoint& getInEndpoint() override;
    IUSBEndpoint& getOutEndpoint() override;

    static IAOAPDevice::Pointer create(LibUsbDevice::Pointer usbDevice, boost::asio::io_service& ioService);

private:
    LibUsbDevice::Pointer libUsbDevice_;
    DeviceHandle handle_;
    const libusb_interface_descriptor* interfaceDescriptor_;
    IUSBEndpoint::Pointer inEndpoint_;
    IUSBEndpoint::Pointer outEndpoint_;

    static constexpr uint16_t cGoogleVendorId = 0x18D1;
    static constexpr uint16_t cAOAPId = 0x2D00;
    static constexpr uint16_t cAOAPWithAdbId = 0x2D01;
};

}
}
