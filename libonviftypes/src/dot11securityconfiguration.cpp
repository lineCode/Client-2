// dot11securityconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11SecurityConfiguration::Dot11SecurityConfiguration()
{

}

Dot11SecurityConfiguration::Dot11SecurityConfiguration(const boost::optional<DOT11SECURITYMODE>& mode, const boost::optional<DOT11CIPHER>& algorithm, const boost::optional<Dot11PSKSet>& psk, const boost::optional<std::string>& dot1x) :
  mode_(mode),
  algorithm_(algorithm),
  psk_(psk), 
  dot1x_(dot1x)
{

}

Dot11SecurityConfiguration::Dot11SecurityConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetDot11SecurityMode(element.text().get());

    }
    else if (name == "Algorithm")
    {
      algorithm_ = GetDot11Cipher(element.text().get());

    }
    else if (name == "PSK")
    {
      psk_ = Dot11PSKSet(element);

    }
    else if (name == "Dot1X")
    {
      dot1x_ = element.text().get();

    }
  }
}

std::string Dot11SecurityConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXmlEnum("tt:Algorithm", algorithm_) + onvif::ToXmlClass("tt:PSK", psk_) + onvif::ToXml("tt:Dot1X", dot1x_) + "</"+name+">");
}

bool Dot11SecurityConfiguration::operator==(const Dot11SecurityConfiguration& rhs) const
{
  return ((mode_ == rhs.mode_) && (algorithm_ == rhs.algorithm_) && (psk_ == rhs.psk_) && (dot1x_ == rhs.dot1x_));
}

}
