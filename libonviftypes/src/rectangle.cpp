// rectangle.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Rectangle::Rectangle()
{

}

Rectangle::Rectangle(const boost::optional<float>& bottom, const boost::optional<float>& top, const boost::optional<float>& right, const boost::optional<float>& left) :
  bottom_(bottom),
  top_(top),
  right_(right),
  left_(left)
{

}

Rectangle::Rectangle(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "bottom")
    {
      bottom_ = element.text().as_float();

    }
    else if (name == "top")
    {
      top_ = element.text().as_float();

    }
    else if (name == "right")
    {
      right_ = element.text().as_float();

    }
    else if (name == "left")
    {
      left_ = element.text().as_float();

    }
  }
}

std::string Rectangle::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:bottom", bottom_) + onvif::ToXml("tt:top", top_) + onvif::ToXml("tt:right", right_) + onvif::ToXml("tt:left", left_) + "</"+name+">");
}

bool Rectangle::operator==(const Rectangle& rhs) const
{
  return ((bottom_ == rhs.bottom_) && (top_ == rhs.top_) && (right_ == rhs.right_) && (left_ == rhs.left_));
}

}
