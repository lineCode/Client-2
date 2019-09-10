// intlist.cpp
//

///// Includes /////

#include <algorithm>

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IntList::IntList()
{

}

IntList::IntList(const std::vector<int>& items) :
  items_(items)
{

}

IntList::IntList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Items")
    {
      items_.push_back(element.text().as_int());

    }
  }
}

std::string IntList::ToXml(const std::string& name) const
{
  std::string items;
  for (const auto item : items_)
  {
    items += "<Items>" + std::to_string(item) + "</Items>";
  
  }

  return ("<"+name+">" + items + "</"+name+">");
}

bool IntList::operator==(const IntList& rhs) const
{
  return std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end());
}

}
