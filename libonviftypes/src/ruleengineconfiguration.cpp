// ruleengineconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RuleEngineConfiguration::RuleEngineConfiguration()
{
  
}

RuleEngineConfiguration::RuleEngineConfiguration(const std::vector<Config>& rules) :
  rules_(rules)
{
  
}

RuleEngineConfiguration::RuleEngineConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Rule")
    {
      rules_.push_back(Config(element));

    }
  }
}

std::string RuleEngineConfiguration::ToXml(const std::string& name) const
{
  std::string rules;
  for (const auto& rule : rules_)
  {
    rules += rule.ToXml("tt:Rule");

  }

  return ("<"+name+">" + rules + "</"+name+">");
}

bool RuleEngineConfiguration::operator==(const RuleEngineConfiguration& rhs) const
{
  return std::is_permutation(rules_.begin(), rules_.end(), rhs.rules_.begin(), rhs.rules_.end());
}

}
