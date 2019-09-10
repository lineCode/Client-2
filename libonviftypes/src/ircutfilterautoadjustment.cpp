// ircutfilterautoadjustment.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IrCutFilterAutoAdjustment::IrCutFilterAutoAdjustment()
{

}

IrCutFilterAutoAdjustment::IrCutFilterAutoAdjustment(const boost::optional<std::string>& boundarytype, const boost::optional<float>& boundaryoffset, const boost::optional<Duration>& responsetime) :
  boundarytype_(boundarytype),
  boundaryoffset_(boundaryoffset),
  responsetime_(responsetime)
{

}

IrCutFilterAutoAdjustment::IrCutFilterAutoAdjustment(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BoundaryType")
    {
      boundarytype_ = element.text().get();

    }
    else if (name == "BoundaryOffset")
    {
      boundaryoffset_ = element.text().as_float();

    }
    else if (name == "ResponseTime")
    {
      responsetime_ = GetDuration(element.text().get());

    }
  }
}

std::string IrCutFilterAutoAdjustment::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:BoundaryType", boundarytype_) + onvif::ToXml("tt:BoundaryOffset", boundaryoffset_) + onvif::ToXml("tt:ResponseTime", responsetime_) + "</"+name+">");
}

bool IrCutFilterAutoAdjustment::operator==(const IrCutFilterAutoAdjustment& rhs) const
{
  return ((boundarytype_ == rhs.boundarytype_) && (boundaryoffset_ == rhs.boundaryoffset_) && (responsetime_ == rhs.responsetime_));
}

}
