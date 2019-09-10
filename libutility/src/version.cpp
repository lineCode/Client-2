// version.cpp
//

///// Includes /////

#include "utility/version.hpp"

///// Namespaces /////

namespace utility
{

///// Methods /////

Version::Version() :
  major_(0),
  minor_(0),
  build_(0)
{

}

Version::Version(unsigned int major, unsigned int minor, unsigned int build) :
  major_(major),
  minor_(minor),
  build_(build)
{

}

Version::~Version()
{

}

std::string Version::ToString() const
{
  return (std::to_string(major_) + '.' + std::to_string(minor_) + '.' + std::to_string(build_));
}

bool Version::operator==(const Version& rhs) const
{
  return ((major_ == rhs.major_) && (minor_ == rhs.minor_) && (build_ == rhs.build_));
}

bool Version::operator!=(const Version& rhs) const
{
  return !operator==(rhs);
}

Version& Version::operator=(const Version& rhs)
{
  major_ = rhs.major_;
  minor_ = rhs.minor_;
  build_ = rhs.build_;
  return *this;
}

bool Version::operator>(const Version& rhs) const
{
  if (major_ == rhs.major_)
  {
    if (minor_ == rhs.minor_)
    {
    
      return (build_ > rhs.build_);
    }
    else
    {
    
      return (minor_ > rhs.minor_);
    }
  }
  else
  {

    return (major_ > rhs.major_);
  }
}

bool Version::operator<(const Version& rhs) const
{
  if (major_ == rhs.major_)
  {
    if (minor_ == rhs.minor_)
    {
    
      return (build_ < rhs.build_);
    }
    else
    {
    
      return (minor_ < rhs.minor_);
    }
  }
  else
  {

    return (major_ < rhs.major_);
  }
}

bool Version::operator>=(const Version& rhs) const
{
  return ((*this == rhs) || (*this > rhs));
}

bool Version::operator<=(const Version& rhs) const
{
  return ((*this == rhs) || (*this < rhs));
}

}
