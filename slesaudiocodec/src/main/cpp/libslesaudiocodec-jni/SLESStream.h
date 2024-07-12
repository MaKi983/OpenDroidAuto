#pragma once

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <Data.hpp>

#pragma once

#include <Data.hpp>

class Packet {
public:
    typedef Packet* Pointer;

//    Packet(common::DataConstBuffer buf, int64_t ts) : timestamp(ts), size(buf.size) {
    Packet(common::Data buf, int64_t ts, size_t size) : buffer(std::move(buf)), cbuffer(buffer), timestamp(ts), size(buf.size()) {
    }

    static common::Data toData(common::DataConstBuffer& buffer, int64_t timestamp){
        common::Data data;

        const common::DataConstBuffer ts(&timestamp, sizeof(timestamp));
        common::copy(data, ts);

        const common::DataConstBuffer s(&buffer.size, sizeof(buffer.size));
        common::copy(data, s);

        common::copy(data, buffer);

        return data;
    }

    static Packet::Pointer fromData(common::Data data, int offset){
        int64_t timestamp = reinterpret_cast<const int64_t&>(data[offset]);
//        Log_v("timestamp %lld", timestamp);
        size_t size = reinterpret_cast<const size_t&>(data[offset + 8]);
//        Log_v("size %d", size);
        common::DataConstBuffer b(&data[offset], size + 12, 12);
//        Log_v("buf %s", common::dump(b).c_str());
        common::Data d;
        common::copy(d, b);

        return new Packet(d, timestamp, size);
    }

    size_t packetSize(){
        return sizeof(int64_t) + sizeof(size_t) + buffer.size();
    }

    common::Data buffer;
    common::DataConstBuffer cbuffer;
//    common::Data buf;
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
    SLEffectSendItf bqPlayerEffectSend;
//
//    // recorder interfaces
//    SLObjectItf recorderObject;
//    SLRecordItf recorderRecord;
//    SLAndroidSimpleBufferQueueItf recorderBufferQueue;
//
//    // buffer indexes
//    int currentInputIndex;
//    int currentOutputIndex;
//
//    // current buffer half (0, 1)
//    int currentOutputBuffer;
//    int currentInputBuffer;
//
//    // buffers
//    short *outputBuffer[2];
//    short *inputBuffer[2];
//
//    // size of buffers
//    int outBufSamples;
//    int inBufSamples;
//
//    // locks
//    void*  inlock;
//    void*  outlock;
//
//    double time;
//    int inchannels;
//    int outchannels;
//    int sr;
};
