// analyticsengineconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AnalyticsEngineConfiguration::AnalyticsEngineConfiguration()
{

}

AnalyticsEngineConfiguration::AnalyticsEngineConfiguration(const std::vector<Config>& analyticsmodules) :
  analyticsmodules_(analyticsmodules)
{

}

AnalyticsEngineConfiguration::AnalyticsEngineConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AnalyticsModule")
    {
      analyticsmodules_.push_back(Config(element));

    }
  }
}

std::string AnalyticsEngineConfiguration::ToXml(const std::string& name) const
{
  std::string analyticmodules;
  for (const auto& analyticsmodule : analyticsmodules_)
  {
    analyticmodules += analyticsmodule.ToXml("tt:AnalyticsModule");

  }

  return ("<"+name+">" + analyticmodules + "</"+name+">");
} 

bool AnalyticsEngineConfiguration::operator==(const AnalyticsEngineConfiguration& rhs) const
{
  return std::is_permutation(analyticsmodules_.begin(), analyticsmodules_.end(), rhs.analyticsmodules_.begin(), rhs.analyticsmodules_.end());
}

}
