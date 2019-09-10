// header.hpp
//

#ifndef IDXZUDY8W60K4ONPCEP8ZAYPXFTPXDO5BF
#define IDXZUDY8W60K4ONPCEP8ZAYPXFTPXDO5BF

///// Includes /////

#include <string>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Classes /////

class Header
{
 public:

  static Header WWWAuthenticate(const std::string& realm, const std::string& nonce, bool stale);

  Header();
  Header(const std::string& key, const std::string& value);
  virtual ~Header();

  virtual std::string GetKey() const { return key_; }
  virtual std::string GetValue() const { return value_; }

 private:

  std::string key_;
  std::string value_;

};

}

}

#endif
