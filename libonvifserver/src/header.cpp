// header.cpp
//

///// Includes /////

#include "onvifserver/header.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Methods /////

Header Header::WWWAuthenticate(const std::string& realm, const std::string& nonce, bool stale)
{
  return Header(std::string("WWW-Authenticate"), std::string("Digest realm=\"") + realm + std::string("\", qop=\"auth\", nonce=\"") + nonce + std::string("\", stale=") + (stale ? std::string("TRUE") : std::string("FALSE")));
}

Header::Header()
{

}

Header::Header(const std::string& key, const std::string& value) :
  key_(key),
  value_(value)
{

}

Header::~Header()
{

}

}

}
