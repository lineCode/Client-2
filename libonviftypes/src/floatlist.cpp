// floatlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FloatList::FloatList()
{

}

FloatList::FloatList(const std::vector<float>& items) :
  items_(items)
{

}

FloatList::FloatList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Items")
    {
      items_.push_back(element.text().as_float());

    }
  }
}

std::string FloatList::ToXml(const std::string& name) const
{
  std::string items;
  for (const auto item : items_)
  {
    items += "<Items>" + std::to_string(item) + "</Items>";
  
  }

  return ("<"+name+">" + items + "</"+name+">");
}

bool FloatList::operator==(const FloatList& rhs) const
{
  return std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end());
}

}
