// mediatype.hpp
//

#ifndef ID3JMJGA6ZFT4Y4DIQB8UKLIQG0OT8G2MI
#define ID3JMJGA6ZFT4Y4DIQB8UKLIQG0OT8G2MI

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Enumerations /////

enum MEDIATYPE
{
  MEDIATYPE_INVALID = 0,
  MEDIATYPE_AUDIO,
  MEDIATYPE_VIDEO,
  MEDIATYPE_TEXT,
  MEDIATYPE_APPLICATION,
  MEDIATYPE_MESSAGE
};

///// Functions /////

std::string ToString(MEDIATYPE mediatype);
MEDIATYPE MediaTypeFromString(const std::string& mediatype);

}

}

#endif
