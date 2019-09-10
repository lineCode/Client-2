// iocapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IOCapabilities::IOCapabilities()
{

}

IOCapabilities::IOCapabilities(const boost::optional<int>& inputconnectors, const boost::optional<int>& relayoutputs) :
  inputconnectors_(inputconnectors),
  relayoutputs_(relayoutputs)
{

}

IOCapabilities::IOCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "InputConnectors")
    {
      inputconnectors_ = element.text().as_int();

    }
    else if (name == "RelayOutputs")
    {
      relayoutputs_ = element.text().as_int();

    }
  }
}

std::string IOCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:InputConnectors", inputconnectors_) + onvif::ToXml("tt:RelayOutputs", relayoutputs_) + "</"+name+">");
}

bool IOCapabilities::operator==(const IOCapabilities& rhs) const
{
  return ((inputconnectors_ == rhs.inputconnectors_) && (relayoutputs_ == rhs.relayoutputs_));
}

}
