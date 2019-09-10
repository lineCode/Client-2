// deviceiocapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

DeviceIOCapabilities::DeviceIOCapabilities()
{
  
}

DeviceIOCapabilities::DeviceIOCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<int>& videosources, const boost::optional<int>& videooutputs, const boost::optional<int>& audiosources, const boost::optional<int>& audiooutputs, const boost::optional<int>& relayoutputs) :
  xaddr_(xaddr),
  videosources_(videosources),
  videooutputs_(videooutputs),
  audiosources_(audiosources),
  audiooutputs_(audiooutputs),
  relayoutputs_(relayoutputs)
{
  
}

DeviceIOCapabilities::DeviceIOCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "VideoSources")
    {
      videosources_ = element.text().as_int();

    }
    else if (name == "VideoOutputs")
    {
      videooutputs_ = element.text().as_int();

    }
    else if (name == "AudioSources")
    {
      audiosources_ = element.text().as_int();

    }
    else if (name == "AudioOutputs")
    {
      audiooutputs_ = element.text().as_int();

    }
    else if (name == "RelayOutputs")
    {
      relayoutputs_ = element.text().as_int();

    }
  }
}

std::string DeviceIOCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:VideoSources", videosources_) + onvif::ToXml("tt:VideoOutputs", videooutputs_) + onvif::ToXml("tt:AudioSources", audiosources_) + onvif::ToXml("tt:AudioOutputs", audiooutputs_) + onvif::ToXml("tt:RelayOutputs", relayoutputs_) + "</"+name+">");
}

bool DeviceIOCapabilities::operator==(const DeviceIOCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (videosources_ == rhs.videosources_) && (videooutputs_ == rhs.videooutputs_) && (audiosources_ == rhs.audiosources_) && (audiooutputs_ == rhs.audiooutputs_) && (relayoutputs_ == rhs.relayoutputs_));
}

}
