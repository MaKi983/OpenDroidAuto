#include <tcp/TCPEndpoint.hpp>
#include <Log.h>

namespace aasdk
{
namespace tcp
{

TCPEndpoint::TCPEndpoint(ITCPWrapper& tcpWrapper, SocketPointer socket)
    : tcpWrapper_(tcpWrapper)
    , socket_(std::move(socket))
{

}

//TCPEndpoint::~TCPEndpoint(){
//    delete socket_;
//}

void TCPEndpoint::send(common::DataConstBuffer buffer, Promise::Pointer promise) {
    if (Log::isDebug()) Log_d("asyncWrite");
    tcpWrapper_.asyncWrite(*socket_, std::move(buffer),
                           std::bind(&TCPEndpoint::asyncOperationHandler,
                                     this->shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2,
                                     std::move(promise)));
}

void TCPEndpoint::receive(common::DataBuffer buffer, Promise::Pointer promise)
{
    if (Log::isDebug()) Log_d("asyncRead");
    tcpWrapper_.asyncRead(*socket_, std::move(buffer),
                          std::bind(&TCPEndpoint::asyncOperationHandler,
                                    this->shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2,
                                    std::move(promise)));
}

void TCPEndpoint::stop()
{
    tcpWrapper_.close(*socket_);
}

void TCPEndpoint::asyncOperationHandler(const boost::system::error_code& ec, size_t bytesTransferred, Promise::Pointer promise)
{
    if (Log::isDebug()) Log_d("asyncOperationHandler");
    if(!ec)
    {
        if (Log::isVerbose()) Log_v("not error, resolve promise -> bytesTransferred %d", bytesTransferred);
        promise->resolve(bytesTransferred);
    }
    else
    {
        Log_e("error! reject promise");
        auto error = ec == boost::asio::error::operation_aborted ? error::Error(error::ErrorCode::OPERATION_ABORTED) : error::Error(error::ErrorCode::TCP_TRANSFER, static_cast<uint32_t>(ec.value()));
        Log_e("tcp error %d/%s", error.getNativeCode(), error.what());
        promise->reject(error);
    }
}

}
}