// dot11configuration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11Configuration::Dot11Configuration()
{

}

Dot11Configuration::Dot11Configuration(const boost::optional<std::string> ssid, const boost::optional<DOT11STATIONMODE>& mode, const boost::optional<std::string>& alias, const boost::optional<unsigned int>& priority, const boost::optional<Dot11SecurityConfiguration>& security) :
  ssid_(ssid),
  mode_(mode),
  alias_(alias), 
  priority_(priority),
  security_(security)
{

}

Dot11Configuration::Dot11Configuration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SSID")
    {
      ssid_ = element.text().get();

    }
    else if (name == "Mode")
    {
      mode_ = GetDot11StationMode(element.text().get());

    }
    else if (name == "Alias")
    {
      alias_ = element.text().get();

    }
    else if (name == "Priority")
    {
      priority_ = element.text().as_uint();

    }
    else if (name == "Security")
    {
      security_ = Dot11SecurityConfiguration(element);

    }
  }
}

std::string Dot11Configuration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:SSID", ssid_) + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Alias", alias_) + onvif::ToXml("tt:Priority", priority_) + onvif::ToXmlClass("tt:Security", security_) + "</"+name+">");
}

bool Dot11Configuration::operator==(const Dot11Configuration& rhs) const
{
  return ((ssid_ == rhs.ssid_) && (mode_ == rhs.mode_) && (alias_ == rhs.alias_) && (priority_ == rhs.priority_) && (security_ == rhs.security_));
}

}
