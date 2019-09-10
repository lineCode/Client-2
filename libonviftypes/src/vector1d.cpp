// vector1d.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Vector1d::Vector1d()
{

}

Vector1d::Vector1d(const boost::optional<float>& x, const boost::optional<std::string>& space) :
  x_(x),
  space_(space)
{

}

Vector1d::Vector1d(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "x")
    {
      x_ = attribute.as_float();

    }
    else if (name == "space")
    {
      space_ = attribute.value();

    }
  }
}

std::string Vector1d::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("x", x_) + ToXmlAttribute("space", space_) + "/>");
}

bool Vector1d::operator==(const Vector1d& rhs) const
{
  return ((x_ == rhs.x_) && (space_ == rhs.space_));
}

}
