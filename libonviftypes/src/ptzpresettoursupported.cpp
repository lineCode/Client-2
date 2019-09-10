// ptzpresettoursupported.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZPresetTourSupported::PTZPresetTourSupported()
{

}

PTZPresetTourSupported::PTZPresetTourSupported(const boost::optional<int>& maximumnumberofpresettours, const std::vector<PTZPRESETTOUROPERATION>& ptzpresettouroperation) :
  maximumnumberofpresettours_(maximumnumberofpresettours),
  ptzpresettouroperation_(ptzpresettouroperation)
{

}

PTZPresetTourSupported::PTZPresetTourSupported(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "MaximumNumberOfPresetTours")
    {
      maximumnumberofpresettours_ = element.text().as_int();

    }
    else if (name == "PTZPresetTourOperation")
    {
      boost::optional<PTZPRESETTOUROPERATION> ptzpresettouroperation = GetPTZPresetTourOperation(element.text().get());
      if (ptzpresettouroperation.is_initialized())
      {
        ptzpresettouroperation_.push_back(*ptzpresettouroperation);

      }

    }
  }
}

std::string PTZPresetTourSupported::ToXml(const std::string& name) const
{
  std::string ptzpresettouroperations;
  for (const auto& ptzpresettouroperation : ptzpresettouroperation_)
  {
    ptzpresettouroperations += onvif::ToXmlEnum("tt:PTZPresetTourOperation", ptzpresettouroperation);

  }

  return ("<"+name+">" + onvif::ToXml("tt:MaximumNumberOfPresetTours", maximumnumberofpresettours_) + ptzpresettouroperations + "</"+name+">");
}

bool PTZPresetTourSupported::operator==(const PTZPresetTourSupported& rhs) const
{
  return ((maximumnumberofpresettours_ == rhs.maximumnumberofpresettours_) && std::is_permutation(ptzpresettouroperation_.begin(), ptzpresettouroperation_.end(), rhs.ptzpresettouroperation_.begin(), rhs.ptzpresettouroperation_.end()));
}

}
