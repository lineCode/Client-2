// intrectangle.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IntRectangle::IntRectangle()
{
  
}

IntRectangle::IntRectangle(const boost::optional<int>& x, const boost::optional<int>& y, const boost::optional<int>& width, const boost::optional<int>& height) :
  x_(x),
  y_(y),
  width_(width),
  height_(height)
{
  
}

IntRectangle::IntRectangle(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "x")
    {
      x_ = attribute.as_int();

    }
    else if (name == "y")
    {
      y_ = attribute.as_int();

    }
    else if (name == "width")
    {
      width_ = attribute.as_int();

    }
    else if (name == "height")
    {
      height_ = attribute.as_int();

    }
  }
}

std::string IntRectangle::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("x", x_) + ToXmlAttribute("y", y_) + ToXmlAttribute("width", width_) + ToXmlAttribute("height", height_) + "/>");
}

bool IntRectangle::operator==(const IntRectangle& rhs) const
{
  return ((x_ == rhs.x_) && (y_ == rhs.y_) && (width_ == rhs.width_) && (height_ == rhs.height_));
}

}
