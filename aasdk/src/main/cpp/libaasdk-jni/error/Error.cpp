#include <error/Error.hpp>

namespace aasdk
{
namespace error
{

Error::Error()
    : code_(ErrorCode::NONE)
    , nativeCode_(0)
{

}

Error::Error(ErrorCode code, uint32_t nativeCode)
    : code_(code)
    , nativeCode_(nativeCode)
{
    message_ = "AaSdk error code: " + std::to_string(static_cast<uint32_t>(code_))
                + ", native code: " + std::to_string(nativeCode_);
}

ErrorCode Error::getCode() const
{
    return code_;
}

uint32_t Error::getNativeCode() const
{
    return nativeCode_;
}

const char* Error::what() const noexcept
{
    return message_.c_str();
}

bool Error::operator!() const
{
    return code_ == ErrorCode::NONE;
}

bool Error::operator==(const Error& other) const
{
    return code_ == other.code_ && nativeCode_ == other.nativeCode_;
}

bool Error::operator==(const ErrorCode& code) const
{
    return code_ == code;
}

bool Error::operator!=(const ErrorCode& code) const
{
    return !operator==(code);
}

}
}
