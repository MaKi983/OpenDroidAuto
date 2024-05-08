#pragma once

#include <string>

namespace aasdk
{
namespace messenger
{

enum class EncryptionType
{
    PLAIN,
    ENCRYPTED = 1 << 3
};

std::string encryptionTypeToString(EncryptionType encryptionType);

}
}
