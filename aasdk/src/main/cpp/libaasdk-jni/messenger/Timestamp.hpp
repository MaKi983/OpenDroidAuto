#pragma once

#include <common/Data.hpp>


namespace aasdk
{
namespace messenger
{

class Timestamp
{
public:
    typedef uint64_t ValueType;

    Timestamp(ValueType stamp);
    Timestamp(const common::DataConstBuffer& buffer);

    common::Data getData() const;
    ValueType getValue() const;

private:
    ValueType stamp_;
};

}
}
