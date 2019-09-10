// securitycapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

SecurityCapabilities::SecurityCapabilities()
{

}

SecurityCapabilities::SecurityCapabilities(const boost::optional<bool>& tls10, const boost::optional<bool>& tls11, const boost::optional<bool>& tls12, const boost::optional<bool>& onboardkeygeneration, const boost::optional<bool>& accesspolicyconfig, const boost::optional<bool>& defaultaccesspolicy, const boost::optional<bool>& dot1x, const boost::optional<bool>& remoteuserhandling, const boost::optional<bool>& x509token, const boost::optional<bool>& samltoken, const boost::optional<bool>& kerberostoken, const boost::optional<bool>& usernametoken, const boost::optional<bool>& httpdigest, const boost::optional<bool>& reltoken, const std::vector<int>& supportedeapmethods, const boost::optional<int>& maxusers, const boost::optional<int>& maxusernamelength, const boost::optional<int>& maxpasswordlength) :
  tls10_(tls10),
  tls11_(tls11),
  tls12_(tls12),
  onboardkeygeneration_(onboardkeygeneration),
  accesspolicyconfig_(accesspolicyconfig),
  defaultaccesspolicy_(defaultaccesspolicy),
  dot1x_(dot1x),
  remoteuserhandling_(remoteuserhandling),
  x509token_(x509token),
  samltoken_(samltoken),
  kerberostoken_(kerberostoken),
  usernametoken_(usernametoken),
  httpdigest_(httpdigest),
  reltoken_(reltoken),
  supportedeapmethods_(supportedeapmethods),
  maxusers_(maxusers),
  maxusernamelength_(maxusernamelength),
  maxpasswordlength_(maxpasswordlength)
{

}

SecurityCapabilities::SecurityCapabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "TLS1.0")
    {
      tls10_ = attribute.as_bool();

    }
    else if (name == "TLS1.1")
    {
      tls11_ = attribute.as_bool();

    }
    else if (name == "TLS1.2")
    {
      tls12_ = attribute.as_bool();

    }
    else if (name == "OnboardKeyGeneration")
    {
      onboardkeygeneration_ = attribute.as_bool();

    }
    else if (name == "AccessPolicyConfig")
    {
      accesspolicyconfig_ = attribute.as_bool();

    }
    else if (name == "DefaultAccessPolicy")
    {
      defaultaccesspolicy_ = attribute.as_bool();

    }
    else if (name == "Dot1X")
    {
      dot1x_ = attribute.as_bool();

    }
    else if (name == "RemoteUserHandling")
    {
      remoteuserhandling_ = attribute.as_bool();

    }
    else if (name == "X.509Token")
    {
      x509token_ = attribute.as_bool();

    }
    else if (name == "SAMLToken")
    {
      samltoken_ = attribute.as_bool();

    }
    else if (name == "KerberosToken")
    {
      kerberostoken_ = attribute.as_bool();

    }
    else if (name == "UsernameToken")
    {
      usernametoken_ = attribute.as_bool();

    }
    else if (name == "HttpDigest")
    {
      httpdigest_ = attribute.as_bool();

    }
    else if (name == "RELToken")
    {
      reltoken_ = attribute.as_bool();

    }
    else if (name == "SupportedEAPMethods")
    {
      std::vector<std::string> supportedeapmethods;
      const std::string text(attribute.value());
      for (const auto& supportedeapmethod : boost::split(supportedeapmethods, text, boost::is_any_of(" \r\n\t")))
      {
        try
        {
          supportedeapmethods_.push_back(std::stoi(supportedeapmethod.c_str()));

        }
        catch (...)
        {

        }
      }
    }
    else if (name == "MaxUsers")
    {
      maxusers_ = attribute.as_int();

    }
    else if (name == "MaxUserNameLength")
    {
      maxusernamelength_ = attribute.as_int();

    }
    else if (name == "MaxPasswordLength")
    {
      maxpasswordlength_ = attribute.as_int();

    }
  }
}

std::string SecurityCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name + onvif::ToXmlAttribute("TLS1.0", tls10_) + onvif::ToXmlAttribute("TLS1.1", tls11_) + onvif::ToXmlAttribute("TLS1.2", tls12_) + onvif::ToXmlAttribute("OnboardKeyGeneration", onboardkeygeneration_) + onvif::ToXmlAttribute("AccessPolicyConfig", accesspolicyconfig_) + onvif::ToXmlAttribute("DefaultAccessPolicy", defaultaccesspolicy_) + onvif::ToXmlAttribute("Dot1X", dot1x_) + onvif::ToXmlAttribute("RemoteUserHandling", remoteuserhandling_) + onvif::ToXmlAttribute("X.509Token", x509token_) + onvif::ToXmlAttribute("SAMLToken", samltoken_) + onvif::ToXmlAttribute("KerberosToken", kerberostoken_) + onvif::ToXmlAttribute("UsernameToken", usernametoken_) + onvif::ToXmlAttribute("HttpDigest", httpdigest_) + onvif::ToXmlAttribute("RELToken", reltoken_) + onvif::ToXmlAttributeList("SupportedEAPMethods", supportedeapmethods_) + onvif::ToXmlAttribute("MaxUsers", maxusers_) + onvif::ToXmlAttribute("MaxUserNameLength", maxusernamelength_) + onvif::ToXmlAttribute("MaxPasswordLength", maxpasswordlength_) + "/>");
}

Element SecurityCapabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (tls10_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("TLS1.0"), ToString(*tls10_)));

  }

  if (tls11_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("TLS1.1"), ToString(*tls11_)));

  }

  if (tls12_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("TLS1.2"), ToString(*tls12_)));

  }

  if (onboardkeygeneration_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("OnboardKeyGeneration"), ToString(*onboardkeygeneration_)));

  }

  if (accesspolicyconfig_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("AccessPolicyConfig"), ToString(*accesspolicyconfig_)));

  }

  if (defaultaccesspolicy_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DefaultAccessPolicy"), std::to_string(*defaultaccesspolicy_)));

  }

  if (dot1x_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("Dot1X"), ToString(*dot1x_)));

  }

  if (remoteuserhandling_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RemoteUserHandling"), std::to_string(*remoteuserhandling_)));

  }

  if (x509token_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("X.509Token"), ToString(*x509token_)));

  }

  if (samltoken_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SAMLToken"), ToString(*samltoken_)));

  }

  if (kerberostoken_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("KerberosToken"), ToString(*kerberostoken_)));

  }

  if (usernametoken_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("UsernameToken"), ToString(*usernametoken_)));

  }

  if (httpdigest_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HttpDigest"), ToString(*httpdigest_)));

  }

  if (reltoken_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RELToken"), ToString(*reltoken_)));

  }

  std::vector<std::string> supportedeapmethods;
  for (auto supportedeapmethod : supportedeapmethods_)
  {
    supportedeapmethods.push_back(std::to_string(supportedeapmethod));

  }
  attributes.push_back(std::make_pair(std::string("SupportedEAPMethods"), boost::join(supportedeapmethods, std::string(" "))));

  if (maxusers_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxUsers"), std::to_string(*maxusers_)));

  }

  if (maxusernamelength_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxUserNameLength"), std::to_string(*maxusernamelength_)));

  }

  if (maxpasswordlength_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxPasswordLength"), std::to_string(*maxpasswordlength_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool SecurityCapabilities::operator==(const SecurityCapabilities& rhs) const
{
  return ((tls10_ == rhs.tls10_) && (tls11_ == rhs.tls11_) && (tls12_ == rhs.tls12_) && (onboardkeygeneration_ == rhs.onboardkeygeneration_) && (accesspolicyconfig_ == rhs.accesspolicyconfig_) && (defaultaccesspolicy_ == rhs.defaultaccesspolicy_) && (dot1x_ == rhs.dot1x_) && (remoteuserhandling_ == rhs.remoteuserhandling_) && (x509token_ == rhs.x509token_) && (samltoken_ == rhs.samltoken_) && (kerberostoken_ == rhs.kerberostoken_) && (usernametoken_ == rhs.usernametoken_) && (httpdigest_ == rhs.httpdigest_) && (reltoken_ == rhs.reltoken_) && std::is_permutation(supportedeapmethods_.begin(), supportedeapmethods_.end(), rhs.supportedeapmethods_.begin(), rhs.supportedeapmethods_.end()) && (maxusers_ == rhs.maxusers_) && (maxusernamelength_ == rhs.maxusernamelength_) && (maxpasswordlength_ == rhs.maxpasswordlength_));
}

}

}
