// widedynamicrange20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

WideDynamicRange20::WideDynamicRange20()
{

}

WideDynamicRange20::WideDynamicRange20(const boost::optional<WIDEDYNAMICMODE>& mode, const boost::optional<float>& level) :
  mode_(mode),
  level_(level)
{

}

WideDynamicRange20::WideDynamicRange20(const pugi::xml_node& node)
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

std::string WideDynamicRange20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool WideDynamicRange20::operator==(const WideDynamicRange20& rhs) const
{
  return ((mode_ == rhs.mode_) && (level_ == rhs.level_));
}

}
