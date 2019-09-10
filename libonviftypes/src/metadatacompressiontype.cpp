// metadatacompressiontype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<METADATACOMPRESSIONTYPE> GetMetadataCompressionType(const std::string& metadatacompressiontype)
{
  if (boost::iequals(metadatacompressiontype, "None"))
  {

    return METADATACOMPRESSIONTYPE_NONE;
  }
  else if (boost::iequals(metadatacompressiontype, "GZIP"))
  {

    return METADATACOMPRESSIONTYPE_GZIP;
  }
  else if (boost::iequals(metadatacompressiontype, "EXI"))
  {

    return METADATACOMPRESSIONTYPE_EXI;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(METADATACOMPRESSIONTYPE metadatacompressiontype)
{
  switch (metadatacompressiontype)
  {
    case METADATACOMPRESSIONTYPE_NONE:
    {
    
      return std::string("None");
    }
    case METADATACOMPRESSIONTYPE_GZIP:
    {
    
      return std::string("GZIP");
    }
    case METADATACOMPRESSIONTYPE_EXI:
    {
    
      return std::string("EXI");
    }
    default:
    {

      throw std::runtime_error("Invalid METADATACOMPRESSIONTYPE");
    }
  }
}

}
