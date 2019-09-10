// getrecordingjobsresponseitem.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

GetRecordingJobsResponseItem::GetRecordingJobsResponseItem()
{

}

GetRecordingJobsResponseItem::GetRecordingJobsResponseItem(const boost::optional<std::string>& jobtoken, const boost::optional<RecordingJobConfiguration>& jobconfiguration) :
  jobtoken_(jobtoken),
  jobconfiguration_(jobconfiguration)
{

}

GetRecordingJobsResponseItem::GetRecordingJobsResponseItem(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "JobToken")
    {
      jobtoken_ = element.text().get();

    }
    else if (name == "JobConfiguration")
    {
      jobconfiguration_ = RecordingJobConfiguration(element);

    }
  }
}

std::string GetRecordingJobsResponseItem::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:JobToken", jobtoken_) + onvif::ToXmlClass("tt:JobConfiguration", jobconfiguration_) + "</"+name+">");
}

bool GetRecordingJobsResponseItem::operator==(const GetRecordingJobsResponseItem& rhs) const
{
  return ((jobtoken_ == rhs.jobtoken_) && (jobconfiguration_ == rhs.jobconfiguration_));
}

}

}
