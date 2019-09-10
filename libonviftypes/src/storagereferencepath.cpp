// storagereferencepath.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

StorageReferencePath::StorageReferencePath()
{

}

StorageReferencePath::StorageReferencePath(const boost::optional<std::string>& storagetoken, const boost::optional<std::string>& relativepath) :
  storagetoken_(storagetoken),
  relativepath_(relativepath)
{

}

StorageReferencePath::StorageReferencePath(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "StorageToken")
    {
      storagetoken_ = element.text().get();

    }
    else if (name == "RelativePath")
    {
      relativepath_ = element.text().get();

    }
  }
}

std::string StorageReferencePath::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:StorageToken", storagetoken_) + onvif::ToXml("tt:RelativePath", relativepath_) + "</"+name+">");
}

bool StorageReferencePath::operator==(const StorageReferencePath& rhs) const
{
  return ((storagetoken_ == rhs.storagetoken_) && (relativepath_ == rhs.relativepath_));
}

}
