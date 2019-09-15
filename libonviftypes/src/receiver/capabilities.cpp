// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<int>& supportedreceivers, const boost::optional<int>& maximumrtspurilength) :
  rtpmulticast_(rtpmulticast),
  rtptcp_(rtptcp),
  rtprtsptcp_(rtprtsptcp),
  supportedreceivers_(supportedreceivers),
  maximumrtspurilength_(maximumrtspurilength)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "RTP_Multicast")
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
    else if (name == "SupportedReceivers")
    {
      supportedreceivers_ = attribute.as_int();

    }
    else if (name == "MaximumRTSPURILength")
    {
      maximumrtspurilength_ = attribute.as_int();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("RTP_Multicast", rtpmulticast_) + ToXmlAttribute("RTP_TCP", rtptcp_) + ToXmlAttribute("RTP_RTSP_TCP", rtprtsptcp_) + ToXmlAttribute("SupportedReceivers", supportedreceivers_) + ToXmlAttribute("MaximumRTSPURILength", maximumrtspurilength_) + "/>");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (rtpmulticast_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RTP_Multicast"), onvif::ToString(*rtpmulticast_)));

  }

  if (rtptcp_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RTP_TCP"), onvif::ToString(*rtptcp_)));

  }

  if (rtprtsptcp_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RTP_RTSP_TCP"), onvif::ToString(*rtprtsptcp_)));

  }

  if (supportedreceivers_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SupportedReceivers"), std::to_string(*supportedreceivers_)));

  }

  if (maximumrtspurilength_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaximumRTSPURILength"), std::to_string(*maximumrtspurilength_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((rtpmulticast_ == rhs.rtpmulticast_) && (rtptcp_ == rhs.rtptcp_) && (rtprtsptcp_ == rhs.rtprtsptcp_) && (supportedreceivers_ == rhs.supportedreceivers_) && (maximumrtspurilength_ == rhs.maximumrtspurilength_));
}

}

}
