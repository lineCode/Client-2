// elementitem.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ElementItem::ElementItem()
{
  
}

ElementItem::ElementItem(const std::vector<Element>& elements) :
  elements_(elements)
{
  
}

ElementItem::ElementItem(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    elements_.push_back(Element(element));
  
  }
}

std::string ElementItem::ToXml(const std::string& name) const
{
  std::string elements;
  for (const auto& element : elements_)
  {
    elements += element.ToXml();

  }

  return ("<"+name+">" + elements + "</"+name+">");
}

bool ElementItem::operator==(const ElementItem& rhs) const
{
  return std::is_permutation(elements_.begin(), elements_.end(), rhs.elements_.begin(), rhs.elements_.end());
}

}
