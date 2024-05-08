#pragma once

#include "IPinger.h"

namespace service
{

class Pinger: public IPinger
{
public:
    Pinger(boost::asio::io_service& ioService, time_t duration);

    void ping(Promise::Pointer promise) override;
    void pong() override;
    void cancel() override;

private:

    void onTimerExceeded(const boost::system::error_code& error);

    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer_;
    time_t duration_;
    bool cancelled_;
    Promise::Pointer promise_;
    int64_t pingsCount_;
    int64_t pongsCount_;
};

}
