// packets.hpp
//

#ifndef IDSLP35IOJRPRE307O80LSQKDMNLC7TCOI
#define IDSLP35IOJRPRE307O80LSQKDMNLC7TCOI

///// Includes /////

#include <vector>

#include "packet.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Classes /////

class Packets
{
 public:

  Packets();
  Packets(const std::vector<Packet>& rtcppackets);
  ~Packets();

  std::vector<char> ToData() const;

  std::vector<Packet> rtcppackets_;

};

}

}

#endif
