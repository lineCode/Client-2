// onvifversion.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

OnvifVersion::OnvifVersion()
{

}

OnvifVersion::OnvifVersion(int major, int minor) :
  major_(major), minor_(minor)
{

}

OnvifVersion::OnvifVersion(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Major")
    {
      major_ = element.text().as_int();

    }
    else if (name == "Minor")
    {
      minor_ = element.text().as_int();

    }
  }
}

utility::Version OnvifVersion::GetVersion() const
{
  const int major = major_.is_initialized() ? *major_ : 0;
  const int minor = minor_.is_initialized() ? *minor_ : 0;

  // Grab the top two digits of minor because this is what the onvif standard says...
  const std::vector<int> split = utility::SplitDigits(minor);
  if (split.size() == 1)
  {
  
    return utility::Version(major, 0, split.front());
  }
  else
  {
    const int minortop = (split.size() >= 1) ? (*split.begin()) : 0;
    const int minorbot = (split.size() >= 2) ? (*(split.begin()+1)) : 0;
    return utility::Version(major, minortop, minorbot);
  }
}

std::string OnvifVersion::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Major", major_) + onvif::ToXml("tt:Minor", minor_) + "</"+name+">");
}

bool OnvifVersion::operator==(const OnvifVersion& rhs) const
{
  return ((major_ == rhs.major_) && (minor_ == rhs.minor_));
}

}
