// capabilitycategory.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<CAPABILITYCATEGORY> GetCapabilityCategory(const std::string& capability)
{
  if (boost::iequals(capability, "All"))
  {

    return CAPABILITYCATEGORY_ALL;
  }
  else if (boost::iequals(capability, "Analytics"))
  {

    return CAPABILITYCATEGORY_ANALYTICS;
  }
  else if (boost::iequals(capability, "Device"))
  {

    return CAPABILITYCATEGORY_DEVICE;
  }
  else if (boost::iequals(capability, "Events"))
  {

    return CAPABILITYCATEGORY_EVENTS;
  }
  else if (boost::iequals(capability, "Imaging"))
  {

    return CAPABILITYCATEGORY_IMAGING;
  }
  else if (boost::iequals(capability, "Media"))
  {

    return CAPABILITYCATEGORY_MEDIA;
  }
  else if (boost::iequals(capability, "PTZ"))
  {

    return CAPABILITYCATEGORY_PTZ;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(CAPABILITYCATEGORY capability)
{
  switch (capability)
  {
    case CAPABILITYCATEGORY_ALL:
    {
    
      return std::string("All");
    }
    case CAPABILITYCATEGORY_ANALYTICS:
    {
    
      return std::string("Analytics");
    }
    case CAPABILITYCATEGORY_DEVICE:
    {
    
      return std::string("Device");
    }
    case CAPABILITYCATEGORY_EVENTS:
    {
    
      return std::string("Events");
    }
    case CAPABILITYCATEGORY_IMAGING:
    {
    
      return std::string("Imaging>");
    }
    case CAPABILITYCATEGORY_MEDIA:
    {
    
      return std::string("Media");
    }
    case CAPABILITYCATEGORY_PTZ:
    {
    
      return std::string("PTZ");
    }
    default:
    {

      throw std::runtime_error("Invalid CAPABILITYCATEGORY");
    }
  }
}

}
