// metadataconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

MetadataConfigurationOptions::MetadataConfigurationOptions()
{
  
}

MetadataConfigurationOptions::MetadataConfigurationOptions(const boost::optional<PTZStatusFilterOptions>& ptzstatusfilteroptions, const std::vector<METADATACOMPRESSIONTYPE>& compressiontype) :
  ptzstatusfilteroptions_(ptzstatusfilteroptions),
  compressiontype_(compressiontype)
{
  
}

MetadataConfigurationOptions::MetadataConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "PTZStatusFilterOptions")
    {
      ptzstatusfilteroptions_ = PTZStatusFilterOptions(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "CompressionType")
        {
          boost::optional<METADATACOMPRESSIONTYPE> metadatacompressiontype = GetMetadataCompressionType(extension.text().get());
          if (metadatacompressiontype.is_initialized())
          {
            compressiontype_.push_back(*metadatacompressiontype);

          }
        }
      }
    }
  }
}

std::string MetadataConfigurationOptions::ToXml(const std::string& name) const
{
  std::string compressiontypes;
  for (const auto& compressiontype : compressiontype_)
  {
    compressiontypes += onvif::ToXmlEnum("tt:CompressionType", compressiontype);

  }

  return ("<"+name+">" + onvif::ToXmlClass("tt:PTZStatusFilterOptions", ptzstatusfilteroptions_) + std::string("<tt:Extension>") + compressiontypes + "</tt:Extension></" + name + ">");
}

bool MetadataConfigurationOptions::operator==(const MetadataConfigurationOptions& rhs) const
{
  return ((ptzstatusfilteroptions_ == rhs.ptzstatusfilteroptions_) && std::is_permutation(compressiontype_.begin(), compressiontype_.end(), rhs.compressiontype_.begin(), rhs.compressiontype_.end()));
}

}
