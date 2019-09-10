// nonce.cpp
//

///// Includes /////

#include "onvifserver/nonce.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Methods /////

Nonce::Nonce()
{

}

Nonce::Nonce(const std::string& nonce, const std::chrono::steady_clock::time_point& time) :
  nonce_(nonce),
  time_(time)
{

}

Nonce::~Nonce()
{

}

}

}
