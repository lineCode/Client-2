// imagestabilizationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImageStabilizationOptions::ImageStabilizationOptions()
{

}

ImageStabilizationOptions::ImageStabilizationOptions(const std::vector<IMAGESTABILIZATIONMODE>& mode, const boost::optional<FloatRange>& level) :
  mode_(mode),
  level_(level)
{

}

ImageStabilizationOptions::ImageStabilizationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<IMAGESTABILIZATIONMODE> mode = GetImageStabilizationMode(element.text().get());
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

std::string ImageStabilizationOptions::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXmlClass("tt:Level", level_) + "</"+name+">");
}

bool ImageStabilizationOptions::operator==(const ImageStabilizationOptions& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && (level_ == rhs.level_));
}

}
