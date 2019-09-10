// tonecompensation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ToneCompensation::ToneCompensation()
{

}

ToneCompensation::ToneCompensation(const boost::optional<TONECOMPENSATIONMODE>& mode, const boost::optional<float>& level) :
  mode_(mode),
  level_(level)
{

}

ToneCompensation::ToneCompensation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetToneCompensationMode(element.text().get());

    }
    else if (name == "Level")
    {
      level_ = element.text().as_float();

    }
  }
}

std::string ToneCompensation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool ToneCompensation::operator==(const ToneCompensation& rhs) const
{
  return ((mode_ == rhs.mode_) && (level_ == rhs.level_));
}

}
