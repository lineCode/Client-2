// backlightcompensation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

BacklightCompensation::BacklightCompensation()
{

}

BacklightCompensation::BacklightCompensation(const boost::optional<BACKLIGHTCOMPENSATIONMODE>& mode, const boost::optional<float>& level) :
  mode_(mode),
  level_(level)
{

}

BacklightCompensation::BacklightCompensation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetBacklightCompensationMode(element.text().get());

    }
    else if (name == "Level")
    {
      level_ = element.text().as_float();

    }
  }
}

std::string BacklightCompensation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool BacklightCompensation::operator==(const BacklightCompensation& rhs) const
{
  return (mode_ == rhs.mode_) && (level_ == rhs.level_);
}

}
