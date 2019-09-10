// realtimestreamingcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

RealTimeStreamingCapabilities::RealTimeStreamingCapabilities()
{
	
}

RealTimeStreamingCapabilities::RealTimeStreamingCapabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp) :
  rtpmulticast_(rtpmulticast),
  rtptcp_(rtptcp),
  rtprtsptcp_(rtprtsptcp)
{
	
}

RealTimeStreamingCapabilities::RealTimeStreamingCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RTPMulticast")
    {
      rtpmulticast_ = element.text().as_bool();

    }
    else if (name == "RTP_TCP")
    {
      rtptcp_ = element.text().as_bool();

    }
    else if (name == "RTP_RTSP_TCP")
    {
      rtprtsptcp_ = element.text().as_bool();

    }
  }
}

std::string RealTimeStreamingCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:RTPMulticast", rtpmulticast_) + onvif::ToXml("tt:RTP_TCP", rtptcp_) + onvif::ToXml("tt:RTP_RTSP_TCP", rtprtsptcp_) + "</"+name+">");
}

bool RealTimeStreamingCapabilities::operator==(const RealTimeStreamingCapabilities& rhs) const
{
  return ((rtpmulticast_ == rhs.rtpmulticast_) && (rtptcp_ == rhs.rtptcp_) && (rtprtsptcp_ == rhs.rtprtsptcp_));
}

}
