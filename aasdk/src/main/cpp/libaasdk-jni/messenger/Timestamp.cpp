#include <boost/endian/conversion.hpp>
#include <messenger/Timestamp.hpp>
#include <Log.h>
#include <chrono>

namespace aasdk
{
namespace messenger
{

Timestamp::Timestamp(ValueType stamp)
    : stamp_(stamp)
{

}

Timestamp::Timestamp(const common::DataConstBuffer& buffer)
{
//    const ValueType& timestampBig = reinterpret_cast<const ValueType&>(buffer.cdata[0]); // generates sigbus on release build
//    auto timestampBig = reinterpret_cast<const ValueType>(&buffer.cdata[0]); // result in wrong timestamp
//    stamp_ = boost::endian::big_to_native(timestampBig);
    stamp_ = std::chrono::system_clock::now().time_since_epoch().count();
    Log_v("stamp_ %lld", stamp_);
}

common::Data Timestamp::getData() const
{
    const ValueType timestampBig = boost::endian::native_to_big(stamp_);
    const common::DataConstBuffer timestampBuffer(&timestampBig, sizeof(timestampBig));
    return common::createData(timestampBuffer);
}

Timestamp::ValueType Timestamp::getValue() const
{
    return stamp_;
}

}
}
