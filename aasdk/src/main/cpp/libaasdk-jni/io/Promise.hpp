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

#include <functional>
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <error/Error.hpp>
#include <io/IOContextWrapper.hpp>
#include <Log.h>

namespace aasdk
{
namespace io
{

template<typename ResolveArgumentType, typename ErrorArgumentType = error::Error>
class Promise: boost::noncopyable, public std::enable_shared_from_this<Promise<ResolveArgumentType, ErrorArgumentType>>
{
public:
    typedef ResolveArgumentType ValueType;
    typedef ErrorArgumentType ErrorType;
    typedef std::function<void(ResolveArgumentType)> ResolveHandler;
    typedef std::function<void(ErrorArgumentType)> RejectHandler;
    typedef std::shared_ptr<Promise> Pointer;

    static Pointer defer(boost::asio::io_service& ioService, std::string name) {
        return std::make_shared<Promise>(ioService, name);
    }

    static Pointer defer(boost::asio::io_service::strand& strand, std::string name)
    {
        return std::make_shared<Promise>(strand, name);
    }

    Promise(boost::asio::io_service& ioService, std::string name)
        : ioContextWrapper_(ioService)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    Promise(boost::asio::io_service::strand& strand, std::string name)
        : ioContextWrapper_(strand)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    ~Promise(){
        if (Log::isVerbose()) Log_v("%s/destructor", name_.c_str());
        ioContextWrapper_.reset();
        rejectHandler_ = RejectHandler();
        resolveHandler_ = ResolveHandler();
    }

    void then(ResolveHandler resolveHandler, RejectHandler rejectHandler = RejectHandler())
    {
        resolveHandler_ = std::move(resolveHandler);
        rejectHandler_ = std::move(rejectHandler);
    }

    void resolve(ResolveArgumentType argument)
    {
        if (Log::isVerbose()) Log_v("%s/resolve", name_.c_str());
        if(resolveHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this(), argument = std::move(argument)]() mutable {
                resolveHandler_(std::move(argument));
            });
        }
    }

    void reject(ErrorArgumentType error)
    {
        if (Log::isVerbose()) Log_v("%s/reject", name_.c_str());
        if(rejectHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this(), error = std::move(error)]() mutable {
                rejectHandler_(std::move(error));
            });
        }
    }

private:
//    bool isPending() const
//    {
//        return ioContextWrapper_.isActive();
//    }

    ResolveHandler resolveHandler_;
    RejectHandler rejectHandler_;
    IOContextWrapper ioContextWrapper_;
    std::string name_;
};

template<typename ErrorArgumentType>
class Promise<void, ErrorArgumentType>: boost::noncopyable, public std::enable_shared_from_this<Promise<void, ErrorArgumentType>>
{
public:
    typedef ErrorArgumentType ErrorType;
    typedef std::function<void()> ResolveHandler;
    typedef std::function<void(ErrorArgumentType)> RejectHandler;
    typedef std::shared_ptr<Promise> Pointer;

    static Pointer defer(boost::asio::io_service& ioService, std::string name)
    {
        return std::make_shared<Promise>(ioService, name);
    }

    static Pointer defer(boost::asio::io_service::strand& strand, std::string name)
    {
        return std::make_shared<Promise>(strand, name);
    }

    Promise(boost::asio::io_service& ioService, std::string name)
        : ioContextWrapper_(ioService)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    Promise(boost::asio::io_service::strand& strand, std::string name)
        : ioContextWrapper_(strand)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    ~Promise(){
        if (Log::isVerbose()) Log_v("%s/destructor", name_.c_str());
        ioContextWrapper_.reset();
        rejectHandler_ = RejectHandler();
        resolveHandler_ = ResolveHandler();
    }

    void then(ResolveHandler resolveHandler, RejectHandler rejectHandler = RejectHandler())
    {
        resolveHandler_ = std::move(resolveHandler);
        rejectHandler_ = std::move(rejectHandler);
    }

    void resolve()
    {
        if (Log::isVerbose()) Log_v("%s/resolve", name_.c_str());
        if(resolveHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this()]() mutable {
                resolveHandler_();
            });
        }
    }

    void reject(ErrorArgumentType error)
    {
        if (Log::isVerbose()) Log_v("%s/reject", name_.c_str());
        if(rejectHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this(), error = std::move(error)]() mutable {
                rejectHandler_(std::move(error));
            });
        }
    }

private:
    ResolveHandler resolveHandler_;
    RejectHandler rejectHandler_;
    IOContextWrapper ioContextWrapper_;
    std::string name_;
};

template<>
class Promise<void, void>: boost::noncopyable, public std::enable_shared_from_this<Promise<void, void>>
{
public:
    typedef std::function<void()> ResolveHandler;
    typedef std::function<void()> RejectHandler;
    typedef std::shared_ptr<Promise> Pointer;

    static Pointer defer(boost::asio::io_service& ioService, std::string name)
    {
        return std::make_shared<Promise>(ioService, name);
    }

    static Pointer defer(boost::asio::io_service::strand& strand, std::string name)
    {
        return std::make_shared<Promise>(strand, name);
    }

    Promise(boost::asio::io_service& ioService, std::string name)
        : ioContextWrapper_(ioService)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    Promise(boost::asio::io_service::strand& strand, std::string name)
        : ioContextWrapper_(strand)
        , name_(std::move(name))
    {
    }

    ~Promise(){
        if (Log::isVerbose()) Log_v("%s/destructor", name_.c_str());
        ioContextWrapper_.reset();
        rejectHandler_ = RejectHandler();
        resolveHandler_ = ResolveHandler();
    }

    void then(ResolveHandler resolveHandler, RejectHandler rejectHandler = RejectHandler())
    {
        resolveHandler_ = std::move(resolveHandler);
        rejectHandler_ = std::move(rejectHandler);
    }

    void resolve()
    {
        if (Log::isVerbose()) Log_v("%s/resolve", name_.c_str());
        if(resolveHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this()]() mutable {
                resolveHandler_();
            });
        }
    }

    void reject()
    {
        Log_v("%s/reject", name_.c_str());
        if(rejectHandler_ != nullptr)
        {
            ioContextWrapper_.post([this, self = this->shared_from_this()]() mutable {
                rejectHandler_();
            });
        }
    }

private:
    ResolveHandler resolveHandler_;
    RejectHandler rejectHandler_;
    IOContextWrapper ioContextWrapper_;
    std::string name_;
};

template<typename ResolveArgumentType>
class Promise<ResolveArgumentType, void>: boost::noncopyable, public std::enable_shared_from_this<Promise<ResolveArgumentType, void>>
{
public:
    typedef ResolveArgumentType ValueType;
    typedef std::function<void(ResolveArgumentType)> ResolveHandler;
    typedef std::function<void(void)> RejectHandler;
    typedef std::shared_ptr<Promise> Pointer;

    static Pointer defer(boost::asio::io_service& ioService, std::string name)
    {
        return std::make_shared<Promise>(ioService, name);
    }

    static Pointer defer(boost::asio::io_service::strand& strand, std::string name)
    {
        return std::make_shared<Promise>(strand, name);
    }

    Promise(boost::asio::io_service& ioService, std::string name)
        : ioContextWrapper_(ioService)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    Promise(boost::asio::io_service::strand& strand, std::string name)
        : ioContextWrapper_(strand)
        , name_(std::move(name))
    {
        if (Log::isVerbose()) Log_v("%s/create", name_.c_str());
    }

    ~Promise(){
        if (Log::isVerbose()) Log_v("%s/destructor", name_.c_str());
        ioContextWrapper_.reset();
        rejectHandler_ = RejectHandler();
        resolveHandler_ = ResolveHandler();
    }

    void then(ResolveHandler resolveHandler, RejectHandler rejectHandler = RejectHandler())
    {
        resolveHandler_ = std::move(resolveHandler);
        rejectHandler_ = std::move(rejectHandler);
    }

    void resolve(ResolveArgumentType argument)
    {
        if (Log::isVerbose()) Log_v("%s/resolve", name_.c_str());
        if(resolveHandler_ != nullptr)
        {
            ioContextWrapper_.post([self = this->shared_from_this(), argument = std::move(argument), resolveHandler = resolveHandler_, name = name_]() mutable {
                resolveHandler(std::move(argument));
            });
        }
    }

    void reject()
    {
        if (Log::isVerbose()) Log_v("%s/reject", name_.c_str());
        if(rejectHandler_ != nullptr )
        {
            ioContextWrapper_.post([this, self = this->shared_from_this()]() mutable {
                rejectHandler_();
            });
        }
    }

private:
    ResolveHandler resolveHandler_;
    RejectHandler rejectHandler_;
    IOContextWrapper ioContextWrapper_;
    std::string name_;
};


}
}
