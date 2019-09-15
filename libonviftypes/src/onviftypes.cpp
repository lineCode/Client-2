// types.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <curl/curl.h>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

std::string UrlDecode(const std::string& input)
{
  CURL* curl = curl_easy_init();
  if (curl == nullptr)
  {

    return std::string();
  }

  int size = 0;
  char* output = curl_easy_unescape(curl, input.c_str(), static_cast<int>(input.size()), &size);
  if (output == nullptr)
  {

    return std::string();
  }

  const std::string result(output);
  curl_free(output);
  curl_easy_cleanup(curl);
  return result;
}

std::string TrimNamespace(const std::string& text)
{
  size_t pos = text.find(':');
  if ((pos == std::string::npos) || (pos > text.length()))
  {

    return text;
  }

  return text.substr((pos+1), std::string::npos);
}

std::string ToString(const bool value)
{
  return value ? std::string("true") : std::string("false");
}

}
