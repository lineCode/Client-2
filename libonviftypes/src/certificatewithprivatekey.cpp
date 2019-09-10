// certificatewithprivatekey.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

CertificateWithPrivateKey::CertificateWithPrivateKey()
{

}

CertificateWithPrivateKey::CertificateWithPrivateKey(const boost::optional<std::string> certificateid, const boost::optional<BinaryData>& certificate, const boost::optional<BinaryData>& privatekey) :
  certificateid_(certificateid),
  certificate_(certificate),
  privatekey_(privatekey)
{

}

CertificateWithPrivateKey::CertificateWithPrivateKey(const pugi::xml_node& node)
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
    else if (name == "PrivateKey")
    {
      privatekey_ = BinaryData(element);

    }
  }
}

std::string CertificateWithPrivateKey::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:CertificateID", certificateid_) + onvif::ToXmlClass("tt:Certificate", certificate_) + onvif::ToXmlClass("tt:PrivateKey", privatekey_) + "</"+name+">");
}

bool CertificateWithPrivateKey::operator==(const CertificateWithPrivateKey& rhs) const
{
  return ((certificateid_ == rhs.certificateid_) && (certificate_ == rhs.certificate_) && (privatekey_ == rhs.privatekey_));
}

}
