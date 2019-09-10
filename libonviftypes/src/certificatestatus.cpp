// certificatestatus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

CertificateStatus::CertificateStatus()
{

}

CertificateStatus::CertificateStatus(const boost::optional<std::string>& certificateid, const boost::optional<bool>& status) :
  certificateid_(certificateid),
  status_(status)
{

}

CertificateStatus::CertificateStatus(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "CertificateID")
    {
      certificateid_ = element.text().get();

    }
    else if (name == "Status")
    {
      status_ = element.text().as_bool();

    }
  }
}

std::string CertificateStatus::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:CertificateID", certificateid_) + onvif::ToXml("tt:Status", status_) + "</"+name+">");
}

bool CertificateStatus::operator==(const CertificateStatus& rhs) const
{
  return ((certificateid_ == rhs.certificateid_) && (status_ == rhs.status_));
}

}
