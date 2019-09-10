// certificateinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

CertificateInformation::CertificateInformation()
{
	
}

CertificateInformation::CertificateInformation(const boost::optional<std::string> certificateid, const boost::optional<std::string>& issuerdn, const boost::optional<std::string>& subjectdn, const boost::optional<CertificateUsage>& keyusage, const boost::optional<CertificateUsage>& extendedkeyusage, const boost::optional<int>& keylength, const boost::optional<std::string>& version, const boost::optional<std::string>& serialnum, const boost::optional<std::string>& signaturealgorithm, const boost::optional<DateTimeRange>& validity) :
  certificateid_(certificateid),
  issuerdn_(issuerdn),
  subjectdn_(subjectdn),
  keyusage_(keyusage),
  extendedkeyusage_(extendedkeyusage),
  keylength_(keylength),
  version_(version),
  signaturealgorithm_(signaturealgorithm),
  validity_(validity)
{
	
}

CertificateInformation::CertificateInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "CertificateID")
    {
      certificateid_ = std::string(element.text().get());

    }
    else if (name == "IssuerDN")
    {
      issuerdn_ = std::string(element.text().get());

    }
    else if (name == "SubjectDN")
    {
      subjectdn_ = std::string(element.text().get());

    }
    else if (name == "KeyUsage")
    {
      keyusage_ = CertificateUsage(element);

    }
    else if (name == "ExtendedKeyUsage")
    {
      extendedkeyusage_ = CertificateUsage(element);

    }
    else if (name == "KeyLength")
    {
      keylength_ = element.text().as_int();

    }
    else if (name == "Version")
    {
      version_ = std::string(element.text().get());

    }
    else if (name == "SerialNum")
    {
      serialnum_ = std::string(element.text().get());

    }
    else if (name == "SignatureAlgorithm")
    {
      signaturealgorithm_ = std::string(element.text().get());

    }
    else if (name == "Validity")
    {
      validity_ = DateTimeRange(element);

    }
  }
}

std::string CertificateInformation::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + onvif::ToXml("tt:CertificateID", certificateid_) + onvif::ToXml("tt:IssuerDN", issuerdn_) + onvif::ToXml("tt:SubjectDN", subjectdn_) + onvif::ToXmlClass("tt:KeyUsage", keyusage_) + onvif::ToXmlClass("tt:ExtendedKeyUsage", extendedkeyusage_) + onvif::ToXml("tt:KeyLength", keylength_) + onvif::ToXml("tt:Version", version_) + onvif::ToXml("tt:SerialNum", serialnum_) + onvif::ToXml("tt:SignatureAlgorithm", signaturealgorithm_) + onvif::ToXmlClass("tt:Validity", validity_) + "</"+name+">");
}

bool CertificateInformation::operator==(const CertificateInformation& rhs) const
{
  return ((certificateid_ == rhs.certificateid_) && (issuerdn_ == rhs.issuerdn_) && (subjectdn_ == rhs.subjectdn_) && (keyusage_ == rhs.keyusage_) && (extendedkeyusage_ == rhs.extendedkeyusage_) && (keylength_ == rhs.keylength_) && (version_ == rhs.version_) && (serialnum_ == rhs.serialnum_) && (signaturealgorithm_ == rhs.signaturealgorithm_) && (validity_ == rhs.validity_));
}

}
