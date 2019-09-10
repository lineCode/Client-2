// packets.cpp
//

///// Includes /////

#include "rtsp/rtcp/packets.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Methods /////

Packets::Packets()
{

}

Packets::Packets(const std::vector<Packet>& rtcppackets) :
  rtcppackets_(rtcppackets)
{

}

Packets::~Packets()
{

}

std::vector<char> Packets::ToData() const
{
  std::vector<char> result;
  for (const auto& rtcppacket : rtcppackets_)
  {
    std::vector<char> data = rtcppacket.ToData();
    result.insert(result.end(), data.begin(), data.end());
  }

  return result;
}

}

}
