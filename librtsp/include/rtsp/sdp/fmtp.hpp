// fmtp.hpp
//

#ifndef ID2A1SDWNCLITJOD73INI5NM1UUH2PPLZV
#define ID2A1SDWNCLITJOD73INI5NM1UUH2PPLZV

///// Includes /////

#include <map>
#include <string>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Structures /////

struct FMTPPARAMETER
{
  FMTPPARAMETER()
  {

  }

  FMTPPARAMETER(const std::string& key, const std::string& value) :
    key_(key),
    value_(value)
  {

  }

  bool IsValid() const
  {
    return (!key_.empty() && !value_.empty());
  }

  std::string ToString() const
  {
    if (!IsValid())
    {

      return std::string();
    }

    return (key_ + std::string("=") + value_);
  }

  bool operator==(const FMTPPARAMETER& rhs) const
  {
    return ((key_ == rhs.key_) && (value_ == rhs.value_));
  }

  std::string key_;
  std::string value_;

};

///// Classes /////

class Fmtp
{
 public:

  Fmtp();
  Fmtp(const std::map< int, std::vector<FMTPPARAMETER> >& fmtps);
  Fmtp(const std::vector<std::string>& fmtps);
  ~Fmtp();

  void AddFmtp(const std::string& fmtp);

  void Clear();
  bool IsValid() const;
  std::vector<std::string> ToString() const;

  bool operator==(const Fmtp& rhs) const;

  std::map< int, std::vector<FMTPPARAMETER> > fmtps_;

};

}

}

#endif
