// privatekey.cpp
//

///// Includes /////

#include "utility/privatekey.hpp"

#include <boost/scope_exit.hpp>
#include <fstream>
#include <memory>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <string.h>

///// Namespaces /////

namespace utility
{

///// Methods /////

PrivateKey::PrivateKey() :
  key_(nullptr),
  pkey_(nullptr)
{
  
}

PrivateKey::PrivateKey(const PrivateKey& lhs) :
  key_(RSAPrivateKey_dup(lhs.key_)),
  pkey_(nullptr)
{
  
}

PrivateKey::~PrivateKey()
{
  Destroy();

}

int PrivateKey::Init()
{
  Destroy();
  
  BIGNUM* bn = BN_new();
  if (BN_set_word(bn, RSA_F4) != 1)
  {
    BN_free(bn);
    return 1;
  }

  key_ = RSA_new();
  if (!key_)
  {
    BN_free(bn);
    return 1;
  }

  if (RSA_generate_key_ex(key_, 2048, bn, NULL) != 1)
  {
    BN_free(bn);
    RSA_free(key_);
    return 1;
  }

  BN_free(bn);

  return 0;
}

int PrivateKey::Init(const boost::filesystem::path& filename, const std::string& password)
{
  Destroy();

  std::ifstream file(filename.c_str());
  if (!file.is_open())
  {
  
    return 1;
  }

  return InitFromData(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()), password);
}

int PrivateKey::InitFromData(const std::string& privatekey, const std::string& password)
{
  BIO* bio = BIO_new_mem_buf((void*)privatekey.data(), static_cast<int>(privatekey.size()));
  BOOST_SCOPE_EXIT(&bio)
  {
    BIO_free(bio);

  } BOOST_SCOPE_EXIT_END

  key_ = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, const_cast<char*>(password.data()));
  if (!key_)
  {

    return 1;
  }

  return 0;
}

void PrivateKey::Destroy()
{
  if (pkey_)
  {
    EVP_PKEY_free(pkey_);
    pkey_ = nullptr;
  }

  if (key_)
  {
    RSA_free(key_);
    key_ = nullptr;
  }
}

std::string PrivateKey::GetString() const
{
  if (pkey_ == nullptr)
  {
    pkey_ = EVP_PKEY_new();
    if (!pkey_)
    {

      return std::string();
    }
  }

  if (EVP_PKEY_set1_RSA(pkey_, key_) != 1)
  {
    EVP_PKEY_free(pkey_);
    return std::string();
  }
  
  std::unique_ptr<BIO, decltype(&::BIO_free)> bio(BIO_new(BIO_s_mem()), ::BIO_free);
  if (!bio)
  {

    return std::string();
  }

  if (PEM_write_bio_PrivateKey(bio.get(), pkey_, NULL, NULL, 0, NULL, NULL) != 1)
  {

    return std::string();
  }
  const int length = BIO_pending(bio.get()) + 1;
  std::unique_ptr<char[]> privatekey = std::make_unique<char[]>(length);
  std::memset(privatekey.get(), '\0', length);

  char* data = privatekey.get();
  BIO_get_mem_data(bio.get(), &data);
  return std::string(data, length - 1);
}

int PrivateKey::Save(const std::string& filename)
{
  if (pkey_ == nullptr)
  {
    pkey_ = EVP_PKEY_new();
    if (!pkey_)
    {

      return 1;
    }
  }

  if (EVP_PKEY_set1_RSA(pkey_, key_) != 1)
  {
    EVP_PKEY_free(pkey_);
    return 2;
  }

  std::unique_ptr<BIO, decltype(&::BIO_free)> bio(BIO_new_file(filename.c_str(), "w"), ::BIO_free);
  if (!bio)
  {

    return 3;
  }

  if (PEM_write_bio_PrivateKey(bio.get(), pkey_, NULL, NULL, 0, NULL, NULL) != 1)
  {

    return 4;
  }

  return 0;
}

boost::optional< std::array<unsigned char, 256> >PrivateKey::Sign(const unsigned char* data, const size_t size) const
{
  if (!key_)
  {

    return boost::none;
  }

  if (pkey_ == nullptr)
  {
    pkey_ = EVP_PKEY_new();
    if (!pkey_)
    {

      return boost::none;
    }
  }

  if (EVP_PKEY_set1_RSA(pkey_, key_) != 1)
  {
    EVP_PKEY_free(pkey_);
    return boost::none;
  }

  EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
  if (!mdctx)
  {

    return boost::none;
  }

  if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey_) != 1)
  {
    EVP_MD_CTX_destroy(mdctx);
    return boost::none;
  }

  if (EVP_DigestSignUpdate(mdctx, data, size) != 1)
  {
    EVP_MD_CTX_destroy(mdctx);
    return boost::none;
  }

  size_t slen = 0;
  if (EVP_DigestSignFinal(mdctx, nullptr, &slen) != 1)
  {
    EVP_MD_CTX_destroy(mdctx);
    return boost::none;
  }

  unsigned char* sig = static_cast<unsigned char*>(OPENSSL_malloc(static_cast<int>(sizeof(unsigned char)*slen)));
  if (EVP_DigestSignFinal(mdctx, sig, &slen) != 1)
  {
    OPENSSL_free(sig);
    EVP_MD_CTX_destroy(mdctx);
    return boost::none;
  }

  if (slen != 256)
  {

    return boost::none;
  }

  std::array<unsigned char, 256> signature;
  memcpy(signature.data(), sig, 256);

  OPENSSL_free(sig);
  EVP_MD_CTX_destroy(mdctx);

  return signature;
}

int PrivateKey::GetSize() const
{
  if (key_ == nullptr)
  {

    return 0;
  }
  return RSA_size(key_);
}

PrivateKey& PrivateKey::operator=(const PrivateKey& lhs)
{
  key_ = RSAPrivateKey_dup(lhs.key_);
  pkey_ = nullptr;
  return *this;
}

}
