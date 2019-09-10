// arrayoffileprogress.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ArrayOfFileProgress::ArrayOfFileProgress()
{

}

ArrayOfFileProgress::ArrayOfFileProgress(const std::vector<FileProgress>& fileprogress) :
  fileprogress_(fileprogress)
{

}

ArrayOfFileProgress::ArrayOfFileProgress(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FileProgress")
    {
      fileprogress_.push_back(FileProgress(element));

    }
  }
}

std::string ArrayOfFileProgress::ToXml(const std::string& name) const
{
  std::string fileprogresss;
  for (const auto fileprogress : fileprogress_)
  {
    fileprogresss += fileprogress.ToXml("tt:FileProgress");

  }

  return ("<"+name+">" + fileprogresss + "</"+name+">");
}

bool ArrayOfFileProgress::operator==(const ArrayOfFileProgress& rhs) const
{
  return std::is_permutation(fileprogress_.begin(), fileprogress_.end(), rhs.fileprogress_.begin(), rhs.fileprogress_.end());
}

}
