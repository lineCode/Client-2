// user.cpp
//

///// Includes /////

#include "onvifserver/user.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Methods /////

User::User()
{

}

User::User(const std::string& address, const Nonce& nonce, const std::chrono::steady_clock::time_point& lastrequesttime) :
  address_(address),
  nonce_(nonce),
  nc_(0),
  lastrequesttime_(lastrequesttime)
{

}

User::~User()
{

}

}

}
