// widedynamicrange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

WideDynamicRange::WideDynamicRange()
{

}

WideDynamicRange::WideDynamicRange(const boost::optional<WIDEDYNAMICMODE>& mode, const boost::optional<float>& level) :
  mode_(mode),
  level_(level)
{

}

WideDynamicRange::WideDynamicRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetWideDynamicMode(element.text().get());

    }
    else if (name == "Level")
    {
      level_ = element.text().as_float();

    }
  }
}

std::string WideDynamicRange::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool WideDynamicRange::operator==(const WideDynamicRange& rhs) const
{
  return ((mode_ == rhs.mode_) && (level_ == rhs.level_));
}

}
