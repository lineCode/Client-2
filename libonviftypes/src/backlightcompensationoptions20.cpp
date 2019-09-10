// backlightcompensationoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

BacklightCompensationOptions20::BacklightCompensationOptions20()
{

}

BacklightCompensationOptions20::BacklightCompensationOptions20(const std::vector<BACKLIGHTCOMPENSATIONMODE>& mode, const boost::optional<FloatRange>& level) :
  mode_(mode),
  level_(level)
{

}

BacklightCompensationOptions20::BacklightCompensationOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<BACKLIGHTCOMPENSATIONMODE> mode = GetBacklightCompensationMode(element.text().get());
      if (mode.is_initialized())
      {
        mode_.push_back(*mode);

      }
    }
    else if (name == "Level")
    {
      level_ = FloatRange(element);

    }
  }
}

std::string BacklightCompensationOptions20::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXmlClass("Level", level_) + "</"+name+">");
}

bool BacklightCompensationOptions20::operator==(const BacklightCompensationOptions20& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && (level_ == rhs.level_));
}

}
