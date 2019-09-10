// storageconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

StorageConfiguration::StorageConfiguration()
{

}

StorageConfiguration::StorageConfiguration(const boost::optional<std::string> token, const boost::optional<StorageConfigurationData>& data) :
  token_(token),
  data_(data)
{

}

StorageConfiguration::StorageConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Data")
    {
      data_ = StorageConfigurationData(element);

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

std::string StorageConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("token", token_) + ">" + ToXmlClass("tt:Data", data_) + "</"+name+">");
}

bool StorageConfiguration::operator==(const StorageConfiguration& rhs) const
{
  return ((data_ == rhs.data_) && (token_ == rhs.token_));
}

}

}
