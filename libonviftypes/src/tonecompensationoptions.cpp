// tonecompensationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ToneCompensationOptions::ToneCompensationOptions()
{

}

ToneCompensationOptions::ToneCompensationOptions(const std::vector<TONECOMPENSATIONMODE>& mode, const boost::optional<bool>& level) :
  mode_(mode),
  level_(level)
{

}

ToneCompensationOptions::ToneCompensationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<TONECOMPENSATIONMODE> mode = GetToneCompensationMode(element.text().get());
      if (mode.is_initialized())
      {
        mode_.push_back(*mode);

      }
    }
    else if (name == "Level")
    {
      level_ = element.text().as_bool();

    }
  }
}

std::string ToneCompensationOptions::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool ToneCompensationOptions::operator==(const ToneCompensationOptions& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && (level_ == rhs.level_));
}

}
