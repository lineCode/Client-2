// joboptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

JobOptions::JobOptions()
{

}

JobOptions::JobOptions(const boost::optional<int>& spare, const boost::optional<StringAttrList>& compatiblesources) :
  spare_(spare),
  compatiblesources_(compatiblesources)
{

}

JobOptions::JobOptions(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    std::string name = TrimNamespace(attribute.name());
    if (name == "Spare")
    {
      spare_ = attribute.as_int();

    }
    else if (name == "CompatibleSources")
    {
      compatiblesources_ = StringAttrList(attribute.value());

    }
  }
}

std::string JobOptions::ToXml(const std::string& name) const
{
  std::string compatiblesources;
  if (compatiblesources_.is_initialized())
  {
    compatiblesources = std::string(" CompatibleSources=\"") + compatiblesources_->ToString() + "\"";
  
  }

  return ("<"+name + ToXmlAttribute("Spare", spare_) + compatiblesources + "></"+name+">");
}

bool JobOptions::operator==(const JobOptions& rhs) const
{
  return ((spare_ == rhs.spare_) && (compatiblesources_ == rhs.compatiblesources_));
}

}

}
