// searchscope.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SearchScope::SearchScope()
{

}

SearchScope::SearchScope(const std::vector<SourceReference>& includedsources, const std::vector<std::string>& includedrecordings, const boost::optional<std::string>& recordinginformationfilter) :
  includedsources_(includedsources),
  includedrecordings_(includedrecordings),
  recordinginformationfilter_(recordinginformationfilter)
{

}

SearchScope::SearchScope(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "IncludedSources")
    {
      includedsources_.push_back(SourceReference(element));

    }
    else if (name == "IncludedRecordings")
    {
      includedrecordings_.push_back(element.text().get());

    }
    else if (name == "RecordingInformationFilter")
    {
      recordinginformationfilter_ = element.text().get();

    }
  }
}

std::string SearchScope::ToXml(const std::string& name) const
{
  std::string includedsources;
  for (const auto includedsource : includedsources_)
  {
    includedsources += includedsource.ToXml("tt:IncludedSources");

  }

  return ("<"+name+">" + includedsources + onvif::ToXml("tt:IncludedRecordings", includedrecordings_) + onvif::ToXml("tt:RecordingInformationFilter", recordinginformationfilter_) + "</"+name+">");
}

bool SearchScope::operator==(const SearchScope& rhs) const
{
  return (std::is_permutation(includedsources_.begin(), includedsources_.end(), rhs.includedsources_.begin(), rhs.includedsources_.end()) && std::is_permutation(includedrecordings_.begin(), includedrecordings_.end(), rhs.includedrecordings_.begin(), rhs.includedrecordings_.end()) && (recordinginformationfilter_ == rhs.recordinginformationfilter_));
}

}
