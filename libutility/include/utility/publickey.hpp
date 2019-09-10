// publickey.hpp
//

#ifndef ID4RAUEIWRQA2ZWXB2WFKQVPJR70DU2BBY
#define ID4RAUEIWRQA2ZWXB2WFKQVPJR70DU2BBY

///// Includes /////

#include <boost/filesystem/path.hpp>
#include <openssl/rsa.h>
#include <string>
#include <vector>

#include "privatekey.hpp"

///// Namespaces /////

namespace utility
{

///// Classes /////

class PublicKey 
{
 public:

  PublicKey();
  PublicKey(const PublicKey& rhs);
  ~PublicKey();

  int Init(const std::vector<char>& data);
  int Init(const std::string& text);
  int Init(const boost::filesystem::path& path);
  int Init(const PrivateKey& privatekey); // Generate public key from private key
  void Destroy();

  std::string GetString() const;

  bool Verify(const unsigned char* message, const size_t size, const unsigned char* signature) const; // Signature must be 256 bytes long

  RSA* GetKey() const { return key_; }

  PublicKey& operator=(const PublicKey&) = delete;

 private:

  RSA* key_;

};

}

#endif
