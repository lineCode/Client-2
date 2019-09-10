// itemlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ItemList::ItemList()
{

}

ItemList::ItemList(const std::vector<SimpleItem>& simpleitems, const std::vector<ElementItem>& elementitems) :
  simpleitems_(simpleitems),
  elementitems_(elementitems)
{

}

ItemList::ItemList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SimpleItem")
    {
      simpleitems_.push_back(SimpleItem(element));

    }
    else if (name == "ElementItem")
    {
      elementitems_.push_back(ElementItem(element));

    }
  }
}

std::string ItemList::ToXml(const std::string& name) const
{
  std::string items;
  for (const auto& simpleitem : simpleitems_)
  {
    items += simpleitem.ToXml("tt:SimpleItem");

  }
  
  for (const auto& elementitem : elementitems_)
  {
    items += elementitem.ToXml("tt:ElementItem");

  }

  return ("<"+name+">" + items + "</"+name+">");
}

bool ItemList::operator==(const ItemList& rhs) const
{
  return (std::is_permutation(simpleitems_.begin(), simpleitems_.end(), rhs.simpleitems_.begin(), rhs.simpleitems_.end()) && std::is_permutation(elementitems_.begin(), elementitems_.end(), rhs.elementitems_.begin(), rhs.elementitems_.end()));
}

}
