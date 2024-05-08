#pragma once

#include "io/Promise.hpp"

namespace service
{

    class IPinger
    {
    public:
        typedef IPinger* Pointer;
        typedef aasdk::io::Promise<void> Promise;

        virtual ~IPinger() = default;
        virtual void ping(Promise::Pointer promise) = 0;
        virtual void pong() = 0;
        virtual void cancel() = 0;
    };

}
