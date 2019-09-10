// imagingsettings.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImagingSettings::ImagingSettings()
{

}

ImagingSettings::ImagingSettings(const boost::optional<BacklightCompensation>& backlightcompensation, const boost::optional<float>& brightness, const boost::optional<float>& colorsaturation, const boost::optional<float>& contrast, const boost::optional<Exposure>& exposure, const boost::optional<FocusConfiguration>& focus, const boost::optional<IRCUTFILTERMODE>& ircutfilter, const boost::optional<float>& sharpness, const boost::optional<WideDynamicRange>& widedynamicrange, const boost::optional<WhiteBalance>& whitebalance) :
  backlightcompensation_(backlightcompensation),
  brightness_(brightness),
  colorsaturation_(colorsaturation),
  contrast_(contrast),
  exposure_(exposure),
  focus_(focus),
  ircutfilter_(ircutfilter),
  sharpness_(sharpness),
  widedynamicrange_(widedynamicrange),
  whitebalance_(whitebalance)
{

}

ImagingSettings::ImagingSettings(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BacklightCompensation")
    {
      backlightcompensation_ = BacklightCompensation(element);

    }
    else if (name == "Brightness")
    {
      brightness_ = element.text().as_float();

    }
    else if (name == "ColorSaturation")
    {
      colorsaturation_ = element.text().as_float();

    }
    else if (name == "Contrast")
    {
      contrast_ = element.text().as_float();

    }
    else if (name == "Exposure")
    {
      exposure_ = Exposure(element);

    }
    else if (name == "Focus")
    {
      focus_ = FocusConfiguration(element);

    }
    else if (name == "IrCutFilter")
    {
      ircutfilter_ = GetIrCutFilterMode(element.text().get());

    }
    else if (name == "Sharpness")
    {
      sharpness_ = element.text().as_float();

    }
    else if (name == "WideDynamicRange")
    {
      widedynamicrange_ = WideDynamicRange(element);

    }
    else if (name == "WhiteBalance")
    {
      whitebalance_ = WhiteBalance(element);

    }
  }
}

std::string ImagingSettings::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:BacklightCompensation", backlightcompensation_) + onvif::ToXml("tt:Brightness", brightness_) + onvif::ToXml("tt:ColorSaturation", colorsaturation_) + onvif::ToXml("tt:Contrast", contrast_) + ToXmlClass("tt:Exposure", exposure_) + ToXmlClass("tt:Focus", focus_) + onvif::ToXmlEnum("tt:IrCutFilter", ircutfilter_) + onvif::ToXml("tt:Sharpness", sharpness_) + ToXmlClass("tt:WhiteBalance", whitebalance_) + ToXmlClass("tt:WideDynamicRange", widedynamicrange_) + "</"+name+">");
}

bool ImagingSettings::operator==(const ImagingSettings& rhs) const
{
  return ((backlightcompensation_ == rhs.backlightcompensation_) && (brightness_ == rhs.brightness_) && (colorsaturation_ == rhs.colorsaturation_) && (contrast_ == rhs.contrast_) && (exposure_ == rhs.exposure_) && (focus_ == rhs.focus_) && (ircutfilter_ == rhs.ircutfilter_) && (sharpness_ == rhs.sharpness_) && (widedynamicrange_ == rhs.widedynamicrange_) && (whitebalance_ == rhs.whitebalance_));
}

}
