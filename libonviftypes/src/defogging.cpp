// defogging.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Defogging::Defogging()
{

}

Defogging::Defogging(const boost::optional<DEFOGGINGMODE>& mode, const boost::optional<float>& level) :
  mode_(mode),
  level_(level)
{

}

Defogging::Defogging(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetDefoggingMode(element.text().get());

    }
    else if (name == "Level")
    {
      level_ = element.text().as_float();

    }
  }
}

std::string Defogging::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool Defogging::operator==(const Defogging& rhs) const
{
  return ((mode_ == rhs.mode_) && (level_ == rhs.level_));
}

}
