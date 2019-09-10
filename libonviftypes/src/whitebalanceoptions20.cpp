// whitebalanceoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

WhiteBalanceOptions20::WhiteBalanceOptions20()
{

}

WhiteBalanceOptions20::WhiteBalanceOptions20(const std::vector<WHITEBALANCEMODE>& mode, const boost::optional<FloatRange>& yrgain, const boost::optional<FloatRange>& ybgain) :
  mode_(mode),
  yrgain_(yrgain),
  ybgain_(ybgain)
{

}

WhiteBalanceOptions20::WhiteBalanceOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<WHITEBALANCEMODE> mode = GetWhiteBalanceMode(element.text().get());
      if (mode.is_initialized())
      {
        mode_.push_back(*mode);

      }
    }
    else if (name == "YrGain")
    {
      yrgain_ = FloatRange(element);

    }
    else if (name == "YbGain")
    {
      ybgain_ = FloatRange(element);

    }
  }
}

std::string WhiteBalanceOptions20::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXmlClass("tt:YrGain", yrgain_) + onvif::ToXmlClass("tt:YbGain", ybgain_) + "</"+name+">");
}

bool WhiteBalanceOptions20::operator==(const WhiteBalanceOptions20& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && (yrgain_ == rhs.yrgain_) && (ybgain_ == rhs.ybgain_));
}

}
