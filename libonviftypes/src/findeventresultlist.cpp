// findeventresultlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

FindEventResultList::FindEventResultList()
{

}

FindEventResultList::FindEventResultList(const boost::optional<SEARCHSTATE>& searchstate, const std::vector<FindEventResult>& result) :
  searchstate_(searchstate),
  result_(result)
{

}

FindEventResultList::FindEventResultList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SearchState")
    {
      searchstate_ = GetSearchState(element.text().get());

    }
    else if (name == "Result")
    {
      result_.push_back(FindEventResult(element));

    }
  }
}

std::string FindEventResultList::ToXml(const std::string& name) const
{
  std::string results;
  for (const auto& result : result_)
  {
    results += result.ToXml("tt:Result");

  }

  return ("<"+name+">" + onvif::ToXmlEnum("tt:SearchState", searchstate_) + results + "</"+name+">");
}

bool FindEventResultList::operator==(const FindEventResultList& rhs) const
{
  return ((searchstate_ == rhs.searchstate_) && (result_ == rhs.result_));
}

}
