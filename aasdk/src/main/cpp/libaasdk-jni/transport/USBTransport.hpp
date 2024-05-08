#pragma once

#include <boost/asio.hpp>
#include <transport/Transport.hpp>
#include <usb/IAOAPDevice.hpp>

namespace aasdk
{
namespace transport
{

class USBTransport: public Transport
{
public:
    typedef USBTransport *Pointer;

    USBTransport(boost::asio::io_service& ioService, usb::IAOAPDevice::Pointer aoapDevice);

    void stop() override;

private:
    void enqueueReceive(common::DataBuffer buffer) override;
    void enqueueSend(SendQueue::iterator queueElement) override;
    void doSend(SendQueue::iterator queueElement, common::Data::size_type offset);
    void sendHandler(SendQueue::iterator queueElement, common::Data::size_type offset, size_t bytesTransferred);

    usb::IAOAPDevice::Pointer aoapDevice_;

    static constexpr uint32_t cSendTimeoutMs = 10000;
    static constexpr uint32_t cReceiveTimeoutMs = 0;
};

}
}
