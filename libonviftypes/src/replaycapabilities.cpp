// replaycapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ReplayCapabilities::ReplayCapabilities()
{
  
}

ReplayCapabilities::ReplayCapabilities(const boost::optional<std::string>& xaddr) :
  xaddr_(xaddr)
{
  
}

ReplayCapabilities::ReplayCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
  }
}

std::string ReplayCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + "</"+name+">");
}

bool ReplayCapabilities::operator==(const ReplayCapabilities& rhs) const
{
  return (xaddr_ == rhs.xaddr_);
}

}
