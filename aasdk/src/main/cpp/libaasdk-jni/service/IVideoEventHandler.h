#pragma once

namespace service
{

    class IVideoEventHandler
    {
    public:
        typedef IVideoEventHandler* Pointer;

        virtual ~IVideoEventHandler() = default;
        virtual void onAVChannelStopIndication() = 0;
    };

}
