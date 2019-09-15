// exposure20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Exposure20::Exposure20()
{

}

Exposure20::Exposure20(const boost::optional<EXPOSUREMODE>& mode, const boost::optional<EXPOSUREPRIORITY>& priority, const boost::optional<Rectangle>& window, const boost::optional<float>& minexposuretime, const boost::optional<float>& maxexposuretime, const boost::optional<float>& mingain, const boost::optional<float>& maxgain, const boost::optional<float>& miniris, const boost::optional<float>& maxiris, const boost::optional<float>& exposuretime, const boost::optional<float>& gain, const boost::optional<float>& iris) :
  mode_(mode),
  priority_(priority),
  window_(window),
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

Exposure20::Exposure20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetExposureMode(element.text().get());

    }
    else if (name == "Priority")
    {
      priority_ = GetExposurePriority(element.text().get());

    }
    else if (name == "Window")
    {
      window_ = Rectangle(element);

    }
    else if (name == "MinExposureTime")
    {
      minexposuretime_ = element.text().as_float();

    }
    else if (name == "MaxExposureTime")
    {
      maxexposuretime_ = element.text().as_float();

    }
    else if (name == "MinGain")
    {
      mingain_ = element.text().as_float();

    }
    else if (name == "MaxGain")
    {
      maxgain_ = element.text().as_float();

    }
    else if (name == "MinIris")
    {
      miniris_ = element.text().as_float();

    }
    else if (name == "MaxIris")
    {
      maxiris_ = element.text().as_float();

    }
    else if (name == "ExposureTime")
    {
      exposuretime_ = element.text().as_float();

    }
    else if (name == "Gain")
    {
      gain_ = element.text().as_float();

    }
    else if (name == "Iris")
    {
      iris_ = element.text().as_float();

    }
  }
}

std::string Exposure20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXmlEnum("tt:Priority", priority_) + onvif::ToXmlClass("tt:Window", window_) + onvif::ToXml("tt:MinExposureTime", minexposuretime_) + onvif::ToXml("tt:MaxExposureTime", maxexposuretime_) + onvif::ToXml("tt:MinGain", mingain_) + onvif::ToXml("tt:MaxGain", maxgain_) + onvif::ToXml("tt:MinIris", miniris_) + onvif::ToXml("tt:MaxIris", maxiris_) + onvif::ToXml("tt:ExposureTime", exposuretime_) + onvif::ToXml("tt:Gain", gain_) + onvif::ToXml("tt:Iris", iris_) + "</"+name+">");
}

bool Exposure20::operator==(const Exposure20& rhs) const
{
  return ((mode_ == rhs.mode_) && (priority_ == rhs.priority_) && (window_ == rhs.window_) && (minexposuretime_ == rhs.minexposuretime_) && (maxexposuretime_ == rhs.maxexposuretime_) && (mingain_ == rhs.mingain_) && (maxgain_ == rhs.maxgain_) && (miniris_ == rhs.miniris_) && (maxiris_ == rhs.maxiris_) && (exposuretime_ == rhs.exposuretime_) && (gain_ == rhs.gain_) && (iris_ == rhs.iris_));
}

}
