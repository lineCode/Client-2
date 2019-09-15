// ptzcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZCapabilities::PTZCapabilities()
{

}

PTZCapabilities::PTZCapabilities(const boost::optional<std::string>& xaddr) :
  xaddr_(xaddr)
{

}

PTZCapabilities::PTZCapabilities(const pugi::xml_node& node)
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

std::string PTZCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + "</"+name+">");
}

bool PTZCapabilities::operator==(const PTZCapabilities& rhs) const
{
  return (xaddr_ == rhs.xaddr_);
}

}
