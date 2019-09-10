// transport.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Transport::Transport()
{

}

Transport::Transport(const Transport& transport) :
  protocol_(transport.protocol_),
  tunnel_(transport.tunnel_ ? std::make_unique<Transport>(*transport.tunnel_) : nullptr)
{
  
}

Transport::Transport(TRANSPORTPROTOCOL transportprotocol) :
  protocol_(transportprotocol)
{

}

Transport::Transport(TRANSPORTPROTOCOL transportprotocol, const Transport& transport) :
  protocol_(transportprotocol),
  tunnel_(std::make_unique<Transport>(transport))
{

}

Transport::Transport(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Protocol")
    {
      protocol_ = GetTransportProtocol(element.text().get());

    }
    else if (name == "Tunnel")
    {
      tunnel_ = std::make_unique<Transport>(element);

    }
  }
}

std::string Transport::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Protocol", protocol_) + (tunnel_ ? tunnel_->ToXml("tt:Tunnel") :  std::string()) + "</"+name+">");
}

Transport Transport::operator=(const Transport& transport)
{
  protocol_ = transport.protocol_;
  if (transport.tunnel_)
  {
    tunnel_ = std::make_unique<Transport>(*transport.tunnel_);
  
  }
  return *this;
}

bool Transport::operator==(const Transport& rhs) const
{
  if (tunnel_ && rhs.tunnel_)
  {

    return ((protocol_ == rhs.protocol_) && (*tunnel_ == *rhs.tunnel_));
  }
  else if (!tunnel_ && !rhs.tunnel_)
  {

    return (protocol_ == rhs.protocol_);
  }
  else
  {

    return false;
  }
}

}
