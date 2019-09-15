// tlsconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

TLSConfiguration::TLSConfiguration()
{

}

TLSConfiguration::TLSConfiguration(const boost::optional<std::string>& certificateid) :
  certificateid_(certificateid)
{

}

TLSConfiguration::TLSConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "CertificateID")
    {
      certificateid_ = element.text().get();

    }
  }
}

std::string TLSConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:CertificateID", certificateid_) + "</"+name+">");
}

bool TLSConfiguration::operator==(const TLSConfiguration& rhs) const
{
  return (certificateid_ == rhs.certificateid_);
}

}
