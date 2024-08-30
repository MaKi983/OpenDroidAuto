#pragma once

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <Data.hpp>

typedef std::vector<uint16_t> Data16;

class Packet {
public:
    typedef Packet* Pointer;

    Packet(common::Data buf, int64_t ts, size_t size) : buffer(std::move(buf)), cbuffer(buffer), timestamp(ts), size(buf.size()) {
    }


    common::Data buffer;
    common::DataConstBuffer cbuffer;
    const size_t size;
    const int64_t timestamp;

};

class SLESStream {

public:
    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // buffer queue player interfaces
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
};
