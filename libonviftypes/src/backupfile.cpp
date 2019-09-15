// backupfile.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

BackupFile::BackupFile()
{

}

BackupFile::BackupFile(const boost::optional<std::string>& name, const boost::optional<AttachmentData>& data) :
  name_(name),
  data_(data)
{

}

BackupFile::BackupFile(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "Data")
    {
      data_ = AttachmentData(element);

    }
  }
}

std::string BackupFile::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Name", name_) + onvif::ToXmlClass("tt:Data", data_) + "</"+name+">");
}

bool BackupFile::operator==(const BackupFile& rhs) const
{
  return ((name_ == rhs.name_) && (data_ == rhs.data_));
}

}
