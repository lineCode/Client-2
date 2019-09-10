// onvifjpegpacket.cpp
//

///// Includes /////

#include "rtsp/rtp/onvifjpegpacket.hpp"

#include <utility/utility.hpp>

#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <arpa/inet.h>
#endif

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

///// Methods /////

JPEGHEADER::JPEGHEADER() :
  typespecificfragmentoffset_(0),
  type_(0),
  q_(0),
  width_(0),
  height_(0)
{

}

JPEGHEADER::JPEGHEADER(const uint8_t typespecific, const uint32_t fragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height) :
  typespecificfragmentoffset_((static_cast<uint32_t>(typespecific) << 24) | fragmentoffset),
  type_(type),
  q_(q),
  width_(width),
  height_(height)
{

}

uint8_t JPEGHEADER::GetTypeSpecific() const
{
  return ((typespecificfragmentoffset_ & 0b11111111000000000000000000000000) >> 24);
}

void JPEGHEADER::SetFragmentOffset(const uint32_t fragmentoffset)
{
  const uint8_t* tmp = reinterpret_cast<const uint8_t*>(&fragmentoffset);
  if (utility::IsBigEndian())
  {
    typespecificfragmentoffset_ = (typespecificfragmentoffset_ & 0b11111111000000000000000000000000) | ((tmp[2] << 16) | (tmp[1] << 8) | (tmp[0]));

  }
  else
  {
    typespecificfragmentoffset_ = (typespecificfragmentoffset_ & 0b11111111000000000000000000000000) | ((tmp[2] << 16) | (tmp[1] << 8) | (tmp[0]));

  }
}

uint32_t JPEGHEADER::GetFragmentOffset() const
{
  const uint8_t* fragmentoffset = reinterpret_cast<const uint8_t*>(&typespecificfragmentoffset_);
  if (utility::IsBigEndian())
  {
    return (fragmentoffset[2] << 16) | (fragmentoffset[1] << 8) | (fragmentoffset[0]);

  }
  else
  {
    return (fragmentoffset[2] << 16) | (fragmentoffset[1] << 8) | (fragmentoffset[0]);

  }
}

ONVIFJPEGPacket::ONVIFJPEGPacket() :
  header_(0)
{

}

ONVIFJPEGPacket::ONVIFJPEGPacket(const ONVIFPacket& rtponvifpacket, const uint8_t typespecific, const uint32_t fragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height) :
  rtponvifpacket_(rtponvifpacket),
  header_(ntohl(0xFFD80000 | 2)),
  jpegheader_(typespecific, fragmentoffset, type, q, width, height)
{

}

ONVIFJPEGPacket::ONVIFJPEGPacket(const ONVIFPacket& rtponvifpacket, const JPEGHEADER& jpegheader) :
  rtponvifpacket_(rtponvifpacket),
  header_(ntohl(0xFFD80000 | 2)),
  jpegheader_(jpegheader)
{

}

ONVIFJPEGPacket::~ONVIFJPEGPacket()
{

}

}

}
