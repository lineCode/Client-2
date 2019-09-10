// dot11status.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11Status::Dot11Status()
{

}

Dot11Status::Dot11Status(const boost::optional<std::string>& ssid, const boost::optional<std::string>& bssid, const boost::optional<DOT11CIPHER>& paircipher, const boost::optional<DOT11CIPHER>& groupcipher, const boost::optional<DOT11SIGNALSTRENGTH>& signalstrength, const boost::optional<std::string>& activeconfigalias) :
  ssid_(ssid),
  bssid_(bssid),
  paircipher_(paircipher),
  groupcipher_(groupcipher),
  signalstrength_(signalstrength),
  activeconfigalias_(activeconfigalias)
{

}

Dot11Status::Dot11Status(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SSID")
    {
      ssid_ = element.text().get();

    }
    else if (name == "BSSID")
    {
      bssid_ = element.text().get();

    }
    else if (name == "PairCipher")
    {
      paircipher_ = GetDot11Cipher(element.text().get());

    }
    else if (name == "GroupCipher")
    {
      groupcipher_ = GetDot11Cipher(element.text().get());

    }
    else if (name == "SignalStrength")
    {
      signalstrength_ = GetDot11SignalStrength(element.text().get());

    }
    else if (name == "ActiveConfigAlias")
    {
      activeconfigalias_ = element.text().get();

    }
  }
}

std::string Dot11Status::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:SSID", ssid_) + onvif::ToXml("tt:BSSID", bssid_) + onvif::ToXmlEnum("tt:PairCipher", paircipher_) + onvif::ToXmlEnum("tt:GroupCipher", groupcipher_) + onvif::ToXmlEnum("tt:SignalStrength", signalstrength_) + onvif::ToXml("tt:ActiveConfigAlias", activeconfigalias_) + "</"+name+">");
}

bool Dot11Status::operator==(const Dot11Status& rhs) const
{
  return ((ssid_ == rhs.ssid_) && (bssid_ == rhs.bssid_) && (paircipher_ == rhs.paircipher_) && (groupcipher_ == rhs.groupcipher_) && (signalstrength_ == rhs.signalstrength_) && (activeconfigalias_ == rhs.activeconfigalias_));
}

}
