#pragma once

#include <Data.hpp>
#include <Log.h>

class Packet {
public:
    typedef Packet* Pointer;

    Packet(common::Data buf, int64_t ts, size_t size) : buffer(std::move(buf)), timestamp(ts), size(size) {}

    common::Data buffer;
    const size_t size;
    const int64_t timestamp;

};
