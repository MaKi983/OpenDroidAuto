#pragma once

#include <tcp/ITCPWrapper.hpp>

namespace aasdk
{
namespace tcp
{

class TCPWrapper: public ITCPWrapper
{
public:
    void asyncWrite(boost::asio::ip::tcp::socket& socket, common::DataConstBuffer buffer, Handler handler) override;
    void asyncRead(boost::asio::ip::tcp::socket& socket, common::DataBuffer buffer, Handler handler) override;
    void close(boost::asio::ip::tcp::socket& socket) override;
    void asyncConnect(boost::asio::ip::tcp::socket& socket, const std::string& hostname, uint16_t port, ConnectHandler handler) override;
    boost::system::error_code connect(boost::asio::ip::tcp::socket& socket, const std::string& hostname, uint16_t port) override;
};

}
}
