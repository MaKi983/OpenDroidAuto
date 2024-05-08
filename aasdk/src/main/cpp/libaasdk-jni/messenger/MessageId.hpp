#pragma once

#include <common/Data.hpp>

namespace aasdk
{
namespace messenger
{

class MessageId
{
public:
    MessageId(uint16_t id);
    MessageId(const common::Data& data);

    common::Data getData() const;
    static constexpr size_t getSizeOf() { return 2; }
    uint16_t getId() const;

    bool operator>(uint16_t id) const;
    bool operator<(uint16_t id) const;
    bool operator==(uint16_t id) const;
    bool operator>=(uint16_t id) const;
    bool operator<=(uint16_t id) const;
    MessageId& operator=(uint16_t id);

private:
    uint16_t id_;
};

std::ostream& operator<<(std::ostream& stream, const aasdk::messenger::MessageId& messageId);

}
}
