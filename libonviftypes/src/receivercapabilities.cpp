// receivercapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ReceiverCapabilities::ReceiverCapabilities()
{
  
}

ReceiverCapabilities::ReceiverCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<int>& supportedreceivers, const boost::optional<int>& maxrtspurilength) :
  xaddr_(xaddr),
  rtpmulticast_(rtpmulticast),
  rtptcp_(rtptcp),
  rtprtsptcp_(rtprtsptcp),
  supportedreceivers_(supportedreceivers),
  maxrtspurilength_(maxrtspurilength)
{
  
}

ReceiverCapabilities::ReceiverCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "RTP_Multicast")
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
    else if (name == "SupportedReceivers")
    {
      supportedreceivers_ = element.text().as_int();

    }
    else if (name == "MaximumRTSPURILength")
    {
      maxrtspurilength_ = element.text().as_int();

    }
  }
}

std::string ReceiverCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:RTP_Multicast", rtpmulticast_) + onvif::ToXml("tt:RTP_TCP", rtptcp_) + onvif::ToXml("tt:RTP_RTSP_TCP", rtprtsptcp_) + onvif::ToXml("tt:SupportedReceivers", supportedreceivers_) + onvif::ToXml("tt:MaximumRTSPURILength", maxrtspurilength_) + "</"+name+">");
}

bool ReceiverCapabilities::operator==(const ReceiverCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (rtpmulticast_ == rhs.rtpmulticast_) && (rtptcp_ == rhs.rtptcp_) && (rtprtsptcp_ == rhs.rtprtsptcp_) && (supportedreceivers_ == rhs.supportedreceivers_) && (maxrtspurilength_ == rhs.maxrtspurilength_));
}

}
