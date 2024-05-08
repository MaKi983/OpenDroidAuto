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

#include <memory>
#include <common/Data.hpp>
#include <io/Promise.hpp>


namespace aasdk
{
namespace usb
{

class IUSBEndpoint
{
public:
    typedef IUSBEndpoint* Pointer;
    typedef io::Promise<size_t> Promise;

    IUSBEndpoint() = default;
    virtual ~IUSBEndpoint() = default;

    virtual uint8_t getAddress() = 0;
    virtual void controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) = 0;
    virtual void bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) = 0;
    virtual void interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) = 0;
    virtual void cancelTransfers() = 0;
};

}
}
