#include <cstring>
#include <transport/DataSink.hpp>
#include <error/Error.hpp>

namespace aasdk
{
namespace transport
{

DataSink::DataSink()
    : data_(common::cStaticDataSize)
{
}

common::DataBuffer DataSink::fill()
{
    const auto offset = data_.size();
    data_.resize(data_.size() + cChunkSize);

    auto ptr = data_.is_linearized() ? &data_[offset] : data_.linearize() + offset;
    return common::DataBuffer(ptr, cChunkSize);
}

void DataSink::commit(common::Data::size_type size)
{
    if(size > cChunkSize)
    {
        throw error::Error(error::ErrorCode::DATA_SINK_COMMIT_OVERFLOW);
    }

//    common::Data data(size, 0);
//    std::copy(data_.begin(), data_.begin() + size, data.begin());

    data_.erase_end((cChunkSize - size));
}

common::Data::size_type DataSink::getAvailableSize()
{
    return data_.size();
}

std::shared_ptr<common::Data> DataSink::consume(common::Data::size_type size)
{
    if(size > data_.size())
    {
        throw error::Error(error::ErrorCode::DATA_SINK_CONSUME_UNDERFLOW);
    }

    auto data = std::make_shared<common::Data>(size, 0);
    std::copy(data_.begin(), data_.begin() + size, data->begin());
    data_.erase_begin(size);

    return data;
}

}
}
