// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& dynamicrecordings, const boost::optional<bool>& dynamictracks, const boost::optional<EncodingTypes>& encoding, const boost::optional<float>& maxrate, const boost::optional<float>& maxtotalrate, const boost::optional<int>& maxrecordings, const boost::optional<int>& maxrecordingjobs, const boost::optional<bool>& options, const boost::optional<bool>& metadatarecording, const boost::optional<StringAttrList>& supportedexportfileformats) :
  dynamicrecordings_(dynamicrecordings),
  dynamictracks_(dynamictracks),
  encoding_(encoding),
  maxrate_(maxrate),
  maxtotalrate_(maxtotalrate),
  maxrecordings_(maxrecordings),
  maxrecordingjobs_(maxrecordingjobs),
  options_(options),
  metadatarecording_(metadatarecording),
  supportedexportfileformats_(supportedexportfileformats)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "DynamicRecordings")
    {
      dynamicrecordings_ = attribute.as_bool();
      
    }
    else if (name == "DynamicTracks")
    {
      dynamictracks_ = attribute.as_bool();

    }
    else if (name == "Encoding")
    {
      encoding_ = EncodingTypes(attribute.value());

    }
    else if (name == "MaxRate")
    {
      maxrate_ = attribute.as_float();

    }
    else if (name == "MaxTotalRate")
    {
      maxtotalrate_ = attribute.as_float();

    }
    else if (name == "MaxRecordings")
    {
      maxrecordings_ = attribute.as_int();

    }
    else if (name == "MaxRecordingJobs")
    {
      maxrecordingjobs_ = attribute.as_int();

    }
    else if (name == "Options")
    {
      options_ = attribute.as_bool();

    }
    else if (name == "MetadataRecording")
    {
      metadatarecording_ = attribute.as_bool();

    }
    else if (name == "SupportedExportFileFormats")
    {
      supportedexportfileformats_ = StringAttrList(attribute.value());

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  std::string encodingtypes;
  if (encoding_.is_initialized())
  {
    encodingtypes = std::string(" Encoding=\"") + encoding_->ToString() + "\"";
  
  }

  std::string supportedexportfileformats;
  if (supportedexportfileformats_.is_initialized())
  {
    supportedexportfileformats = std::string(" SupportedExportFileFormats=\"") + supportedexportfileformats_->ToString() + "\"";
  
  }

  return ("<"+name + ToXmlAttribute("DynamicRecordings", dynamicrecordings_) + ToXmlAttribute("DynamicTracks", dynamictracks_) + encodingtypes + ToXmlAttribute("MaxRate", maxrate_) + ToXmlAttribute("MaxTotalRate", maxtotalrate_) + ToXmlAttribute("MaxRecordings", maxrecordings_) + ToXmlAttribute("MaxRecordingJobs", maxrecordingjobs_) + ToXmlAttribute("Options", options_) + ToXmlAttribute("MetadataRecording", metadatarecording_) + supportedexportfileformats + "/>");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (dynamicrecordings_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DynamicRecordings"), onvif::ToString(*dynamicrecordings_)));

  }

  if (dynamictracks_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DynamicTracks"), onvif::ToString(*dynamictracks_)));

  }

  if (maxrate_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxRate"), std::to_string(*maxrate_)));

  }

  if (encoding_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("Encoding"), encoding_->ToString()));

  }

  if (maxtotalrate_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxTotalRate"), std::to_string(*maxtotalrate_)));

  }

  if (maxrecordings_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxRecordings"), std::to_string(*maxrecordings_)));

  }

  if (maxrecordingjobs_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxRecordingJobs"), std::to_string(*maxrecordingjobs_)));

  }

  if (options_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("Options"), onvif::ToString(*options_)));

  }

  if (metadatarecording_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MetadataRecording"), onvif::ToString(*metadatarecording_)));

  }

  if (supportedexportfileformats_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SupportedExportFileFormats"), supportedexportfileformats_->ToString()));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((dynamicrecordings_ == rhs.dynamicrecordings_) && (dynamictracks_ == rhs.dynamictracks_) && (encoding_ == rhs.encoding_) && (maxrate_ == rhs.maxrate_) && (maxtotalrate_ == rhs.maxtotalrate_) && (maxrecordings_ == rhs.maxrecordings_) && (maxrecordingjobs_ == rhs.maxrecordingjobs_) && (options_ == rhs.options_) && (metadatarecording_ == rhs.metadatarecording_) && (supportedexportfileformats_ == rhs.supportedexportfileformats_));
}

}

}
