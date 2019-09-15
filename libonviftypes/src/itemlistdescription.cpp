// itemlistdescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ItemListDescription::ItemListDescription()
{

}

ItemListDescription::ItemListDescription(const std::vector<SimpleItemDescription>& simpleitemdescription, const std::vector<ElementItemDescription>& elementitemdescription) :
  simpleitemdescription_(simpleitemdescription),
  elementitemdescription_(elementitemdescription)
{

}

ItemListDescription::ItemListDescription(const pugi::xml_node& node)
{
  for (const auto& element : node.children())
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SimpleItemDescription")
    {
      simpleitemdescription_.push_back(SimpleItemDescription(element));

    }
    else if (name == "ElementItemDescription")
    {
      elementitemdescription_.push_back(ElementItemDescription(element));

    }
  }
}

std::string ItemListDescription::ToXml(const std::string& name) const
{
  std::string simpleitemdescriptions;
  for (const auto& simpleitemdescription : simpleitemdescription_)
  {
    simpleitemdescriptions += simpleitemdescription.ToXml("tt:SimpleItemDescription");

  }

  std::string elementitemdescriptions;
  for (const auto& elementitemdescription : elementitemdescription_)
  {
    elementitemdescriptions += elementitemdescription.ToXml("tt:ElementItemDescription");

  }

  return ("<" + name + ">" + simpleitemdescriptions + elementitemdescriptions + "</" + name + ">");
}

bool ItemListDescription::operator==(const ItemListDescription& rhs) const
{
  return (std::is_permutation(simpleitemdescription_.begin(), simpleitemdescription_.end(), rhs.simpleitemdescription_.begin(), rhs.simpleitemdescription_.end()) && std::is_permutation(elementitemdescription_.begin(), elementitemdescription_.end(), rhs.elementitemdescription_.begin(), rhs.elementitemdescription_.end()));
}

}
