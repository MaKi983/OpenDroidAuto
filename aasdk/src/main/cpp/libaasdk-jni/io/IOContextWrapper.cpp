#include <io/IOContextWrapper.hpp>

namespace aasdk
{
namespace io
{

IOContextWrapper::IOContextWrapper()
    : ioService_(nullptr)
    , strand_(nullptr)
{

}

IOContextWrapper::IOContextWrapper(boost::asio::io_service& ioService)
    : ioService_(&ioService)
    , strand_(nullptr)
{

}

IOContextWrapper::IOContextWrapper(boost::asio::io_service::strand& strand)
    : ioService_(nullptr)
    , strand_(&strand)
{

}

void IOContextWrapper::reset()
{
    ioService_ = nullptr;
    strand_ = nullptr;
}

bool IOContextWrapper::isActive() const
{
    return ioService_ != nullptr || strand_ != nullptr;
}

}
}
