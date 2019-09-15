// element.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Element::Element()
{
  
}

Element::Element(const std::string& name, const std::vector< std::pair<std::string, std::string> >& attributes, const std::string& text, const std::vector<Element>& elements) :
  name_(name),
  attributes_(attributes),
  text_(text),
  elements_(elements)
{
  
}

Element::Element(const pugi::xml_node& node) :
  name_(node.name()), text_(node.text().get())
{
  for (const auto& attribute : node.attributes())
  {
    attributes_.push_back({ attribute.name(), attribute.value() });
  
  }

  for (const auto& element : node)
  {
    if (element.type() == pugi::node_element)
    {
      elements_.push_back(Element(element));

    }
  }
}

std::string Element::ToXml() const
{
  std::string attributes;
  for (const auto& attribute : attributes_)
  {
    attributes += " " + attribute.first + "=\"" + attribute.second + "\"";
  
  }

  std::string elements;
  for (const auto& element : elements_)
  {
    elements += element.ToXml();
  
  }

  return ("<"+name_+attributes+">" + text_ + elements + "</"+name_+">");
}

bool Element::operator==(const Element& rhs) const
{
  // everything must be the same size
  if (attributes_.size() != rhs.attributes_.size())
  {
    
    return false;
  }

  if (elements_.size() != rhs.elements_.size())
  {
    
    return false;
  }

  // Check all members
  if (name_ != rhs.name_)
  {
  
    return false;
  }

  if (text_ != rhs.text_)
  {
  
    return false;
  }

  return (std::is_permutation(attributes_.begin(), attributes_.end(), rhs.attributes_.begin(), rhs.attributes_.end()) && std::is_permutation(elements_.begin(), elements_.end(), rhs.elements_.begin(), rhs.elements_.end()));
}

}
