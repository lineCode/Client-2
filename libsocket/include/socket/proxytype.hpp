// proxytype.hpp
//

#ifndef IDSQJLSLGTQSRYAHLGLGHRSULOVWITYESQ
#define IDSQJLSLGTQSRYAHLGLGHRSULOVWITYESQ

///// Includes /////

#include <string>

///// Namespaces /////

namespace sock
{

///// Enumerations /////

enum PROXYTYPE
{
  PROXYTYPE_NONE,
  PROXYTYPE_HTTP,
  PROXYTYPE_SOCKS5
};

///// Prototypes /////

std::string ToString(PROXYTYPE proxytype);
PROXYTYPE FromString(const std::string& proxytype);
PROXYTYPE FromInt(int proxytype);

}

#endif
