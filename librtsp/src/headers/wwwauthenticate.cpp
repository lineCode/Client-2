// wwwauthenticate.cpp
//

///// Includes /////

#include "rtsp/headers/wwwauthenticate.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

WWWAuthenticate::WWWAuthenticate() :
  authenticationtype_(AUTHENTICATIONTYPE_INVALID)
{

}

WWWAuthenticate::WWWAuthenticate(const std::string& text) :
  authenticationtype_(AUTHENTICATIONTYPE_INVALID)
{
  static const boost::regex regex("[\\s]*WWW-Authenticate[\\s]*:[\\s]*([\\w]+)[\\s]*(.*)");
  boost::smatch match;
  if (!boost::regex_search(text, match, regex))
  {

    return;
  }

  authenticationtype_ = AuthenticationTypeFromString(match[1]);

  // Flags
  std::string flags = match[2].str();
  static const boost::regex flagsregex("[\\s]*[\\w]+[\\s]*=[\\s]*\\\"?[\\w \\-\\(\\)']+\\\"?,?");
  for (boost::sregex_token_iterator i(flags.begin(), flags.end(), flagsregex); i != boost::sregex_token_iterator(); ++i)
  {
    std::string flag = i->str();
    boost::smatch match;
    static const boost::regex flagregex("[\\s]*([\\w]+)[\\s]*=[\\s]*\\\"?([\\w \\-'\\(\\)]+)\\\"?,?[\\s]*");
    if (!boost::regex_search(flag, match, flagregex))
    {

      continue;
    }

    std::string key = match[1];
    std::string value = match[2];

    if (boost::iequals(key, "realm"))
    {
      realm_ = value;

    }
    else if (boost::iequals(key, "nonce"))
    {
      nonce_ = value;

    }
    else if (boost::iequals(key, "stale"))
    {
      stale_ = utility::StringToBool(value);

    }
  }
}

WWWAuthenticate::WWWAuthenticate(AUTHENTICATIONTYPE authenticationtype, const std::string& realm) :
  authenticationtype_(authenticationtype),
  realm_(realm)
{

}

WWWAuthenticate::WWWAuthenticate(AUTHENTICATIONTYPE authenticationtype, const std::string& realm, const std::string& nonce, const boost::optional<bool>& stale) :
  authenticationtype_(authenticationtype),
  realm_(realm),
  nonce_(nonce),
  stale_(stale)
{

}

WWWAuthenticate::~WWWAuthenticate()
{

}

bool WWWAuthenticate::IsValid() const
{
  switch (authenticationtype_)
  {
    case AUTHENTICATIONTYPE_BASIC:
    {

      return !realm_.empty();
    }
    case AUTHENTICATIONTYPE_DIGEST:
    {
      if (realm_.empty() || nonce_.empty())
      {

        return false;
      }
      else
      {

        return true;
      }
    }
    default:
    {

      return false;
    }
  }
}

std::string WWWAuthenticate::ToString() const
{
  switch (authenticationtype_)
  {
    case AUTHENTICATIONTYPE_BASIC:
    {

      return std::string("WWW-Authenticate: Basic realm=\"") + realm_ + std::string("\"\r\n");
    }
    case AUTHENTICATIONTYPE_DIGEST:
    {
      std::string stale;
      if (stale_.is_initialized())
      {
        stale = std::string(", stale=") + (*stale_ ? std::string("TRUE") : std::string("FALSE"));

      }

      return std::string("WWW-Authenticate: Digest realm=\"") + realm_ + std::string("\", nonce=\"") + nonce_ + std::string("\"") + stale + std::string("\r\n");
    }
    default:
    {

      return std::string();
    }
  }
}

bool WWWAuthenticate::operator==(const WWWAuthenticate& rhs) const
{
  return ((authenticationtype_ == rhs.authenticationtype_) && (realm_ == rhs.realm_) && (nonce_ == rhs.nonce_) && (stale_ == rhs.stale_));
}

}

}
