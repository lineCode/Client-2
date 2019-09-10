// publickey.cpp
//

///// Includes /////

#include "utility/publickey.hpp"

#include <memory>
#include <openssl/err.h>
#include <openssl/pem.h>

///// Namespaces /////

namespace utility
{

///// Methods /////

PublicKey::PublicKey() :
  key_(nullptr)
{
  
}

PublicKey::PublicKey(const PublicKey& rhs) :
  key_(RSAPublicKey_dup(rhs.key_))
{

}

PublicKey::~PublicKey()
{
  Destroy();

}

int PublicKey::Init(const std::vector<char>& data)
{
  Destroy();

  BIO* bio = BIO_new_mem_buf(const_cast<char*>(data.data()), static_cast<int>(data.size()));
  if (!bio)
  {

    return 1;
  }

  key_ = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
  if (!key_)
  {
    BIO_free(bio);
    return 1;
  }

  BIO_free(bio);
  return 0;
}

int PublicKey::Init(const std::string& text)
{
  BIO* bio = BIO_new_mem_buf((const void*)text.data(), static_cast<int>(text.size()));
  if (!PEM_read_bio_RSAPublicKey(bio, &key_, nullptr, nullptr))
  {
    BIO_free(bio);
    return 1;
  }
  BIO_free(bio);
  return 0;
}

int PublicKey::Init(const boost::filesystem::path& path)
{
  Destroy();

  FILE* licensersa = fopen(path.string().c_str(), "rb");
  if (!licensersa)
  {

    return 1;
  }

  key_ = PEM_read_RSA_PUBKEY(licensersa, nullptr, nullptr, nullptr);
  fclose(licensersa);
  if (!key_)
  {

    return 1;
  }

  return 0;
}

int PublicKey::Init(const PrivateKey& privatekey)
{
  Destroy();

  if (!privatekey.GetKey())
  {

    return 1;
  }

  BIO* mem = BIO_new(BIO_s_mem());
  if (PEM_write_bio_RSAPublicKey(mem, privatekey.GetKey()) != 1)
  {
    BIO_free(mem);
    return 1;
  }

  key_ = PEM_read_bio_RSAPublicKey(mem, &key_, nullptr, nullptr);
  if (!key_)
  {
    BIO_free(mem);
    return 1;
  }

  BIO_free(mem);
  return 0;
}

void PublicKey::Destroy()
{
  if (key_)
  {
    RSA_free(key_);
    key_ = nullptr;
  }
}

std::string PublicKey::GetString() const
{
  EVP_PKEY* pkey_ = EVP_PKEY_new();
  if (!pkey_)
  {

    return std::string();
  }

  if (EVP_PKEY_set1_RSA(pkey_, key_) != 1)
  {
    EVP_PKEY_free(pkey_);
    return std::string();
  }

  BIO* bio = BIO_new(BIO_s_mem());
  if (!bio)
  {
    EVP_PKEY_free(pkey_);
    return std::string();
  }

  if (PEM_write_bio_RSAPublicKey(bio, key_) != 1)
  {
    BIO_free(bio);
    EVP_PKEY_free(pkey_);
    return std::string();
  }

  const int len = BIO_pending(bio);
  std::unique_ptr<char[]> text = std::make_unique<char[]>(len + 1);

  if (BIO_read(bio, text.get(), len) != len)
  {
    BIO_free(bio);
    EVP_PKEY_free(pkey_);
    return std::string();
  }

  BIO_free(bio);
  EVP_PKEY_free(pkey_);
  return std::string(text.get(), len);
}

bool PublicKey::Verify(const unsigned char* message, const size_t size, const unsigned char* signature) const
{
  if (!key_)
  {

    return false;
  }

  EVP_PKEY* pkey = EVP_PKEY_new();
  if (!pkey)
  {

    return false;
  }

  if (EVP_PKEY_set1_RSA(pkey, key_) != 1)
  {
    EVP_PKEY_free(pkey);
    return false;
  }

  EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
  if (!mdctx)
  {
    EVP_PKEY_free(pkey);
    return false;
  }

  if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pkey) != 1)
  {
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_destroy(mdctx);
    return false;
  }
  if (EVP_DigestVerifyUpdate(mdctx, message, size) != 1)
  {
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_destroy(mdctx);
    return false;
  }

  if (EVP_DigestVerifyFinal(mdctx, signature, 256) == 1) // Promise not to change
  {
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_destroy(mdctx);
    return true;
  }
  else
  {
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_destroy(mdctx);
    return false;
  }
}

}
