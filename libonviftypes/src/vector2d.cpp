// vector2d.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Vector2d::Vector2d()
{

}

Vector2d::Vector2d(const boost::optional<float>& x, const boost::optional<float>& y, const boost::optional<std::string>& space) :
  x_(x),
  y_(y),
  space_(space)
{

}

Vector2d::Vector2d(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "x")
    {
      x_ = attribute.as_float();

    }
    else if (name == "y")
    {
      y_ = attribute.as_float();

    }
    else if (name == "space")
    {
      space_ = attribute.value();

    }
  }
}

std::string Vector2d::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("x", x_) + ToXmlAttribute("y", y_) + ToXmlAttribute("space", space_) + "/>");
}

bool Vector2d::operator==(const Vector2d& rhs) const
{
  return ((x_ == rhs.x_) && (y_ == rhs.y_) && (space_ == rhs.space_));
}

}
