// paritybit.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Functions /////

boost::optional<PARITYBIT> GetParityBit(const std::string& paritybit)
{
  if (boost::iequals(paritybit, "None"))
  {

    return PARITYBIT_NONE;
  }
  else if (boost::iequals(paritybit, "Even"))
  {

    return PARITYBIT_EVEN;
  }
  else if (boost::iequals(paritybit, "Odd"))
  {

    return PARITYBIT_ODD;
  }
  else if (boost::iequals(paritybit, "Mark"))
  {

    return PARITYBIT_MARK;
  }
  else if (boost::iequals(paritybit, "Space"))
  {

    return PARITYBIT_SPACE;
  }
  else if (boost::iequals(paritybit, "Extended"))
  {

    return PARITYBIT_EXTENDED;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(PARITYBIT paritybit)
{
  switch (paritybit)
  {
  case PARITYBIT_NONE:
  {

    return std::string("None");
  }
  case PARITYBIT_EVEN:
  {

    return std::string("Even");
  }
  case PARITYBIT_ODD:
  {

    return std::string("Odd");
  }
  case PARITYBIT_MARK:
  {

    return std::string("Mark");
  }
  case PARITYBIT_SPACE:
  {

    return std::string("Space");
  }
  case PARITYBIT_EXTENDED:
  {

    return std::string("Extended");
  }
  default:
  {

    throw std::runtime_error("Invalid PARITYBIT");
  }
  }
}

}

}
