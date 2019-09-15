// imagingsettings20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImagingSettings20::ImagingSettings20()
{

}

ImagingSettings20::ImagingSettings20(const boost::optional<BacklightCompensation20>& backlightcompensation, const boost::optional<float>& brightness, const boost::optional<float>& colorsaturation, const boost::optional<float>& contrast, const boost::optional<Exposure20>& exposure, const boost::optional<FocusConfiguration20>& focus, const boost::optional<IRCUTFILTERMODE>& ircutfilter, const boost::optional<float>& sharpness, const boost::optional<WideDynamicRange20>& widedynamicrange, const boost::optional<WhiteBalance20>& whitebalance, const boost::optional<ImageStabilization>& imagestabilization, const boost::optional<IrCutFilterAutoAdjustment>& ircurfilterautoadjustment, const boost::optional<ToneCompensation>& tonecompensation, const boost::optional<Defogging>& defogging, const boost::optional<NoiseReduction>& noisereduction) :
  backlightcompensation_(backlightcompensation),
  brightness_(brightness),
  colorsaturation_(colorsaturation),
  contrast_(contrast),
  exposure_(exposure),
  focus_(focus),
  ircutfilter_(ircutfilter),
  sharpness_(sharpness),
  widedynamicrange_(widedynamicrange),
  whitebalance_(whitebalance),
  imagestabilization_(imagestabilization),
  ircurfilterautoadjustment_(ircurfilterautoadjustment),
  tonecompensation_(tonecompensation),
  defogging_(defogging),
  noisereduction_(noisereduction)
{

}

ImagingSettings20::ImagingSettings20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BacklightCompensation")
    {
      backlightcompensation_ = BacklightCompensation20(element);

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
      exposure_ = Exposure20(element);

    }
    else if (name == "Focus")
    {
      focus_ = FocusConfiguration20(element);

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
      widedynamicrange_ = WideDynamicRange20(element);

    }
    else if (name == "WhiteBalance")
    {
      whitebalance_ = WhiteBalance20(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extensionelement : element)
      {
        const std::string extensionname = TrimNamespace(extensionelement.name());
        if (extensionname == "ImageStabilization")
        {
          imagestabilization_ = ImageStabilization(extensionelement);

        }
        else if (extensionname == "Extension")
        {
          for (const auto& extensionextensionelement : extensionelement)
          {
            const std::string extensionextensionname = TrimNamespace(extensionextensionelement.name());
            if (extensionextensionname == "IrCutFilterAutoAdjustment")
            {
              ircurfilterautoadjustment_ = IrCutFilterAutoAdjustment(extensionextensionelement);

            }
            else if (extensionextensionname == "Extension")
            {
              for (const auto& extensionextensionextensionelement : extensionextensionelement)
              {
                const std::string extensionextensionextensionname = TrimNamespace(extensionextensionextensionelement.name());
                if (extensionextensionextensionname == "ToneCompensation")
                {
                  tonecompensation_ = ToneCompensation(extensionextensionextensionelement);

                }
                else if (extensionextensionextensionname == "Defogging")
                {
                  defogging_ = Defogging(extensionextensionextensionelement);

                }
                else if (extensionextensionextensionname == "NoiseReduction")
                {
                  noisereduction_ = NoiseReduction(extensionextensionextensionelement);

                }
              }
            }
          }
        }
      }
    }
  }
}

std::string ImagingSettings20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:BacklightCompensation", backlightcompensation_) + onvif::ToXml("tt:Brightness", brightness_) + onvif::ToXml("tt:ColorSaturation", colorsaturation_) + onvif::ToXml("tt:Contrast", contrast_) + ToXmlClass("tt:Exposure", exposure_) + ToXmlClass("tt:Focus", focus_) + onvif::ToXmlEnum("tt:IrCutFilter", ircutfilter_) + onvif::ToXml("tt:Sharpness", sharpness_) + ToXmlClass("tt:WhiteBalance", whitebalance_) + ToXmlClass("tt:WideDynamicRange", widedynamicrange_) + std::string("<tt:Extension>") + ToXmlClass("tt:ImageStabilization", imagestabilization_) + std::string("<tt:Extension>") + ToXmlClass("tt:IrCutFilterAutoAdjustment", ircurfilterautoadjustment_) + std::string("<tt:Extension>") + ToXmlClass("tt:ToneCompensation", tonecompensation_) + ToXmlClass("tt:Defogging", defogging_) + ToXmlClass("tt:NoiseReduction", noisereduction_) + "</tt:Extension></tt:Extension></tt:Extension></"+name+">");
}

bool ImagingSettings20::operator==(const ImagingSettings20& rhs) const
{
  return ((backlightcompensation_ == rhs.backlightcompensation_) && (brightness_ == rhs.brightness_) && (colorsaturation_ == rhs.colorsaturation_) && (contrast_ == rhs.contrast_) && (exposure_ == rhs.exposure_) && (focus_ == rhs.focus_) && (ircutfilter_ == rhs.ircutfilter_) && (sharpness_ == rhs.sharpness_) && (widedynamicrange_ == rhs.widedynamicrange_) && (whitebalance_ == rhs.whitebalance_) && (imagestabilization_ == rhs.imagestabilization_) && (ircurfilterautoadjustment_ == rhs.ircurfilterautoadjustment_) && (tonecompensation_ == rhs.tonecompensation_) && (defogging_ == rhs.defogging_) && (noisereduction_ == rhs.noisereduction_));
}

}
