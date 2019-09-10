// clientsdk.cpp
//

///// Includes /////

#include "onvifclient/onvifclient.hpp"

#include <curl/curl.h>

///// Namespaces /////

namespace onvif
{

///// Functions /////

int Init()
{
  if (curl_global_init(CURL_GLOBAL_ALL))
  {

    return 1;
  }

  return 0;
}

int Destroy()
{
  curl_global_cleanup();
  return 0;
}

}
