// include.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Include::Include()
{

}

Include::Include(const boost::optional<std::string>& href, const std::string& data) :
  href_(href),
  data_(data.begin(), data.end())
{

}

Include::Include(const boost::optional<std::string>& href, const std::vector<char>& data) :
  href_(href),
  data_(data)
{

}

Include::Include(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "href")
    {
      href_ = attribute.value();

    }
  }

  const std::string text(node.text().get());
  data_ = std::vector<char>(text.begin(), text.end());
}

std::string Include::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("href", href_)+ " xmlns:xop=\"http://www.w3.org/2004/08/xop/include\">" + std::string(data_.begin(), data_.end()) + "</"+name+">");
}

bool Include::operator==(const Include& rhs) const
{
  return ((href_ == rhs.href_) && (data_ == rhs.data_));
}

}
