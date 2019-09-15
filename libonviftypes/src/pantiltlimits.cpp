// pantiltlimits.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PanTiltLimits::PanTiltLimits()
{

}

PanTiltLimits::PanTiltLimits(const boost::optional<Space2dDescription>& range) :
  range_(range)
{

}

PanTiltLimits::PanTiltLimits(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Range")
    {
      range_ = Space2dDescription(element);

    }
  }
}

std::string PanTiltLimits::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Range", range_) + "</"+name+">");
}

bool PanTiltLimits::operator==(const PanTiltLimits& rhs) const
{
  return (range_ == rhs.range_);
}

}
