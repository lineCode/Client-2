// displaycapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DisplayCapabilities::DisplayCapabilities()
{

}

DisplayCapabilities::DisplayCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& fixedlayout) :
  xaddr_(xaddr),
  fixedlayout_(fixedlayout)
{

}

DisplayCapabilities::DisplayCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "FixedLayout")
    {
      fixedlayout_ = element.text().as_bool();

    }
  }
}

std::string DisplayCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:FixedLayout", fixedlayout_) + "</"+name+">");
}

bool DisplayCapabilities::operator==(const DisplayCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (fixedlayout_ == rhs.fixedlayout_));
}

}
