// securitycapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SecurityCapabilities::SecurityCapabilities()
{

}

SecurityCapabilities::SecurityCapabilities(const boost::optional<bool>& tls11, const boost::optional<bool>& tls12, const boost::optional<bool>& onboardkeygeneration, const boost::optional<bool>& accesspolicyconfig, const boost::optional<bool>& x509token, const boost::optional<bool>& samltoken, const boost::optional<bool>& kerberostoken, const boost::optional<bool>& reltoken, const boost::optional<bool>& tls10, const boost::optional<bool>& dot1x, const std::vector<int>& supportedeapmethod, const boost::optional<bool>& remoteuserhandling) :
  tls11_(tls11),
  tls12_(tls12),
  onboardkeygeneration_(onboardkeygeneration),
  accesspolicyconfig_(accesspolicyconfig),
  x509token_(x509token),
  samltoken_(samltoken),
  kerberostoken_(kerberostoken),
  reltoken_(reltoken),
  tls10_(tls10),
  dot1x_(dot1x),
  supportedeapmethod_(supportedeapmethod),
  remoteuserhandling_(remoteuserhandling)
{

}

SecurityCapabilities::SecurityCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TLS1.1")
    {
      tls11_ = element.text().as_bool();

    }
    else if (name == "TLS1.2")
    {
      tls12_ = element.text().as_bool();

    }
    else if (name == "OnboardKeyGeneration")
    {
      onboardkeygeneration_ = element.text().as_bool();

    }
    else if (name == "AccessPolicyConfig")
    {
      accesspolicyconfig_ = element.text().as_bool();

    }
    else if (name == "X.509Token")
    {
      x509token_ = element.text().as_bool();

    }
    else if (name == "SAMLToken")
    {
      samltoken_ = element.text().as_bool();

    }
    else if (name == "KerberosToken")
    {
      kerberostoken_ = element.text().as_bool();

    }
    else if (name == "RELToken")
    {
      reltoken_ = element.text().as_bool();

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "TLS1.0")
        {
          tls10_ = extension.text().as_bool();

        }
        else if (name == "Extension")
        {
          for (const auto& extension2 : extension)
          {
            const std::string name = TrimNamespace(extension2.name());
            if (name == "Dot1X")
            {
              dot1x_ = extension2.text().as_bool();

            }
            else if (name == "SupportedEAPMethod")
            {
              std::vector<std::string> supportedeapmethods;
              const std::string text(extension2.text().get());
              for (const auto& supportedeapmethod : boost::split(supportedeapmethods, text, boost::is_any_of(" \r\n\t")))
              {
                try
                {
                  supportedeapmethod_.push_back(std::stoi(supportedeapmethod.c_str()));

                }
                catch (...)
                {

                }
              }
            }
            else if (name == "RemoteUserHandling")
            {
              remoteuserhandling_ = extension2.text().as_bool();

            }
          }
        }
      }
    }
  }
}

std::string SecurityCapabilities::ToXml(const std::string& name) const
{
  std::string extension;
  if (tls10_.is_initialized())
  {
    extension = "<tt:Extension>" + onvif::ToXml("tt:TLS1.0", tls10_);
    if (dot1x_.is_initialized() || supportedeapmethod_.size() || remoteuserhandling_.is_initialized())
    {
      std::string supportedeapmethod;
      if (supportedeapmethod_.size())
      {
        supportedeapmethod += onvif::ToXmlList("tt:SupportedEAPMethod", supportedeapmethod_);

      }

      extension += "<tt:Extension>" + onvif::ToXml("tt:Dot1X", dot1x_) + supportedeapmethod + onvif::ToXml("tt:RemoteUserHandling", remoteuserhandling_) + "</tt:Extension>";

    }
    extension += "</tt:Extension>";
  }

  return ("<"+name+">" + onvif::ToXml("tt:TLS1.1", tls11_) + onvif::ToXml("tt:TLS1.2", tls12_) + onvif::ToXml("tt:OnboardKeyGeneration", onboardkeygeneration_) + onvif::ToXml("tt:AccessPolicyConfig", accesspolicyconfig_) + onvif::ToXml("tt:X.509Token", x509token_) + onvif::ToXml("tt:SAMLToken", samltoken_) + onvif::ToXml("tt:KerberosToken", kerberostoken_) + onvif::ToXml("tt:RELToken", reltoken_) + extension + "</"+name+">");
}

bool SecurityCapabilities::operator==(const SecurityCapabilities& rhs) const
{
  return ((tls11_ == rhs.tls11_) && (tls12_ == rhs.tls12_) && (onboardkeygeneration_ == rhs.onboardkeygeneration_) && (accesspolicyconfig_ == rhs.accesspolicyconfig_) && (x509token_ == rhs.x509token_) && (samltoken_ == rhs.samltoken_) && (kerberostoken_ == rhs.kerberostoken_) && (reltoken_ == rhs.reltoken_) && (tls10_ == rhs.tls10_) && (dot1x_ == rhs.dot1x_) && std::is_permutation(supportedeapmethod_.begin(), supportedeapmethod_.end(), rhs.supportedeapmethod_.begin(), rhs.supportedeapmethod_.end()) && (remoteuserhandling_ == rhs.remoteuserhandling_));
}

}
