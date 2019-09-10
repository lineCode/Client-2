// ptznode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZNode::PTZNode()
{

}

PTZNode::PTZNode(const boost::optional<std::string>& token, const boost::optional<bool>& fixedhomeposition, const boost::optional<std::string>& name, const boost::optional<PTZSpaces>& supportedptzspaces, const boost::optional<int>& maximumnumberofpresets, const boost::optional<bool>& homesupported, const std::vector<std::string>& auxiliarycommands, const std::vector<PTZPresetTourSupported>& supportedpresettour) :
  token_(token),
  fixedhomeposition_(fixedhomeposition),
  name_(name),
  supportedptzspaces_(supportedptzspaces),
  maximumnumberofpresets_(maximumnumberofpresets),
  homesupported_(homesupported),
  auxiliarycommands_(auxiliarycommands),
  supportedpresettour_(supportedpresettour)
{

}

PTZNode::PTZNode(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
    if (name == "FixedHomePosition")
    {
      fixedhomeposition_ = attribute.as_bool();

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "SupportedPTZSpaces")
    {
      supportedptzspaces_ = PTZSpaces(element);

    }
    else if (name == "MaximumNumberOfPresets")
    {
      maximumnumberofpresets_ = element.text().as_int();

    }
    else if (name == "HomeSupported")
    {
      homesupported_ = element.text().as_bool();

    }
    else if (name == "AuxiliaryCommands")
    {
      auxiliarycommands_.push_back(element.text().get());

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "SupportedPresetTour")
        {
          supportedpresettour_.push_back(PTZPresetTourSupported(extension));

        }
      }
    }
  }
}

std::string PTZNode::ToXml(const std::string& name) const
{
  std::string supportedpresettours;
  for (const auto& supportedpresettour : supportedpresettour_)
  {
    supportedpresettours += supportedpresettour.ToXml("tt:SupportedPresetTour");

  }

  return ("<"+name + ToXmlAttribute("token", token_) + ToXmlAttribute("FixedHomePosition", fixedhomeposition_) + ">" + onvif::ToXml("tt:Name", name_) + ToXmlClass("tt:SupportedPTZSpaces", supportedptzspaces_) + onvif::ToXml("tt:MaximumNumberOfPresets", maximumnumberofpresets_) + onvif::ToXml("tt:HomeSupported", homesupported_) + onvif::ToXml("tt:AuxiliaryCommands", auxiliarycommands_) + std::string("<tt:Extension>") + supportedpresettours + "</tt:Extension></"+name+">");
}

bool PTZNode::operator==(const PTZNode& rhs) const
{
  return ((token_ == rhs.token_) && (fixedhomeposition_ == rhs.fixedhomeposition_) && (name_ == rhs.name_) && (supportedptzspaces_ == rhs.supportedptzspaces_) && (maximumnumberofpresets_ == rhs.maximumnumberofpresets_) && (homesupported_ == rhs.homesupported_) && std::is_permutation(auxiliarycommands_.begin(), auxiliarycommands_.end(), rhs.auxiliarycommands_.begin(), rhs.auxiliarycommands_.end()) && (supportedpresettour_ == rhs.supportedpresettour_));
}

}
