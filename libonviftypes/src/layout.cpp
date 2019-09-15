// layout.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Layout::Layout()
{
  
}

Layout::Layout(const std::vector<PaneLayout>& panelayout) :
  panelayout_(panelayout)
{
  
}

  Layout::Layout(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "PaneLayout")
    {
      panelayout_.push_back(PaneLayout(element));

    }
  }
}

std::string Layout::ToXml(const std::string& name) const
{
  std::string panelayouts;
  for (const auto& panelayout : panelayout_)
  {
    panelayouts += panelayout.ToXml("PaneLayout");

  }

  return ("<"+name+">" + panelayouts + "</"+name+">");
}

bool Layout::operator==(const Layout& rhs) const
{
  return std::is_permutation(panelayout_.begin(), panelayout_.end(), rhs.panelayout_.begin(), rhs.panelayout_.end());
}

}
