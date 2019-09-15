// filtertype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace ws
{

///// Classes /////

FilterType::FilterType()
{

}

FilterType::FilterType(const std::vector<TopicExpression>& topicexpression, const std::vector<MessageContent>& messagecontent) :
  topicexpression_(topicexpression),
  messagecontent_(messagecontent)
{

}

FilterType::FilterType(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TopicExpression")
    {
      topicexpression_.push_back(TopicExpression(element));

    }
    else if (name == "MessageContent")
    {
      messagecontent_.push_back(MessageContent(element));

    }
  }
}

std::string FilterType::ToXml(const std::string& name) const
{
  std::string topicexpressions;
  for (const auto& topicexpression : topicexpression_)
  {
    topicexpressions += topicexpression.ToXml("wsnt:TopicExpression");

  }

  std::string messagecontents;
  for (const auto& messagecontent : messagecontent_)
  {
    messagecontents += messagecontent.ToXml("wsnt:MessageContent");

  }

  return std::string("<" + name + ">" + topicexpressions + messagecontents + "</" + name + ">");
}

bool FilterType::operator==(const FilterType& rhs) const
{
  return (std::is_permutation(topicexpression_.begin(), topicexpression_.end(), rhs.topicexpression_.begin(), rhs.topicexpression_.end()) && std::is_permutation(messagecontent_.begin(), messagecontent_.end(), rhs.messagecontent_.begin(), rhs.messagecontent_.end()));
}

}

}
