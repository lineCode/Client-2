// privatekey.hpp
//

#ifndef IDZGA2V0KI3OOQ58ILLDXFJTL4FGAFP2B9
#define IDZGA2V0KI3OOQ58ILLDXFJTL4FGAFP2B9

///// Includes /////

#include <array>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <openssl/rsa.h>
#include <string>
#include <vector>

///// Namespaces /////

namespace utility
{

///// Classes /////

class PrivateKey
{
 public:

  PrivateKey();
  PrivateKey(const PrivateKey& lhs);
  ~PrivateKey();

  int Init(); // Generates a new private key
  int Init(const boost::filesystem::path& filename, const std::string& password);
  int InitFromData(const std::string& privatekey, const std::string& password);
  void Destroy();

  std::string GetString() const; // Returns an empty string upon failed
  int Save(const std::string& filename);

  boost::optional< std::array<unsigned char, 256> > Sign(const unsigned char* data, const size_t size) const;

  RSA* GetKey() const { return key_; }

  int GetSize() const;

  PrivateKey& operator=(const PrivateKey& lhs);

 private:

  RSA* key_;
  mutable EVP_PKEY* pkey_; // Lazily initialised for signing

};

}

#endif
