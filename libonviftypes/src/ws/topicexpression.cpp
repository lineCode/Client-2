// topicexpression.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace ws
{

///// Classes /////

TopicExpression::TopicExpression()
{

}

TopicExpression::TopicExpression(const boost::optional<std::string>& dialect, const std::string& expression) :
  dialect_(dialect),
  expression_(expression)
{

}

TopicExpression::TopicExpression(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "Dialect")
    {
      dialect_ = attribute.value();
      
    }
  }

  expression_ = node.text().get();
}

std::string TopicExpression::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("Dialect", dialect_) + ">" + expression_ + "</" + name + ">");
}

bool TopicExpression::operator==(const TopicExpression& rhs) const
{
  return ((dialect_ == rhs.dialect_) && (expression_ == rhs.expression_));
}

}

}
