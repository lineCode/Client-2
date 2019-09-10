// packet.hpp
//

#ifndef IDPOAPFX2BJQK3D0AS06OZFEQKR6PXI0E5
#define IDPOAPFX2BJQK3D0AS06OZFEQKR6PXI0E5

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

#include "chunk.hpp"
#include "reportblock.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Globals /////

const uint8_t RTCPPAYLOADTYPE_INVALID = 0;
const uint8_t RTCPPAYLOADTYPE_SENDERREPORT = 200;
const uint8_t RTCPPAYLOADTYPE_RECEIVERREPORT = 201;
const uint8_t RTCPPAYLOADTYPE_SOURCEDESCRIPTION = 202;
const uint8_t RTCPPAYLOADTYPE_GOODBYE = 203;

///// Classes /////

class Packet
{
 public:

  Packet();
  Packet(bool padding, uint32_t sendersrc, uint64_t ntptimestamp, uint32_t rtptimestamp, uint32_t senderspacketcount, uint32_t sendersoctetcount, const std::vector<ReportBlock>& reportblocks);
  Packet(bool padding, uint32_t sendersrc, const std::vector<ReportBlock>& reportblocks);
  Packet(bool padding, const std::vector<Chunk>& chunks);
  Packet(bool padding, std::vector<uint32_t> goodbyesendersrcs, const std::vector<char>& goodbyereason);
  ~Packet();

  int Init(const char* buffer, std::size_t size);

  std::vector<char> ToData() const;

  bool operator==(const Packet& rhs) const;

  uint8_t version_;
  bool padding_;
  uint8_t payloadtype_;
  uint16_t length_;
  uint32_t sendersrc_;
  uint64_t ntptimestamp_;
  uint32_t rtptimestamp_;
  uint32_t senderspacketcount_;
  uint32_t sendersoctetcount_;
  std::vector<ReportBlock> reportblocks_;
  std::vector<Chunk> chunks_;
  std::vector<uint32_t> goodbyesendersrc_;
  std::vector<char> goodbyereason_; // Must not exceed 256 bytes in length, or ToData() will create a corrupted packet

};

}

}

#endif
