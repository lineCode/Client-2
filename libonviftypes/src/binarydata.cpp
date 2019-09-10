// binarydata.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

BinaryData::BinaryData()
{

}

BinaryData::BinaryData(const std::vector<unsigned char>& data, const boost::optional<std::string>& contenttype) :
  data_(data),
  contenttype_(contenttype)
{

}

BinaryData::BinaryData(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Data")
    {
      data_ = utility::Base64Decode(element.text().get());

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

std::string BinaryData::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("contentType", contenttype_)+">" + onvif::ToXml("tt:Data", boost::optional<std::string>(utility::Base64Encode(data_.data(), data_.size()))) + "</"+name+">");
}

bool BinaryData::operator==(const BinaryData& rhs) const
{
  return ((data_ == rhs.data_) && (contenttype_ == rhs.contenttype_));
}

}
