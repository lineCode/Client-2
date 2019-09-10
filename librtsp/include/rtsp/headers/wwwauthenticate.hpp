// wwwauthenticate.hpp
//

#ifndef IDL84KX63FH44Y8JKS3BE4EGP3G8X771XR
#define IDL84KX63FH44Y8JKS3BE4EGP3G8X771XR

///// Includes /////

#include <boost/optional.hpp>
#include <string>

#include "rtsp/headers/authenticationtype.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

class WWWAuthenticate
{
 public:

  WWWAuthenticate();
  WWWAuthenticate(const std::string& text);
  WWWAuthenticate(AUTHENTICATIONTYPE authenticationtype, const std::string& realm); // For BASIC
  WWWAuthenticate(AUTHENTICATIONTYPE authenticationtype, const std::string& realm, const std::string& nonce, const boost::optional<bool>& stale); // For DIGEST
  ~WWWAuthenticate();

  bool IsValid() const;

  std::string ToString() const;

  bool operator==(const WWWAuthenticate& rhs) const;

  AUTHENTICATIONTYPE authenticationtype_;
  std::string realm_;
  std::string nonce_;
  boost::optional<bool> stale_;

};

}

}

#endif
