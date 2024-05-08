#pragma once

#include <mutex>
#include <transport/ISSLWrapper.hpp>
#include <messenger/ICryptor.hpp>

namespace aasdk
{
namespace messenger
{

class Cryptor: public ICryptor
{
public:
    Cryptor(transport::ISSLWrapper::Pointer sslWrapper);
    ~Cryptor();

    void init() override;
    void deinit() override;
    bool doHandshake() override;
    size_t encrypt(common::Data& output, const common::DataConstBuffer& buffer) override;
    size_t decrypt(common::Data& output, const common::DataConstBuffer& buffer) override;

    common::Data readHandshakeBuffer() override;
    void writeHandshakeBuffer(const common::DataConstBuffer& buffer) override;
    bool isActive() const override;

private:
    size_t read(common::Data& output);
    void write(const common::DataConstBuffer& buffer);

    transport::ISSLWrapper::Pointer sslWrapper_;
    size_t maxBufferSize_;
    X509* certificate_;
    EVP_PKEY* privateKey_;
    SSL_CTX* context_;
    SSL* ssl_;
    transport::ISSLWrapper::BIOs bIOs_;
    bool isActive_;

    const static std::string cCertificate;
    const static std::string cPrivateKey;
    mutable std::mutex mutex_;
};

}
}
