// rotate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Rotate::Rotate()
{

}

Rotate::Rotate(const boost::optional<ROTATEMODE>& rotatemode, const boost::optional<int>& degree) :
  rotatemode_(rotatemode),
  degree_(degree)
{

}

Rotate::Rotate(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      rotatemode_ = GetRotateMode(element.text().get());

    }
    else if (name == "Degree")
    {
      degree_ = element.text().as_int();

    }
  }
}

std::string Rotate::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", rotatemode_) + onvif::ToXml("tt:Degree", degree_) + "</"+name+">");
}

bool Rotate::operator==(const Rotate& rhs) const
{
  return ((rotatemode_ == rhs.rotatemode_) && (degree_ == rhs.degree_));
}

}
