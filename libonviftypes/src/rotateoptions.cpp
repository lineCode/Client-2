// rotateoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

RotateOptions::RotateOptions()
{

}

RotateOptions::RotateOptions(const std::vector<ROTATEMODE>& mode, const boost::optional<IntList>& degreelist) :
  mode_(mode),
  degreelist_(degreelist)
{

}

RotateOptions::RotateOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<ROTATEMODE> mode = GetRotateMode(element.text().get());
      if (!mode.is_initialized())
      {

        continue;
      }

      mode_.push_back(*mode);

    }
    else if (name == "DegreeList")
    {
      degreelist_ = IntList(element);

    }
  }
}

std::string RotateOptions::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXmlClass("tt:DegreeList", degreelist_) + "</"+name+">");
}

bool RotateOptions::operator==(const RotateOptions& rhs) const
{
  return ((std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end())) && (degreelist_ == rhs.degreelist_));
}

}
