// certificateusage.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

CertificateUsage::CertificateUsage()
{

}

CertificateUsage::CertificateUsage(const boost::optional<std::string>& usage, const boost::optional<bool>& critical) :
  usage_(usage),
  critical_(critical)
{

}

CertificateUsage::CertificateUsage(const pugi::xml_node& node)
{
  usage_ = std::string(node.text().get());
  
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "Critical")
    {
      critical_ = attribute.as_bool();

    }
  }
}

std::string CertificateUsage::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("Critical", critical_) + ">" + (usage_.is_initialized() ? *usage_ : std::string()) + "</"+name+">");
}

bool CertificateUsage::operator==(const CertificateUsage& rhs) const
{
  return ((usage_ == rhs.usage_) && (critical_ == rhs.critical_));
}

}
