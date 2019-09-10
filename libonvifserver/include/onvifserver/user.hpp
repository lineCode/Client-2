// user.hpp
//

#ifndef IDEMEPO6XHSQ5J7L4JEVKHTRP24Q7Z0PVG
#define IDEMEPO6XHSQ5J7L4JEVKHTRP24Q7Z0PVG

///// Includes /////

#include <chrono>
#include <string>
#include <vector>

#include "nonce.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Classes /////

class User
{
 public:

  User();
  User(const std::string& address, const Nonce& nonce, const std::chrono::steady_clock::time_point& lastrequesttime);
  ~User();

  const std::string& GetAddress() const { return address_; }

  void SetUsername(const std::string& username) { username_ = username; }
  const std::string& GetUsername() const { return username_; }

  void SetNonce(const Nonce& nonce) { nonce_ = nonce; }
  const Nonce& GetNonce() const { return nonce_; }

  std::vector<Nonce>& GetPrevNonces() { return prevnonces_; }
  const std::vector<Nonce>& GetPrevNonces() const { return prevnonces_; }

  void SetNc(unsigned int nc) { nc_ = nc; }
  unsigned int GetNc() const { return nc_; }

  const std::chrono::steady_clock::time_point& GetLastRequestTime() const { return lastrequesttime_; }
  void SetLastRequestTime(const std::chrono::steady_clock::time_point& lastrequesttime) { lastrequesttime_ = lastrequesttime; }

 private:

  std::string address_;
  std::string username_;
  Nonce nonce_;
  std::vector<Nonce> prevnonces_;
  unsigned int nc_;
  std::chrono::steady_clock::time_point lastrequesttime_;

};

}

}

#endif
