// intrectanglerange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IntRectangleRange::IntRectangleRange()
{
  
}

IntRectangleRange::IntRectangleRange(const boost::optional<IntRange>& xrange, const boost::optional<IntRange>& yrange, const boost::optional<IntRange>& widthrange, const boost::optional<IntRange>& heightrange) :
  xrange_(xrange),
  yrange_(yrange),
  widthrange_(widthrange),
  heightrange_(heightrange)
{
  
}

IntRectangleRange::IntRectangleRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XRange")
    {
      xrange_ = IntRange(element);

    }
    else if (name == "YRange")
    {
      yrange_ = IntRange(element);

    }
    else if (name == "WidthRange")
    {
      widthrange_ = IntRange(element);

    }
    else if (name == "HeightRange")
    {
      heightrange_ = IntRange(element);

    }
  }
}

std::string IntRectangleRange::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("XRange", xrange_) + ToXmlClass("YRange", yrange_) + ToXmlClass("WidthRange", widthrange_) + ToXmlClass("HeightRange", heightrange_) + "</"+name+">");
}

bool IntRectangleRange::operator==(const IntRectangleRange& rhs) const
{
  return ((xrange_ == rhs.xrange_) && (yrange_ == rhs.yrange_) && (widthrange_ == rhs.widthrange_) && (heightrange_ == rhs.heightrange_));
}

}
