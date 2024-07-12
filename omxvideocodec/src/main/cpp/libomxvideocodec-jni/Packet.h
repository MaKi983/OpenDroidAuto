#pragma once

#include <Data.hpp>
#include <Log.h>

class Packet {
public:
    typedef Packet* Pointer;

    Packet(common::Data buf, int64_t ts, size_t size) : buffer(std::move(buf)), timestamp(ts), size(size) {
    }

    static common::Data toData(common::DataConstBuffer& buffer, int64_t timestamp){
        common::Data data;

        const common::DataConstBuffer ts(&timestamp, sizeof(timestamp));
        common::copy(data, ts);

        const common::DataConstBuffer s(&buffer.size, sizeof(size));
        common::copy(data, s);

        common::copy(data, buffer);

        return data;
    }

    static Packet::Pointer fromData(common::Data data, int offset){
        int64_t timestamp = reinterpret_cast<const int64_t&>(data[offset]);
        size_t size = reinterpret_cast<const size_t&>(data[offset + 8]);
        if (Log::isVerbose()) Log_v("readed buffer size %d", size);
        size_t currentSize = size;
        if (size > (data.size()-offset-12) ){
            Log_w("Not full data available");
            currentSize = data.size()-offset-12;
        }
        common::DataConstBuffer b(&data[offset], currentSize + 12, 12);
        common::Data d;
        common::copy(d, b);

        return new Packet(d, timestamp, size);
    }

    size_t packetSize(){
        return sizeof(int64_t) + sizeof(size_t) + size;
    }

    common::Data buffer;
    const size_t size;
    const int64_t timestamp;






};