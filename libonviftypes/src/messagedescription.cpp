// itemlistdescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

MessageDescription::MessageDescription()
{

}

MessageDescription::MessageDescription(bool isproperty, const std::vector<ItemListDescription>& source, const std::vector<ItemListDescription>& key, const std::vector<ItemListDescription>& data) :
  isproperty_(isproperty),
  source_(source),
  key_(key),
  data_(data)
{

}

MessageDescription::MessageDescription(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "IsProperty")
    {
      isproperty_ = attribute.as_bool();

    }
  }

  for (const auto& element : node.children())
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Source")
    {
      source_.push_back(ItemListDescription(element));

    }
    else if (name == "Key")
    {
      key_.push_back(ItemListDescription(element));

    }
    else if (name == "Data")
    {
      data_.push_back(ItemListDescription(element));

    }
  }
}

std::string MessageDescription::ToXml(const std::string& name) const
{
  std::string sources;
  for (const auto& simpleitemdescription : source_)
  {
    sources += simpleitemdescription.ToXml("tt:Source");

  }

  std::string keys;
  for (const auto& elementitemdescription : key_)
  {
    keys += elementitemdescription.ToXml("tt:Key");

  }

  std::string datas;
  for (const auto& elementitemdescription : data_)
  {
    datas += elementitemdescription.ToXml("tt:Data");

  }

  return ("<" + name + ToXmlAttribute("IsProperty", boost::optional<bool>(isproperty_)) + ">" + sources + keys + datas + "</" + name + ">");
}

bool MessageDescription::operator==(const MessageDescription& rhs) const
{
  return ((isproperty_ == rhs.isproperty_) &&std::is_permutation(source_.begin(), source_.end(), rhs.source_.begin(), rhs.source_.end()) && std::is_permutation(key_.begin(), key_.end(), rhs.key_.begin(), rhs.key_.end()) && std::is_permutation(data_.begin(), data_.end(), rhs.data_.begin(), rhs.data_.end()));
}

}
