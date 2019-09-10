// onvifpacket.hpp
//

#ifndef IDEOATN9DLAY90X4Y5MDZCIIAIXMKP71X6
#define IDEOATN9DLAY90X4Y5MDZCIIAIXMKP71X6

///// Includes /////

#include "rtsp/rtp/packet.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

///// Classes /////

#pragma pack(push, 1)
class ONVIFPacket
{
 public:

  ONVIFPacket();
  ONVIFPacket(const Packet& rtppacket, const uint16_t addedlength, const uint64_t ntptimestamp, const bool c, const bool e, const bool d, const uint8_t cseq);
  ~ONVIFPacket();

  const Packet& GetRTPPacket() const { return rtppacket_; }
  Packet& GetRTPPacket() { return rtppacket_; }

  void SetNTPTimestamp(const uint64_t ntptimestamp);
  uint64_t GetNTPTimestamp() const;
  bool GetC() const;
  bool GetE() const;
  bool GetD() const;
  uint8_t GetCseq() const;
  
 private:

  Packet rtppacket_;
  uint32_t header_;
  uint64_t ntptimestamp_;
  uint32_t data_; // C(1), E(1), D(1), mbz(5), Cseq(8), padding(16)

};
#pragma pack(pop)

}

}

#endif
