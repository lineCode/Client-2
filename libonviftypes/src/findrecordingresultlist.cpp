// findrecordingresultlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FindRecordingResultList::FindRecordingResultList()
{

}

FindRecordingResultList::FindRecordingResultList(const boost::optional<SEARCHSTATE>& searchstate, const std::vector<onvif::RecordingInformation>& recordinginformation) :
  searchstate_(searchstate),
  recordinginformation_(recordinginformation)
{

}

FindRecordingResultList::FindRecordingResultList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SearchState")
    {
      searchstate_ = GetSearchState(element.text().get());

    }
    else if (name == "RecordingInformation")
    {
      recordinginformation_.push_back(RecordingInformation(element));

    }
  }
}

std::string FindRecordingResultList::ToXml(const std::string& name) const
{
  std::string recordingsinformation;
  for (const RecordingInformation& recordinginformation : recordinginformation_)
  {
    recordingsinformation += recordinginformation.ToXml("tt:RecordingInformation");

  }
  return ("<"+name+">" + onvif::ToXmlEnum("tt:SearchState", searchstate_) + recordingsinformation + "</"+name+">");
}

bool FindRecordingResultList::operator==(const FindRecordingResultList& rhs) const
{
  return ((searchstate_ == rhs.searchstate_) && (recordinginformation_ == rhs.recordinginformation_));
}

}
