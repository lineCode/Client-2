// packet.hpp
//

#ifndef ID3JAJ8WG6VYIV2U1HD34EMLA9JT7LHHJR
#define ID3JAJ8WG6VYIV2U1HD34EMLA9JT7LHHJR

///// Includes /////

#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <arpa/inet.h>
#endif

#include <stdint.h>
#include <string>

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

///// Classes /////

#pragma pack(push, 1)
class Packet
{
 public:

  Packet();
  Packet(const bool padding, const bool extension, const uint8_t csrccount, const bool marker, const uint8_t payloadtype, const uint16_t sequencenumber, const uint32_t timestamp, const uint32_t senderssrc);
  ~Packet();

  uint8_t GetVersion() const;
  bool GetPadding() const;
  void SetExtension(const bool extension);
  bool GetExtension() const;
  uint8_t GetCrcCount() const;
  void SetMarker(const bool marker);
  bool GetMarker() const;
  void SetPayloadType(const uint8_t payloadtype);
  uint8_t GetPayloadType() const;
  void IncrementSequenceNumber();
  inline uint16_t GetSequenceNumber() const { return ntohs(sequencenumber_); }
  void SetTimestamp(const uint32_t timestamp) { timestamp_ = ntohl(timestamp); }
  inline uint32_t GetTimestamp() const { return ntohl(timestamp_); }
  inline uint32_t GetSendersSrc() const { return ntohl(ssrc_); }

  unsigned int GetSize() const; // Get the size including the CSRC headers

 private:

  uint8_t header1_;
  uint8_t header2_;
  uint16_t sequencenumber_;
  uint32_t timestamp_;
  uint32_t ssrc_;

};
#pragma pack(pop)

}

}

#endif
