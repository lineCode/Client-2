// license.hpp
//

///// Includes /////

#include "license/license.hpp"

#include <regex>

///// Namespaces /////

namespace license
{

///// Methods /////

License::License() :
  value_(0)
{

}

License::License(const std::string& license, unsigned int value) :
  license_(license),
  value_(value)
{

}

License::~License()
{

}

int License::Init(const std::string& license)
{
  static std::regex regex("([a-zA-Z0-9_]+):([\\d]+)");
  std::smatch match;
  if (!std::regex_match(license, match, regex))
  {

    return 1;
  }

  license_ = match[1].str();
  value_ = std::strtoul(match[2].str().c_str(), nullptr, 10);

  return 0;
}

std::string License::ToString() const
{
  return license_ + std::string(":") + std::to_string(value_);
}

bool License::operator==(const License& rhs) const
{
  return ((license_ == rhs.license_) && (value_ == rhs.value_));
}

}
