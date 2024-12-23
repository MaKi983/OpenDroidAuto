
#pragma once


#include "boost/optional.hpp"
#include "boost/asio/strand.hpp"
#include "IOService.h"
#include "boost/asio/io_service.hpp"

namespace aasdk
{
namespace io
{

    typedef boost::optional<boost::asio::io_service::strand> strand;
//    typedef boost::optional<boost::asio::io_service> ioService;
    typedef boost::asio::io_service ioService;

}
}
