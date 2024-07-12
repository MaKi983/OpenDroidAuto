#include "Pinger.h"
#include "service/Pinger.h"

namespace service
{

Pinger::Pinger(boost::asio::io_service& ioService, time_t duration)
        : strand_(ioService)
        , timer_(ioService)
        , duration_(duration)
        , cancelled_(false)
        , pingsCount_(0)
        , pongsCount_(0)
{

}

void Pinger::ping(Promise::Pointer promise)
{
    strand_.dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
        cancelled_ = false;

        if(promise_ != nullptr)
        {
            promise_->reject(aasdk::error::Error(aasdk::error::ErrorCode::OPERATION_IN_PROGRESS));
        }
        else
        {
            ++pingsCount_;

            promise_ = std::move(promise);
            timer_.expires_from_now(boost::posix_time::milliseconds(duration_));
            timer_.async_wait(strand_.wrap(std::bind(&Pinger::onTimerExceeded, this->shared_from_this(), std::placeholders::_1)));
        }
    });
}

void Pinger::pong()
{
    strand_.dispatch([this, self = this->shared_from_this()]() {
        ++pongsCount_;
    });
}

void Pinger::onTimerExceeded(const boost::system::error_code& error)
{
    if(promise_ == nullptr)
    {
        return;
    }
    else if(error == boost::asio::error::operation_aborted || cancelled_)
    {
        promise_->reject(aasdk::error::Error(aasdk::error::ErrorCode::OPERATION_ABORTED));
    }
    else if(pingsCount_ - pongsCount_ > 1)
    {
        promise_->reject(aasdk::error::Error());
    }
    else
    {
        promise_->resolve();
    }

    promise_.reset();
}

void Pinger::cancel()
{
    cancelled_ = true;
    timer_.cancel();
}

}
