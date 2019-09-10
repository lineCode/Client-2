// systemlogurilist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

SystemLogUriList::SystemLogUriList()
{

}

SystemLogUriList::SystemLogUriList(const std::vector<SystemLogUri>& systemloguris) :
  systemloguris_(systemloguris)
{

}

SystemLogUriList::SystemLogUriList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SystemLog")
    {
      systemloguris_.push_back(SystemLogUri(element));

    }
  }
}

std::string SystemLogUriList::ToXml(const std::string& name) const
{
  std::string systemloguris;
  for (const auto& systemloguri : systemloguris_)
  {
    systemloguris += systemloguri.ToXml("tt:SystemLog");

  }
  
  return ("<"+name+">" + systemloguris + "</"+name+">");
}

bool SystemLogUriList::operator==(const SystemLogUriList& rhs) const
{
  return std::is_permutation(systemloguris_.begin(), systemloguris_.end(), rhs.systemloguris_.begin(), rhs.systemloguris_.end());
}

}
