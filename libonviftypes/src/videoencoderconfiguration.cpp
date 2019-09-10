// videoencoderconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoEncoderConfiguration::VideoEncoderConfiguration()
{
  
}

VideoEncoderConfiguration::VideoEncoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<VIDEOENCODING>& encoding, const boost::optional<VideoResolution>& resolution, const boost::optional<float>& quality, const boost::optional<VideoRateControl>& ratecontrol, const boost::optional<Mpeg4Configuration>& mpeg4, const boost::optional<H264Configuration>& h264, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout) :
  name_(name),
  usecount_(usecount),
  token_(token),
  encoding_(encoding),
  resolution_(resolution),
  quality_(quality),
  ratecontrol_(ratecontrol),
  mpeg4_(mpeg4),
  h264_(h264),
  multicast_(multicast),
  sessiontimeout_(sessiontimeout)
{

}

VideoEncoderConfiguration::VideoEncoderConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "UseCount")
    {
      usecount_ = element.text().as_int();

    }
    else if (name == "Encoding")
    {
      encoding_ = GetVideoEncoding(element.text().get());

    }
    else if (name == "Resolution")
    {
      resolution_ = VideoResolution(element);

    }
    else if (name == "Quality")
    {
      quality_ = element.text().as_float();

    }
    else if (name == "RateControl")
    {
      ratecontrol_ = VideoRateControl(element);

    }
    else if (name == "MPEG4")
    {
      mpeg4_ = Mpeg4Configuration(element);

    }
    else if (name == "H264")
    {
      h264_ = H264Configuration(element);

    }
    else if (name == "Multicast")
    {
      multicast_ = MulticastConfiguration(element);

    }
    else if (name == "SessionTimeout")
    {
      sessiontimeout_ = GetDuration(element.text().get());

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }
}

std::string VideoEncoderConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_)+">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXmlEnum("tt:Encoding", encoding_) + ToXmlClass("tt:Resolution", resolution_) + onvif::ToXml("tt:Quality", quality_) + ToXmlClass("tt:RateControl", ratecontrol_) + ToXmlClass("tt:MPEG4", mpeg4_) + ToXmlClass("tt:H264", h264_) + ToXmlClass("tt:Multicast", multicast_) + onvif::ToXml("tt:SessionTimeout", sessiontimeout_) + "</"+name+">");
}

bool VideoEncoderConfiguration::operator==(const VideoEncoderConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (encoding_ == rhs.encoding_) && (resolution_ == rhs.resolution_) && (quality_ == rhs.quality_) && (ratecontrol_ == rhs.ratecontrol_) && (mpeg4_ == rhs.mpeg4_) && (h264_ == rhs.h264_) && (multicast_ == rhs.multicast_) && (sessiontimeout_ == rhs.sessiontimeout_));
}

}
