// onvifjpegpacket.hpp
//

#ifndef IDEOATN9DLAY90X4Y5MDZCIIAIXMKP71X7
#define IDEOATN9DLAY90X4Y5MDZCIIAIXMKP71X7

///// Includes /////

#include "rtsp/rtp/onvifpacket.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

///// Structures /////

#pragma pack(push, 1)
struct JPEGHEADER
{
  JPEGHEADER();
  JPEGHEADER(const uint8_t typespecific, const uint32_t fragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height);

  uint8_t GetTypeSpecific() const;
  void SetFragmentOffset(const uint32_t fragmentoffset);
  uint32_t GetFragmentOffset() const;
  inline uint8_t GetType() const { return type_; }
  inline uint8_t GetQ() const { return q_; }
  inline uint8_t GetWidth() const { return width_; }
  inline uint8_t GetHeight() const { return height_; }

  uint32_t typespecificfragmentoffset_; // 8bits type-specific, 24 bits fragment offset
  uint8_t type_;
  uint8_t q_;
  uint8_t width_;
  uint8_t height_;

};
#pragma pack(pop)

///// Classes /////

#pragma pack(push, 1)
class ONVIFJPEGPacket
{
 public:

  ONVIFJPEGPacket();
  ONVIFJPEGPacket(const ONVIFPacket& rtponvifpacket, const uint8_t typespecific, const uint32_t fragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height);
  ONVIFJPEGPacket(const ONVIFPacket& rtponvifpacket, const JPEGHEADER& jpegheader);
  ~ONVIFJPEGPacket();

  const ONVIFPacket& GetRTPONVIFPacket() const { return rtponvifpacket_; }
  ONVIFPacket& GetRTPONVIFPacket() { return rtponvifpacket_; }
  const JPEGHEADER& GetJPEGHeader() const { return jpegheader_; }
  JPEGHEADER& GetJPEGHeader() { return jpegheader_; }

 private:

  ONVIFPacket rtponvifpacket_;
  uint32_t header_; // always 0xFF 0xD8 2
  JPEGHEADER jpegheader_;

};
#pragma pack(pop)

}

}

#endif
