// digitalidlestate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DIGITALIDLESTATE> GetDigitalIdleState(const std::string& digitalidlestate)
{
  if (boost::iequals(digitalidlestate, "closed"))
  {

    return DIGITALIDLESTATE_CLOSED;
  }
  else if (boost::iequals(digitalidlestate, "open"))
  {

    return DIGITALIDLESTATE_OPEN;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(DIGITALIDLESTATE digitalidlestate)
{
  switch (digitalidlestate)
  {
    case DIGITALIDLESTATE_CLOSED:
    {

      return std::string("closed");
    }
    case DIGITALIDLESTATE_OPEN:
    {

      return std::string("open");
    }
    default:
    {

      throw std::runtime_error("Invalid DIGITALIDLESTATE");
    }
  }
}

}
