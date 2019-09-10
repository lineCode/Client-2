// ircutfilterautoadjustmentoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IrCutFilterAutoAdjustmentOptions::IrCutFilterAutoAdjustmentOptions()
{

}

IrCutFilterAutoAdjustmentOptions::IrCutFilterAutoAdjustmentOptions(const std::vector<IRCUTFILTERAUTOBOUNDARYTYPE>& boundarytype, const boost::optional<bool>& boundaryoffset, const boost::optional<DurationRange>& responsetimerange) :
  boundarytype_(boundarytype),
  boundaryoffset_(boundaryoffset),
  responsetimerange_(responsetimerange)
{

}

IrCutFilterAutoAdjustmentOptions::IrCutFilterAutoAdjustmentOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BoundaryType")
    {
      boost::optional<IRCUTFILTERAUTOBOUNDARYTYPE> boundarytype = GetIrCutFilterAutoBoundaryType(element.text().get());
      if (boundarytype.is_initialized())
      {
        boundarytype_.push_back(*boundarytype);

      }
    }
    else if (name == "BoundaryOffset")
    {
      boundaryoffset_ = element.text().as_bool();

    }
    else if (name == "ResponseTimeRange")
    {
      responsetimerange_ = DurationRange(element);

    }
  }
}

std::string IrCutFilterAutoAdjustmentOptions::ToXml(const std::string& name) const
{
  std::string boundarytypes;
  for (const auto& boundarytype : boundarytype_)
  {
    boundarytypes += onvif::ToXmlEnum("tt:BoundaryType", boundarytype);

  }
  
  return ("<"+name+">" + boundarytypes + onvif::ToXml("tt:BoundaryOffset", boundaryoffset_) + onvif::ToXmlClass("tt:ResponseTimeRange", responsetimerange_) + "</"+name+">");
}

bool IrCutFilterAutoAdjustmentOptions::operator==(const IrCutFilterAutoAdjustmentOptions& rhs) const
{
  return (std::is_permutation(boundarytype_.begin(), boundarytype_.end(), rhs.boundarytype_.begin(), rhs.boundarytype_.end()) && (boundaryoffset_ == rhs.boundaryoffset_) && (responsetimerange_ == rhs.responsetimerange_));
}

}
