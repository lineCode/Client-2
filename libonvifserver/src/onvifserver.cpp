// onvifservice.hpp
//

///// Includes /////

#include "onvifserver/onvifserver.hpp"

#ifdef _WIN32
  #include <Windows.h>
#endif

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

const std::string ONVIF_LOGGER = "onvif";

///// Prototypes /////

int Init()
{
#ifdef _WIN32
  WSADATA WSAData;
  if (WSAStartup(0x101, &WSAData))
  {
  
    return 1;
  }
#endif
  return 0;
}

int Destroy()
{
#ifdef _WIN32
  if (WSACleanup())
  {

    return 1;
  }
#endif
  return 0;
}

}

}
