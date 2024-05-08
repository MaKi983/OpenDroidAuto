#include <messenger/EncryptionType.hpp>

namespace aasdk
{
namespace messenger
{

std::string encryptionTypeToString(EncryptionType encryptionType)
{
    switch(encryptionType)
    {
    case EncryptionType::PLAIN:
        return "PLAIN";
    case EncryptionType::ENCRYPTED:
        return "ENCRYPTED";
    default:
        return "(unknown)";
    }
}

}
}
