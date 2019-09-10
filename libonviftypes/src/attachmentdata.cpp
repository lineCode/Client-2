// attachmentdata.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AttachmentData::AttachmentData()
{

}

AttachmentData::AttachmentData(const boost::optional<Include>& include, const boost::optional<std::string>& contenttype) :
  include_(include),
  contenttype_(contenttype)
{

}

AttachmentData::AttachmentData(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Include")
    {
      include_ = Include(element);

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "contentType")
    {
      contenttype_ = attribute.value();

    }
  }
}

std::string AttachmentData::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("contentType", contenttype_)+">" + onvif::ToXmlClass("xop:Include", include_) + "</"+name+">");
}

bool AttachmentData::operator==(const AttachmentData& rhs) const
{
  return ((include_ == rhs.include_) && (contenttype_ == rhs.contenttype_));
}

}
