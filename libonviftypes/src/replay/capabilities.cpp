// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace replay
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& reverseplayback, const boost::optional<FloatAttrList>& sessiontimeoutrange, const boost::optional<bool>& rtprtsptcp) :
  reverseplayback_(reverseplayback),
  sessiontimeoutrange_(sessiontimeoutrange),
  rtprtsptcp_(rtprtsptcp)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "ReversePlayback")
    {
      reverseplayback_ = attribute.as_bool();
      
    }
    else if (name == "SessionTimeoutRange")
    {
      sessiontimeoutrange_ = FloatAttrList(attribute.value());

    }
    else if (name == "RTP_RTSP_TCP")
    {
      rtprtsptcp_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  std::string sessiontimeoutrange;
  if (sessiontimeoutrange_.is_initialized())
  {
    sessiontimeoutrange = std::string(" trp:SessionTimeoutRange=\"") + sessiontimeoutrange_->ToString() + "\"";
  
  }

  return ("<"+name + ToXmlAttribute("trp:ReversePlayback", reverseplayback_) + sessiontimeoutrange + ToXmlAttribute("trp:RTP_RTSP_TCP", rtprtsptcp_) + "></"+name+">");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (reverseplayback_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("trp:ReversePlayback"), onvif::ToString(*reverseplayback_)));

  }

  if (sessiontimeoutrange_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("trp:SessionTimeoutRange"), sessiontimeoutrange_->ToString()));

  }

  if (rtprtsptcp_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("trp:RTP_RTSP_TCP"), std::to_string(*rtprtsptcp_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((reverseplayback_ == rhs.reverseplayback_) && (sessiontimeoutrange_ == rhs.sessiontimeoutrange_) && (rtprtsptcp_ == rhs.rtprtsptcp_));
}

}

}
