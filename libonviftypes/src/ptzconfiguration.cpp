// ptzconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZConfiguration::PTZConfiguration()
{
  
}

PTZConfiguration::PTZConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& nodetoken, const boost::optional<std::string>& defaultabsolutepantiltpositionspace, const boost::optional<std::string>& defaultabsolutezoompositionspace, const boost::optional<std::string>& defaultrelativepantilttranslationspace, const boost::optional<std::string>& defaultrelativezoomtranslationspace, const boost::optional<std::string>& defaultcontinuouspantiltvelocityspace, const boost::optional<std::string>& defaultcontinuouszoomvelocityspace, const boost::optional<DefaultPTZSpeed>& defaultptzspeed, const boost::optional<std::string>& defaultptztimeout, const boost::optional<PanTiltLimits> pantiltlimits, const boost::optional<ZoomLimits> zoomlimits) :
  name_(name),
  usecount_(usecount),
  token_(token),
  nodetoken_(nodetoken),
  defaultabsolutepantiltpositionspace_(defaultabsolutepantiltpositionspace),
  defaultabsolutezoompositionspace_(defaultabsolutezoompositionspace),
  defaultrelativepantilttranslationspace_(defaultrelativepantilttranslationspace),
  defaultrelativezoomtranslationspace_(defaultrelativezoomtranslationspace),
  defaultcontinuouspantiltvelocityspace_(defaultcontinuouspantiltvelocityspace),
  defaultcontinuouszoomvelocityspace_(defaultcontinuouszoomvelocityspace),
  defaultptzspeed_(defaultptzspeed),
  defaultptztimeout_(defaultptztimeout),
  pantiltlimits_(pantiltlimits),
  zoomlimits_(zoomlimits)
{
  
}

PTZConfiguration::PTZConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "UseCount")
    {
      usecount_ = element.text().as_int();

    }
    else if (name == "NodeToken")
    {
      nodetoken_ = element.text().get();

    }
    else if (name == "DefaultAbsolutePantTiltPositionSpace")
    {
      defaultabsolutepantiltpositionspace_ = element.text().get();

    }
    else if (name == "DefaultAbsoluteZoomPositionSpace")
    {
      defaultabsolutezoompositionspace_ = element.text().get();

    }
    else if (name == "DefaultRelativePanTiltTranslationSpace")
    {
      defaultrelativepantilttranslationspace_ = element.text().get();

    }
    else if (name == "DefaultRelativeZoomTranslationSpace")
    {
      defaultrelativezoomtranslationspace_ = element.text().get();

    }
    else if (name == "DefaultContinuousPanTiltVelocitySpace")
    {
      defaultcontinuouspantiltvelocityspace_ = element.text().get();

    }
    else if (name == "DefaultContinuousZoomVelocitySpace")
    {
      defaultcontinuouszoomvelocityspace_ = element.text().get();

    }
    else if (name == "DefaultPTZSpeed")
    {
      defaultptzspeed_ = DefaultPTZSpeed(element);

    }
    else if (name == "DefaultPTZTimeout")
    {
      defaultptztimeout_ = element.text().get();

    }
    else if (name == "PanTiltLimits")
    {
      pantiltlimits_ = PanTiltLimits(element);

    }
    else if (name == "ZoomLimits")
    {
      zoomlimits_ = ZoomLimits(element);

    }
    else if (name == "Extension")
    {
      

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }
}

std::string PTZConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ToXmlAttribute("token", token_)+">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXml("tt:NodeToken", nodetoken_) + onvif::ToXml("tt:DefaultAbsolutePantTiltPositionSpace", defaultabsolutepantiltpositionspace_) + onvif::ToXml("tt:DefaultAbsoluteZoomPositionSpace", defaultabsolutezoompositionspace_) + onvif::ToXml("tt:DefaultRelativePanTiltTranslationSpace", defaultrelativepantilttranslationspace_) + onvif::ToXml("tt:DefaultRelativeZoomTranslationSpace", defaultrelativezoomtranslationspace_) + onvif::ToXml("tt:DefaultContinuousPanTiltVelocitySpace", defaultcontinuouspantiltvelocityspace_) + onvif::ToXml("tt:DefaultContinuousZoomVelocitySpace", defaultcontinuouszoomvelocityspace_) + ToXmlClass("tt:DefaultPTZSpeed", defaultptzspeed_) + onvif::ToXml("tt:DefaultPTZTimeout", defaultptztimeout_) + ToXmlClass("tt:PanTiltLimits", pantiltlimits_) + ToXmlClass("tt:ZoomLimits", zoomlimits_) + "</"+name+">");
}

bool PTZConfiguration::operator==(const PTZConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (nodetoken_ == rhs.nodetoken_) && (defaultabsolutepantiltpositionspace_ == rhs.defaultabsolutepantiltpositionspace_) && (defaultabsolutezoompositionspace_ == rhs.defaultabsolutezoompositionspace_) && (defaultrelativepantilttranslationspace_ == rhs.defaultrelativepantilttranslationspace_) && (defaultrelativezoomtranslationspace_ == rhs.defaultrelativezoomtranslationspace_) && (defaultcontinuouspantiltvelocityspace_ == rhs.defaultcontinuouspantiltvelocityspace_) && (defaultcontinuouszoomvelocityspace_ == rhs.defaultcontinuouszoomvelocityspace_) && (defaultptzspeed_ == rhs.defaultptzspeed_) && (defaultptztimeout_ == rhs.defaultptztimeout_) && (defaultptztimeout_ == rhs.defaultptztimeout_) && (pantiltlimits_ == rhs.pantiltlimits_) && (zoomlimits_ == rhs.zoomlimits_));
}

}
