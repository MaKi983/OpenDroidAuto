#include <transport/USBTransport.hpp>

namespace aasdk
{
namespace transport
{

USBTransport::USBTransport(boost::asio::io_service& ioService, usb::IAOAPDevice::Pointer aoapDevice)
    : Transport(ioService)
    , aoapDevice_(std::move(aoapDevice))
{}

void USBTransport::enqueueReceive(common::DataBuffer buffer)
{
    if (Log::isVerbose()) Log_v("enqueueReceive");
    auto usbEndpointPromise = usb::IUSBEndpoint::Promise::defer(receiveStrand_, "USBTransport_receive");
    usbEndpointPromise->then([this, self = this->shared_from_this()](auto bytesTransferred) {
            this->receiveHandler(bytesTransferred);
        },
        [this, self = this->shared_from_this()](auto e) {
            this->rejectReceivePromises(e);
        });

    if (Log::isVerbose()) Log_v("bulkTransfer inEndpoint");
    aoapDevice_->getInEndpoint().bulkTransfer(buffer, cReceiveTimeoutMs, std::move(usbEndpointPromise));
}

void USBTransport::enqueueSend(SendQueue::iterator queueElement)
{
    if (Log::isVerbose()) Log_v("enqueueSend");
    this->doSend(queueElement, 0);
}

void USBTransport::doSend(SendQueue::iterator queueElement, common::Data::size_type offset)
{
    auto usbEndpointPromise = usb::IUSBEndpoint::Promise::defer(sendStrand_, "USBTransport_send");
    usbEndpointPromise->then([this, self = this->shared_from_this(), queueElement, offset](size_t bytesTransferred) mutable {
            this->sendHandler(queueElement, offset, bytesTransferred);
        },
        [this, self = this->shared_from_this(), queueElement](const error::Error& e) mutable {
            queueElement->second->reject(e);
            sendQueue_.erase(queueElement);

            if(!sendQueue_.empty()) {
                this->doSend(sendQueue_.begin(), 0);
            }
        });

    if (Log::isVerbose()) Log_v("bulkTransfer outEndpoint");
    aoapDevice_->getOutEndpoint().bulkTransfer(common::DataBuffer(queueElement->first, offset), cSendTimeoutMs, std::move(usbEndpointPromise));
}

void USBTransport::sendHandler(SendQueue::iterator queueElement, common::Data::size_type offset, size_t bytesTransferred) {
    if(offset + bytesTransferred < queueElement->first.size()) {
        if (Log::isVerbose()) Log_v("missing data to receive -> %d", (queueElement->first.size() - offset-bytesTransferred) );
        this->doSend(queueElement, offset + bytesTransferred);
    } else {
        queueElement->second->resolve();
        sendQueue_.erase(queueElement);

        if(!sendQueue_.empty()) {
            this->doSend(sendQueue_.begin(), 0);
        }
    }
}

void USBTransport::stop()
{
    aoapDevice_->getInEndpoint().cancelTransfers();
    aoapDevice_->getOutEndpoint().cancelTransfers();
}

}
}
