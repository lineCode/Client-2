// mediacapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

MediaCapabilities::MediaCapabilities()
{

}

MediaCapabilities::MediaCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<RealTimeStreamingCapabilities>& streamingcapabilities, const boost::optional<ProfileCapabilities>& profilecapabilities) :
  xaddr_(xaddr),
  streamingcapabilities_(streamingcapabilities),
  profilecapabilities_(profilecapabilities)
{

}

MediaCapabilities::MediaCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "StreamingCapabilities")
    {
      streamingcapabilities_ = RealTimeStreamingCapabilities(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "ProfileCapabilities")
        {
          profilecapabilities_ = ProfileCapabilities(extension);

        }
      }
    }
  }
}

std::string MediaCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + ToXmlClass("tt:StreamingCapabilities", streamingcapabilities_) + "<tt:Extension>" + ToXmlClass("tt:ProfileCapabilities", profilecapabilities_) + "</tt:Extension></"+name+">");
}

bool MediaCapabilities::operator==(const MediaCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (streamingcapabilities_ == rhs.streamingcapabilities_) && (profilecapabilities_ == rhs.profilecapabilities_));
}

}
