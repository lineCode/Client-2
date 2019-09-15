// serialporttype.cpp
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

boost::optional<SERIALPORTTYPE> GetSerialPortType(const std::string& serialporttype)
{
  if (boost::iequals(serialporttype, "RS232"))
  {

    return SERIALPORTTYPE_RS232;
  }
  else if (boost::iequals(serialporttype, "RS422HalfDuplex"))
  {

    return SERIALPORTTYPE_RS422HALFDUPLEX;
  }
  else if (boost::iequals(serialporttype, "RS422FullDuplex"))
  {

    return SERIALPORTTYPE_RS422FULLDUPLEX;
  }
  else if (boost::iequals(serialporttype, "RS485HalfDuplex"))
  {

    return SERIALPORTTYPE_RS485HALFDUPLEX;
  }
  else if (boost::iequals(serialporttype, "RS485FullDuplex"))
  {

    return SERIALPORTTYPE_RS485FULLDUPLEX;
  }
  else if (boost::iequals(serialporttype, "Generic"))
  {

    return SERIALPORTTYPE_GENERIC;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(SERIALPORTTYPE serialporttype)
{
  switch (serialporttype)
  {
  case SERIALPORTTYPE_RS232:
  {

    return std::string("RS232");
  }
  case SERIALPORTTYPE_RS422HALFDUPLEX:
  {

    return std::string("RS422HalfDuplex");
  }
  case SERIALPORTTYPE_RS422FULLDUPLEX:
  {

    return std::string("RS422FullDuplex");
  }
  case SERIALPORTTYPE_RS485HALFDUPLEX:
  {

    return std::string("RS485HalfDuplex");
  }
  case SERIALPORTTYPE_RS485FULLDUPLEX:
  {

    return std::string("RS485FullDuplex");
  }
  case SERIALPORTTYPE_GENERIC:
  {

    return std::string("Generic");
  }
  default:
  {

    throw std::runtime_error("Invalid SERIALPORTTYPE");
  }
  }
}

}

}
