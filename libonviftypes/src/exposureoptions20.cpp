// exposureoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ExposureOptions20::ExposureOptions20()
{

}

ExposureOptions20::ExposureOptions20(const std::vector<EXPOSUREMODE>& mode, const std::vector<EXPOSUREPRIORITY>& priority, const boost::optional<FloatRange>& minexposuretime, const boost::optional<FloatRange>& maxexposuretime, const boost::optional<FloatRange>& mingain, const boost::optional<FloatRange>& maxgain, const boost::optional<FloatRange>& miniris, const boost::optional<FloatRange>& maxiris, const boost::optional<FloatRange>& exposuretime, const boost::optional<FloatRange>& gain, const boost::optional<FloatRange>& iris) :
  mode_(mode),
  priority_(priority),
  minexposuretime_(minexposuretime),
  maxexposuretime_(maxexposuretime),
  mingain_(mingain),
  maxgain_(maxgain),
  miniris_(miniris),
  maxiris_(maxiris),
  exposuretime_(exposuretime),
  gain_(gain),
  iris_(iris)
{

}

ExposureOptions20::ExposureOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<EXPOSUREMODE> mode = GetExposureMode(element.text().get());
      if (mode.is_initialized())
      {
        mode_.push_back(*mode);

      }
    }
    else if (name == "Priority")
    {
      boost::optional<EXPOSUREPRIORITY> priority = GetExposurePriority(element.text().get());
      if (priority.is_initialized())
      {
        priority_.push_back(*priority);

      }
    }
    else if (name == "MinExposureTime")
    {
      minexposuretime_ = FloatRange(element);

    }
    else if (name == "MaxExposureTime")
    {
      maxexposuretime_ = FloatRange(element);

    }
    else if (name == "MinGain")
    {
      mingain_ = FloatRange(element);

    }
    else if (name == "MaxGain")
    {
      maxgain_ = FloatRange(element);

    }
    else if (name == "MinIris")
    {
      miniris_ = FloatRange(element);

    }
    else if (name == "MaxIris")
    {
      maxiris_ = FloatRange(element);

    }
    else if (name == "ExposureTime")
    {
      exposuretime_ = FloatRange(element);

    }
    else if (name == "Gain")
    {
      gain_ = FloatRange(element);

    }
    else if (name == "Iris")
    {
      iris_ = FloatRange(element);

    }
  }
}

std::string ExposureOptions20::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  std::string priorities;
  for (const auto& priority : priority_)
  {
    priorities += onvif::ToXmlEnum("tt:Priority", priority);

  }
  
  return ("<"+name+">" + modes + priorities + onvif::ToXmlClass("tt:MinExposureTime", minexposuretime_) + onvif::ToXmlClass("tt:MaxExposureTime", maxexposuretime_) + onvif::ToXmlClass("tt:MinGain", mingain_) + onvif::ToXmlClass("tt:MaxGain", maxgain_) + onvif::ToXmlClass("tt:MinIris", miniris_) + onvif::ToXmlClass("tt:MaxIris", maxiris_) + onvif::ToXmlClass("tt:ExposureTime", exposuretime_) + onvif::ToXmlClass("tt:Gain", gain_) + onvif::ToXmlClass("tt:Iris", iris_) + "</"+name+">");
}

bool ExposureOptions20::operator==(const ExposureOptions20& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && std::is_permutation(priority_.begin(), priority_.end(), rhs.priority_.begin(), rhs.priority_.end()) && (minexposuretime_ == rhs.minexposuretime_) && (maxexposuretime_ == rhs.maxexposuretime_) && (mingain_ == rhs.mingain_) && (maxgain_ == rhs.maxgain_) && (miniris_ == rhs.miniris_) && (maxiris_ == rhs.maxiris_) && (exposuretime_ == rhs.exposuretime_) && (gain_ == rhs.gain_) && (iris_ == rhs.iris_));
}

}
