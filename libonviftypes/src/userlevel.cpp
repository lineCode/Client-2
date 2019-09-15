// userlevel.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<USERLEVEL> GetUserLevel(const std::string& userlevel)
{
  if (boost::iequals(userlevel, "Administrator"))
  {

    return USERLEVEL_ADMINISTRATOR;
  }
  else if (boost::iequals(userlevel, "Operator"))
  {

    return USERLEVEL_OPERATOR;
  }
  else if (boost::iequals(userlevel, "User"))
  {

    return USERLEVEL_USER;
  }
  else if (boost::iequals(userlevel, "Anonymous"))
  {

    return USERLEVEL_ANONYMOUS;
  }
  else if (boost::iequals(userlevel, "Extended"))
  {

    return USERLEVEL_EXTENDED;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(USERLEVEL userlevel)
{
  switch (userlevel)
  {
    case USERLEVEL_ADMINISTRATOR:
    {
    
      return std::string("Administrator");
    }
    case USERLEVEL_OPERATOR:
    {
    
      return std::string("Operator");
    }
    case USERLEVEL_USER:
    {
    
      return std::string("User");
    }
    case USERLEVEL_ANONYMOUS:
    {
    
      return std::string("Anonymous");
    }
    case USERLEVEL_EXTENDED:
    {
    
      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid USERLEVEL");
    }
  }
}

}
