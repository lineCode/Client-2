// nonce.hpp
//

#ifndef IDXBHL1H7GIQEE3D1Q03B4E70ZB3DR3VTJ
#define IDXBHL1H7GIQEE3D1Q03B4E70ZB3DR3VTJ

///// Includes /////

#include <chrono>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Classes /////

class Nonce
{
 public:

  Nonce();
  Nonce(const std::string& nonce, const std::chrono::steady_clock::time_point& time);
  ~Nonce();

  const std::string& GetNonce() const { return nonce_; }
  const std::chrono::steady_clock::time_point& GetTime() const { return time_; }

  bool IsEmpty() const { return nonce_.empty(); }

  bool operator==(const std::string& nonce) const { return (nonce_ == nonce); };

 private:

  std::string nonce_;
  std::chrono::steady_clock::time_point time_;

};

}

}

#endif
