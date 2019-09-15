// videoratecontrol.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoRateControl::VideoRateControl()
{

}

VideoRateControl::VideoRateControl(const boost::optional<int>& frameratelimit, const boost::optional<int>& encodinginterval, const boost::optional<int>& bitratelimit) :
  frameratelimit_(frameratelimit),
  encodinginterval_(encodinginterval),
  bitratelimit_(bitratelimit)
{

}

VideoRateControl::VideoRateControl(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FrameRateLimit")
    {
      frameratelimit_ = element.text().as_int();

    }
    else if (name == "EncodingInterval")
    {
      encodinginterval_ = element.text().as_int();

    }
    else if (name == "BitrateLimit")
    {
      bitratelimit_ =element.text().as_int();

    }
  }
}

std::string VideoRateControl::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:FrameRateLimit", frameratelimit_) + onvif::ToXml("tt:EncodingInterval", encodinginterval_) + onvif::ToXml("tt:BitrateLimit", bitratelimit_) + "</"+name+">");
}

bool VideoRateControl::operator==(const VideoRateControl& rhs) const
{
  return ((frameratelimit_ == rhs.frameratelimit_) && (encodinginterval_ == rhs.encodinginterval_) && (bitratelimit_ == rhs.bitratelimit_));
}

}
