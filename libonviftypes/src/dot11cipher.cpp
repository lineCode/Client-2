// dot11cipher.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DOT11CIPHER> GetDot11Cipher(const std::string& dot11cipher)
{
  if (boost::iequals(dot11cipher, "CCMP"))
  {

    return DOT11CIPHER_CCMP;
  }
  else if (boost::iequals(dot11cipher, "TKIP"))
  {

    return DOT11CIPHER_TKIP;
  }
  else if (boost::iequals(dot11cipher, "Any"))
  {

    return DOT11CIPHER_ANY;
  }
  else if (boost::iequals(dot11cipher, "Extended"))
  {

    return DOT11CIPHER_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(DOT11CIPHER dot11cipher)
{
  switch (dot11cipher)
  {
    case DOT11CIPHER_CCMP:
    {

      return std::string("CCMP");
    }
    case DOT11CIPHER_TKIP:
    {

      return std::string("TKIP");
    }
    case DOT11CIPHER_ANY:
    {

      return std::string("Any");
    }
    case DOT11CIPHER_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid DOT11CIPHER");
    }
  }
}

}
