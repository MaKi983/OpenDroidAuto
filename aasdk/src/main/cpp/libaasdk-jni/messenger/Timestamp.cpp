#include <boost/endian/conversion.hpp>
#include <messenger/Timestamp.hpp>
#include <Log.h>
#include <inttypes.h>

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
    const ValueType& timestampBig = reinterpret_cast<const ValueType&>(buffer.cdata[0]);
//    const ValueType& timestampBig = reinterpret_cast<const ValueType&>(buffer.cdata);
    stamp_ = boost::endian::big_to_native(timestampBig);
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
