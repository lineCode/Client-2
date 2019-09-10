// videoencoderconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions()
{
  
}

VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions(const boost::optional<IntRange>& qualityrange, const boost::optional<JpegOptions>& jpeg, const boost::optional<Mpeg4Options>& mpeg4, const boost::optional<H264Options>& h264, const boost::optional<JpegOptions2>& extensionjpeg, const boost::optional<Mpeg4Options2>& extensionmpeg4, const boost::optional<H264Options2>& extensionh264) :
  qualityrange_(qualityrange),
  jpeg_(jpeg),
  mpeg4_(mpeg4),
  h264_(h264),
  extensionjpeg_(extensionjpeg),
  extensionmpeg4_(extensionmpeg4),
  extensionh264_(extensionh264)
{
  
}

VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "QualityRange")
    {
      qualityrange_ = IntRange(element);

    }
    else if (name == "JPEG")
    {
      jpeg_ = JpegOptions(element);

    }
    else if (name == "MPEG4")
    {
      mpeg4_ = Mpeg4Options(element);

    }
    else if (name == "H264")
    {
      h264_ = H264Options(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "JPEG")
        {
          extensionjpeg_ = JpegOptions2(extension);

        }
        else if (name == "MPEG4")
        {
          extensionmpeg4_ = Mpeg4Options2(extension);

        }
        else if (name == "H264")
        {
          extensionh264_ = H264Options2(extension);

        }
      }
    }
  }
}

std::string VideoEncoderConfigurationOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:QualityRange", qualityrange_) + ToXmlClass("tt:JPEG", jpeg_) + ToXmlClass("tt:MPEG4", mpeg4_) + ToXmlClass("tt:H264", h264_) + "<Extension>" + ToXmlClass("tt:JPEG", extensionjpeg_) + ToXmlClass("tt:MPEG4", extensionmpeg4_) + ToXmlClass("tt:H264", extensionh264_) + "</Extension></"+name+">");
}

bool VideoEncoderConfigurationOptions::operator==(const VideoEncoderConfigurationOptions& rhs) const
{
  return ((qualityrange_ == rhs.qualityrange_) && (jpeg_ == rhs.jpeg_) && (mpeg4_ == rhs.mpeg4_) && (h264_ == rhs.h264_) && (extensionjpeg_ == rhs.extensionjpeg_) && (extensionmpeg4_ == rhs.extensionmpeg4_) && (extensionh264_ == rhs.extensionh264_));
}

}
