#include <tcp/TCPEndpoint.hpp>

namespace aasdk
{
namespace tcp
{

TCPEndpoint::TCPEndpoint(ITCPWrapper& tcpWrapper, SocketPointer socket)
    : tcpWrapper_(tcpWrapper)
    , socket_(std::move(socket))
{

}

TCPEndpoint::~TCPEndpoint(){
    delete socket_;
}

void TCPEndpoint::send(common::DataConstBuffer buffer, Promise::Pointer promise)
{
    tcpWrapper_.asyncWrite(*socket_, std::move(buffer),
                           std::bind(&TCPEndpoint::asyncOperationHandler,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2,
                                     std::move(promise)));
}

void TCPEndpoint::receive(common::DataBuffer buffer, Promise::Pointer promise)
{
    tcpWrapper_.asyncRead(*socket_, std::move(buffer),
                          std::bind(&TCPEndpoint::asyncOperationHandler,
                                    this,
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
    if(!ec)
    {
        promise->resolve(bytesTransferred);
    }
    else
    {
        auto error = ec == boost::asio::error::operation_aborted ? error::Error(error::ErrorCode::OPERATION_ABORTED) : error::Error(error::ErrorCode::TCP_TRANSFER, static_cast<uint32_t>(ec.value()));
        promise->reject(error);
    }
}

}
}