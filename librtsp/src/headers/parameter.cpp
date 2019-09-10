// parameter.cpp
//

///// Includes /////

#include "rtsp/headers/parameter.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

Parameter::Parameter()
{

}

Parameter::Parameter(const std::string& parameter)
{
  static const boost::regex regex("[\\s]*([\\w]+)[\\s]*(?::[\\s]*([\\w]+)[\\w]*)?");
  boost::smatch match;
  if (!boost::regex_search(parameter, match, regex))
  {

    return;
  }

  key_ = match[1];
  value_ = match[2];
}

Parameter::Parameter(const std::string& key, const std::string& value) :
  key_(key),
  value_(value)
{

}

Parameter::~Parameter()
{

}

bool Parameter::IsValid() const
{
  return (!key_.empty());
}

std::string Parameter::ToString() const
{
  if (value_.size())
  {

    return key_ + std::string(": ") + value_;
  }
  else
  {

    return key_;
  }
}

bool Parameter::operator==(const Parameter& rhs) const
{
  return ((key_ == rhs.key_) && (value_ == rhs.value_));
}

}

}
