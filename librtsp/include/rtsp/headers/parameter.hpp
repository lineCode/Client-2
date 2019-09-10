// parameter.hpp
//

#ifndef IDHWUVLMXYLJULNAP4E2TJD7EIBTAAWOFB
#define IDHWUVLMXYLJULNAP4E2TJD7EIBTAAWOFB

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

class Parameter
{
public:

  Parameter();
  Parameter(const std::string& parameter);
  Parameter(const std::string& key, const std::string& value);
  ~Parameter();

  bool IsValid() const;

  std::string ToString() const;

  bool operator==(const Parameter& rhs) const;

  std::string key_;
  std::string value_;

};

}

}

#endif
