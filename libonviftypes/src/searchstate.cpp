// searchstate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<SEARCHSTATE> GetSearchState(const std::string& searchstate)
{
  if (boost::iequals(searchstate, "Queued"))
  {

    return SEARCHSTATE_QUEUED;
  }
  else if (boost::iequals(searchstate, "Searching"))
  {

    return SEARCHSTATE_SEARCHING;
  }
  else if (boost::iequals(searchstate, "Completed"))
  {

    return SEARCHSTATE_COMPLETED;
  }
  else if (boost::iequals(searchstate, "Unknown"))
  {

    return SEARCHSTATE_UNKNOWN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(SEARCHSTATE searchstate)
{
  switch (searchstate)
  {
    case SEARCHSTATE_QUEUED:
    {
    
      return std::string("Queued");
    }
    case SEARCHSTATE_SEARCHING:
    {
    
      return std::string("Searching");
    }
    case SEARCHSTATE_COMPLETED:
    {
    
      return std::string("Completed");
    }
    case SEARCHSTATE_UNKNOWN:
    {
    
      return std::string("Unknown");
    }
    default:
    {

      throw std::runtime_error("Invalid SEARCHSTATE");
    }
  }
}

}
