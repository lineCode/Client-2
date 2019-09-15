// panelayout.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PaneLayout::PaneLayout()
{
  
}

PaneLayout::PaneLayout(const boost::optional<std::string>& pane, const boost::optional<Rectangle>& area) :
  pane_(pane),
  area_(area)
{
  
}

PaneLayout::PaneLayout(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Pane")
    {
      pane_ = element.text().get();

    }
    else if (name == "Area")
    {
      area_ = Rectangle(element);

    }
  }
}

std::string PaneLayout::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Pane", pane_) + onvif::ToXmlClass("tt:Area", area_) + "</"+name+">");
}

bool PaneLayout::operator==(const PaneLayout& rhs) const
{
  return ((pane_ == rhs.pane_) && (area_ == rhs.area_));
}

}
