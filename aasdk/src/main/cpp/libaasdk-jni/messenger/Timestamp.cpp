#include <boost/endian/conversion.hpp>
#include <messenger/Timestamp.hpp>
#include <cstring>

namespace aasdk
{
namespace messenger
{

Timestamp::Timestamp(ValueType stamp)
    : stamp_(stamp)
{

}

Timestamp::Timestamp(const common::DataConstBuffer& buffer)
    : stamp_(0)
{
    if (buffer.size < sizeof(ValueType)) {
        return;
    }

    ValueType timestampBig;
    std::memcpy(&timestampBig, buffer.cdata, sizeof(timestampBig));
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
