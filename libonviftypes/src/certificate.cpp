// certificate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Certificate::Certificate()
{

}

Certificate::Certificate(const boost::optional<std::string>& certificateid, const boost::optional<BinaryData>& certificate) :
  certificateid_(certificateid),
  certificate_(certificate)
{

}

Certificate::Certificate(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "CertificateID")
    {
      certificateid_ = element.text().get();

    }
    else if (name == "Certificate")
    {
      certificate_ = BinaryData(element);

    }
  }
}

std::string Certificate::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:CertificateID", certificateid_) + onvif::ToXmlClass("tt:Certificate", certificate_) + "</"+name+">");
}

bool Certificate::operator==(const Certificate& rhs) const
{
  return ((certificateid_ == rhs.certificateid_) && (certificate_ == rhs.certificate_));
}

}
