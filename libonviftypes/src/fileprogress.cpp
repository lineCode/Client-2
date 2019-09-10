// fileprogress.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FileProgress::FileProgress()
{

}

FileProgress::FileProgress(const boost::optional<std::string>& filename, const boost::optional<float>& progress) :
  filename_(filename),
  progress_(progress)
{

}

FileProgress::FileProgress(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FileName")
    {
      filename_ = element.text().get();

    }
    else if (name == "Progress")
    {
      progress_ = element.text().as_float();

    }
  }
}

std::string FileProgress::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:FileName", filename_) + onvif::ToXml("tt:Progress", progress_) + "</"+name+">");
}

bool FileProgress::operator==(const FileProgress& rhs) const
{
  return ((filename_ == rhs.filename_) && (progress_ == rhs.progress_));
}

}
