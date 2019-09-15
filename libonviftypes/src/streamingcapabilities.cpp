// streamingcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

StreamingCapabilities::StreamingCapabilities()
{
  
}

StreamingCapabilities::StreamingCapabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<bool>& nonaggregatecontrol, const boost::optional<bool>& nortspstreaming) :
  rtpmulticast_(rtpmulticast),
  rtptcp_(rtptcp),
  rtprtsptcp_(rtprtsptcp),
  nonaggregatecontrol_(nonaggregatecontrol),
  nortspstreaming_(nortspstreaming)
{
  
}

StreamingCapabilities::StreamingCapabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "RTPMulticast")
    {
      rtpmulticast_ = attribute.as_bool();

    }
    else if (name == "RTP_TCP")
    {
      rtptcp_ = attribute.as_bool();

    }
    else if (name == "RTP_RTSP_TCP")
    {
      rtprtsptcp_ = attribute.as_bool();
      
    }
    else if (name == "NonAggregateControl")
    {
      nonaggregatecontrol_ = attribute.as_bool();

    }
    else if (name == "NoRTSPStreaming")
    {
      nortspstreaming_ = attribute.as_bool();

    }
  }
}

std::string StreamingCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("RTPMulticast", rtpmulticast_) + ToXmlAttribute("RTP_TCP", rtptcp_) + ToXmlAttribute("RTP_RTSP_TCP", rtprtsptcp_) + ToXmlAttribute("NonAggregateControl", nonaggregatecontrol_) + ToXmlAttribute("NoRTSPStreaming", nortspstreaming_) + "></"+name+">");
}

bool StreamingCapabilities::operator==(const StreamingCapabilities& rhs) const
{
  return ((rtpmulticast_ == rhs.rtpmulticast_) && (rtptcp_ == rhs.rtptcp_) && (rtprtsptcp_ == rhs.rtprtsptcp_) && (nonaggregatecontrol_ == rhs.nonaggregatecontrol_) && (nortspstreaming_ == rhs.nortspstreaming_));
}

}
