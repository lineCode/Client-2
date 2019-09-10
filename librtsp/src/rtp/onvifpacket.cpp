// onvifpacket.cpp
//

///// Includes /////

#include "rtsp/rtp/onvifpacket.hpp"

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

ONVIFPacket::ONVIFPacket()
{

}

ONVIFPacket::ONVIFPacket(const Packet& rtppacket, const uint16_t addedlength, const uint64_t ntptimestamp, const bool c, const bool e, const bool d, const uint8_t cseq) :
  rtppacket_(rtppacket),
  header_(ntohl(0xABAC0000 | (addedlength + 3))),
  ntptimestamp_(ntohll(ntptimestamp)),
  data_(ntohl((c ? 0b10000000000000000000000000000000 : 0b00000000000000000000000000000000) | (e ? 0b01000000000000000000000000000000 : 0b00000000000000000000000000000000) | (d ? 0b00100000000000000000000000000000 : 0b00000000000000000000000000000000) | (static_cast<uint32_t>(cseq) << 16)))
{

}

ONVIFPacket::~ONVIFPacket()
{

}

void ONVIFPacket::SetNTPTimestamp(const uint64_t ntptimestamp)
{
  ntptimestamp_ = ntohll(ntptimestamp);
}

uint64_t ONVIFPacket::GetNTPTimestamp() const
{
  return ntohll(ntptimestamp_);
}

bool ONVIFPacket::GetC() const
{
  return (ntohl(data_) & 0b10000000000000000000000000000000);
}

bool ONVIFPacket::GetE() const
{
  return (ntohl(data_) & 0b01000000000000000000000000000000);
}

bool ONVIFPacket::GetD() const
{
  return (ntohl(data_) & 0b00100000000000000000000000000000);
}

uint8_t ONVIFPacket::GetCseq() const
{
  return static_cast<uint8_t>((ntohl(data_) & 0b00000000111111110000000000000000) >> 16);
}

}

}
