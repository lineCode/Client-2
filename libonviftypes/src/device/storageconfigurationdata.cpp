// storageconfigurationdata.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

StorageConfigurationData::StorageConfigurationData()
{

}

StorageConfigurationData::StorageConfigurationData(const boost::optional<std::string> localpath, const boost::optional<std::string>& storageuri, const boost::optional<UserCredential>& user, const boost::optional<std::string>& type) :
  localpath_(localpath),
  storageuri_(storageuri),
  user_(user),
  type_(type)
{

}

StorageConfigurationData::StorageConfigurationData(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "LocalPath")
    {
      localpath_ = element.text().get();

    }
    else if (name == "StorageUri")
    {
      storageuri_ = element.text().get();

    }
    else if (name == "User")
    {
      user_ = UserCredential(element);

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "type")
    {
      type_ = attribute.value();

    }
  }
}

std::string StorageConfigurationData::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("type", type_) + ">" + onvif::ToXml("tt:LocalPath", localpath_) + onvif::ToXml("tt:StorageUri", storageuri_) + onvif::ToXmlClass("tt:User", user_) + "</"+name+">");
}

bool StorageConfigurationData::operator==(const StorageConfigurationData& rhs) const
{
  return ((type_ == rhs.type_) && (localpath_ == rhs.localpath_) && (storageuri_ == rhs.storageuri_) && (user_ == rhs.user_));
}

}

}
