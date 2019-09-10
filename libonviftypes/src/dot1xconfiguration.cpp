// dot1xconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot1XConfiguration::Dot1XConfiguration()
{

}

Dot1XConfiguration::Dot1XConfiguration(const boost::optional<std::string> dot1xconfigurationtoken, const boost::optional<std::string>& identity, const boost::optional<std::string>& anonymousid, const boost::optional<int>& eapmethod, const boost::optional<std::string>& certificateid, const boost::optional<EAPMethodConfiguration>& eapmethodconfiguration) :
  dot1xconfigurationtoken_(dot1xconfigurationtoken),
  identity_(identity),
  anonymousid_(anonymousid),
  eapmethod_(eapmethod),
  certificateid_(certificateid),
  eapmethodconfiguration_(eapmethodconfiguration)
{

}

Dot1XConfiguration::Dot1XConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Dot1XConfigurationToken")
    {
      dot1xconfigurationtoken_ = element.text().get();

    }
    else if (name == "Identity")
    {
      identity_ = element.text().get();

    }
    else if (name == "AnonymousID")
    {
      anonymousid_ = element.text().get();

    }
    else if (name == "EAPMethod")
    {
      eapmethod_ = element.text().as_int();

    }
    else if (name == "CertificateID")
    {
      certificateid_ = element.text().get();

    }
    else if (name == "EAPMethodConfiguration")
    {
      eapmethodconfiguration_ = EAPMethodConfiguration(element);

    }
  }
}

std::string Dot1XConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Dot1XConfigurationToken", dot1xconfigurationtoken_) + onvif::ToXml("tt:Identity", identity_) + onvif::ToXml("tt:AnonymousID", anonymousid_) + onvif::ToXml("tt:EAPMethod", eapmethod_) + onvif::ToXml("tt:CertificateID", certificateid_) + onvif::ToXmlClass("tt:EAPMethodConfiguration", eapmethodconfiguration_) + "</"+name+">");
}

bool Dot1XConfiguration::operator==(const Dot1XConfiguration& rhs) const
{
  return ((dot1xconfigurationtoken_ == rhs.dot1xconfigurationtoken_) && (identity_ == rhs.identity_) && (anonymousid_ == rhs.anonymousid_) && (eapmethod_ == rhs.eapmethod_) && (certificateid_ == rhs.certificateid_) && (eapmethodconfiguration_ == rhs.eapmethodconfiguration_));
}

}
