// packet.cpp
//

///// Includes /////

#include "rtsp/rtp/packet.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

///// Methods /////

Packet::Packet() :
  header1_(0),
  header2_(0),
  sequencenumber_(0),
  timestamp_(0),
  ssrc_(0)
{

}

Packet::Packet(const bool padding, const bool extension, const uint8_t csrccount, const bool marker, const uint8_t payloadtype, const uint16_t sequencenumber, const uint32_t timestamp, const uint32_t senderssrc) :
  header1_(0b10000000 | (padding ? 0b00100000 : 0b00000000) | (extension ? 0b00010000 : 0b00000000) | (csrccount & 0b00001111)),
  header2_((marker ? 0b10000000 : 0b00000000) | payloadtype),
  sequencenumber_(ntohs(sequencenumber)),
  timestamp_(ntohl(timestamp)),
  ssrc_(ntohl(senderssrc))
{

}

Packet::~Packet()
{

}

uint8_t Packet::GetVersion() const
{
  return ((header1_ & 0b10000000) >> 6);
}

bool Packet::GetPadding() const
{
  return (header1_ & 0b00100000) ? true : false;
}

void Packet::SetExtension(const bool extension)
{
  if (extension)
  {
    header1_ |= 0b00010000;

  }
  else
  {
    header1_ &= 0b11101111;

  }
}

bool Packet::GetExtension() const
{
  return (header1_ & 0b00010000) ? true : false;
}

uint8_t Packet::GetCrcCount() const
{
  return (header1_ & 0b00001111);
}

void Packet::SetMarker(const bool marker)
{
  header2_ = (header2_ & 0b01111111) | (marker ? 0b10000000 : 0b00000000);
}

bool Packet::GetMarker() const
{
  return (header2_ & 0b10000000) ? true : false;
}

void Packet::SetPayloadType(const uint8_t payloadtype)
{
  header2_ = (header2_ & 0b10000000) | (payloadtype & 0b01111111);
}

uint8_t Packet::GetPayloadType() const
{
  return (header2_ & 0b01111111);
}

void Packet::IncrementSequenceNumber()
{
  sequencenumber_ = ntohs(ntohs(sequencenumber_) + 1);
}

unsigned int Packet::GetSize() const
{
  return (12 + (GetCrcCount() * 32));
}

}

}
