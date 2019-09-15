// imagingoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImagingOptions20::ImagingOptions20()
{

}

ImagingOptions20::ImagingOptions20(const boost::optional<BacklightCompensationOptions20>& backlightcompensation, const boost::optional<FloatRange>& brightness, const boost::optional<FloatRange>& colorsaturation, const boost::optional<FloatRange>& contrast, const boost::optional<ExposureOptions20>& exposure, const boost::optional<FocusOptions20>& focus, const std::vector<IRCUTFILTERMODE>& ircutfiltermodes, const boost::optional<FloatRange>& sharpness, const boost::optional<WideDynamicRangeOptions20>& widedynamicrange, const boost::optional<WhiteBalanceOptions20>& whitebalance, const boost::optional<ImageStabilizationOptions>& imagestabilization, const boost::optional<IrCutFilterAutoAdjustmentOptions>& ircurfilterautoadjustment, const boost::optional<ToneCompensationOptions>& tonecompensationoptions, const boost::optional<DefoggingOptions>& defoggingoptions, const boost::optional<NoiseReductionOptions>& noisereductionoptions) :
  backlightcompensation_(backlightcompensation),
  brightness_(brightness),
  colorsaturation_(colorsaturation),
  contrast_(contrast),
  exposure_(exposure),
  focus_(focus),
  ircutfiltermodes_(ircutfiltermodes),
  sharpness_(sharpness),
  widedynamicrange_(widedynamicrange),
  whitebalance_(whitebalance),
  imagestabilization_(imagestabilization),
  ircurfilterautoadjustment_(ircurfilterautoadjustment),
  tonecompensationoptions_(tonecompensationoptions),
  defoggingoptions_(defoggingoptions),
  noisereductionoptions_(noisereductionoptions)
{

}

ImagingOptions20::ImagingOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BacklightCompensation")
    {
      backlightcompensation_ = BacklightCompensationOptions20(element);

    }
    else if (name == "Brightness")
    {
      brightness_ = FloatRange(element);

    }
    else if (name == "ColorSaturation")
    {
      colorsaturation_ = FloatRange(element);

    }
    else if (name == "Contrast")
    {
      contrast_ = FloatRange(element);

    }
    else if (name == "Exposure")
    {
      exposure_ = ExposureOptions20(element);

    }
    else if (name == "Focus")
    {
      focus_ = FocusOptions20(element);

    }
    else if (name == "IrCutFilterModes")
    {
      boost::optional<IRCUTFILTERMODE> ircutfiltermode = GetIrCutFilterMode(element.text().get());
      if (ircutfiltermode.is_initialized())
      {
        ircutfiltermodes_.push_back(*ircutfiltermode);

      }
    }
    else if (name == "Sharpness")
    {
      sharpness_ = FloatRange(element);

    }
    else if (name == "WideDynamicRange")
    {
      widedynamicrange_ = WideDynamicRangeOptions20(element);

    }
    else if (name == "WhiteBalance")
    {
      whitebalance_ = WhiteBalanceOptions20(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extensionelement : element)
      {
        const std::string extensionname = TrimNamespace(extensionelement.name());
        if (extensionname == "ImageStabilization")
        {
          imagestabilization_ = ImageStabilizationOptions(extensionelement);

        }
        else if (extensionname == "Extension")
        {
          for (const auto& extensionextensionelement : extensionelement)
          {
            const std::string extensionextensionname = TrimNamespace(extensionextensionelement.name());
            if (extensionextensionname == "IrCutFilterAutoAdjustment")
            {
              ircurfilterautoadjustment_ = IrCutFilterAutoAdjustmentOptions(extensionextensionelement);

            }
            else if (extensionextensionname == "Extension")
            {
              for (const auto& extensionextensionextensionelement : extensionextensionelement)
              {
                const std::string extensionextensionextensionname = TrimNamespace(extensionextensionextensionelement.name());
                if (extensionextensionextensionname == "ToneCompensationOptions")
                {
                  tonecompensationoptions_ = ToneCompensationOptions(extensionextensionextensionelement);

                }
                else if (extensionextensionextensionname == "DefoggingOptions")
                {
                  defoggingoptions_ = DefoggingOptions(extensionextensionextensionelement);

                }
                else if (extensionextensionextensionname == "NoiseReductionOptions")
                {
                  noisereductionoptions_ = NoiseReductionOptions(extensionextensionextensionelement);

                }
              }
            }
          }
        }
      }
    }
  }
}

std::string ImagingOptions20::ToXml(const std::string& name) const
{
  std::string ircutfiltermodes;
  for (const auto& ircutfiltermode : ircutfiltermodes_)
  {
    ircutfiltermodes += onvif::ToXmlEnum("IrCutFilterModes", ircutfiltermode);

  }
  
  return ("<"+name+">" + ToXmlClass("tt:BacklightCompensation", backlightcompensation_) + ToXmlClass("tt:Brightness", brightness_) + ToXmlClass("tt:ColorSaturation", colorsaturation_) + ToXmlClass("tt:Contrast", contrast_) + ToXmlClass("tt:Exposure", exposure_) + ToXmlClass("tt:Focus", focus_) + ircutfiltermodes + ToXmlClass("tt:Sharpness", sharpness_) + ToXmlClass("tt:WhiteBalance", whitebalance_) + ToXmlClass("tt:WideDynamicRange", widedynamicrange_) + std::string("<tt:Extension>") + ToXmlClass("tt:ImageStabilization", imagestabilization_) + std::string("<tt:Extension>") + ToXmlClass("tt:IrCutFilterAutoAdjustment", ircurfilterautoadjustment_) + std::string("<tt:Extension>") + ToXmlClass("tt:ToneCompensationOptions", tonecompensationoptions_) + ToXmlClass("tt:DefoggingOptions", defoggingoptions_) + ToXmlClass("tt:NoiseReductionOptions", noisereductionoptions_) + "</tt:Extension></tt:Extension></tt:Extension></"+name+">");
}

bool ImagingOptions20::operator==(const ImagingOptions20& rhs) const
{
  return ((backlightcompensation_ == rhs.backlightcompensation_) && (brightness_ == rhs.brightness_) && (colorsaturation_ == rhs.colorsaturation_) && (contrast_ == rhs.contrast_) && (exposure_ == rhs.exposure_) && (focus_ == rhs.focus_) && std::is_permutation(ircutfiltermodes_.begin(), ircutfiltermodes_.end(), rhs.ircutfiltermodes_.begin(), rhs.ircutfiltermodes_.end()) && (sharpness_ == rhs.sharpness_) && (widedynamicrange_ == rhs.widedynamicrange_) && (whitebalance_ == rhs.whitebalance_) && (imagestabilization_ == rhs.imagestabilization_) && (ircurfilterautoadjustment_ == rhs.ircurfilterautoadjustment_) && (tonecompensationoptions_ == rhs.tonecompensationoptions_) && (defoggingoptions_ == rhs.defoggingoptions_) && (noisereductionoptions_ == rhs.noisereductionoptions_));
}

}
