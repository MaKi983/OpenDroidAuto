#pragma once

#include <memory>
#include <common/Data.hpp>

namespace aasdk
{
namespace messenger
{

class ICryptor
{
public:
    typedef ICryptor* Pointer;

    ICryptor() = default;
    virtual ~ICryptor() = default;

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual bool doHandshake() = 0;
    virtual size_t encrypt(common::Data& output, const common::DataConstBuffer& buffer) = 0;
    virtual size_t decrypt(common::Data& output, const common::DataConstBuffer& buffer) = 0;
    virtual common::Data readHandshakeBuffer() = 0;
    virtual void writeHandshakeBuffer(const common::DataConstBuffer& buffer) = 0;
    virtual bool isActive() const = 0;
};

}
}
