// reportblock.cpp
//

///// Includes /////

#include "rtsp/rtcp/reportblock.hpp"

#ifdef _WIN32
 #include <Winsock2.h>
#else
 #include <arpa/inet.h>
#endif

#include <utility/utility.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Methods /////

ReportBlock::ReportBlock() :
  sendersrc_(0),
  fractionlost_(0),
  totalpacketslost_(0),
  sequencenumbercyclescount_(0),
  highestsequencenumberreceived_(0),
  interarrivaljitter_(0),
  lastsenderreporttimestamp_(0),
  delaysincelastsenderreporttimestamp_(0)
{

}

ReportBlock::ReportBlock(uint32_t sendersrc, uint8_t fractionlost, uint32_t totalpacketslost, uint16_t sequencenumbercyclescount, uint16_t highestsequencenumberreceived, uint32_t interarrivaljitter, uint32_t lastsenderreporttimestamp, uint32_t delaysincelastsenderreporttimestamp) :
  sendersrc_(sendersrc),
  fractionlost_(fractionlost),
  totalpacketslost_(totalpacketslost),
  sequencenumbercyclescount_(sequencenumbercyclescount),
  highestsequencenumberreceived_(highestsequencenumberreceived),
  interarrivaljitter_(interarrivaljitter),
  lastsenderreporttimestamp_(lastsenderreporttimestamp),
  delaysincelastsenderreporttimestamp_(delaysincelastsenderreporttimestamp)
{

}

ReportBlock::ReportBlock(const char* buffer)
{
  sendersrc_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[0]));
  fractionlost_ = buffer[4];
  if (utility::IsBigEndian())
  {
    totalpacketslost_ = (buffer[7] << 16) | (buffer[6] << 8) | (buffer[5]);

  }
  else
  {
    totalpacketslost_ = (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]);

  }
  sequencenumbercyclescount_ = ntohs(*reinterpret_cast<const uint16_t*>(&buffer[8]));
  highestsequencenumberreceived_ = ntohs(*reinterpret_cast<const uint32_t*>(&buffer[10]));
  interarrivaljitter_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[12]));
  lastsenderreporttimestamp_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[16]));
  delaysincelastsenderreporttimestamp_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[20]));
}

ReportBlock::~ReportBlock()
{

}

std::vector<char> ReportBlock::ToData() const
{
  std::vector<char> data;
  data.reserve(24);

  const uint32_t sendersrc = htonl(sendersrc_);
  const uint16_t sequencenumbercyclescount = htons(sequencenumbercyclescount_);
  const uint16_t highestsequencenumberreceived = htons(highestsequencenumberreceived_);
  const uint32_t interarrivaljitter = htonl(interarrivaljitter_);
  const uint32_t lastsenderreporttimestamp = htonl(lastsenderreporttimestamp_);
  const uint32_t delaysincelastsenderreporttimestamp = htonl(delaysincelastsenderreporttimestamp_);

  data.push_back(reinterpret_cast<const char*>(&sendersrc)[0]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[1]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[2]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[3]);

  data.push_back(fractionlost_);
  if (utility::IsBigEndian())
  {
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[1]);
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[2]);
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[3]);
  }
  else
  {
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[2]);
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[1]);
    data.push_back(reinterpret_cast<const char*>(&totalpacketslost_)[0]);
  }

  data.push_back(reinterpret_cast<const char*>(&sequencenumbercyclescount)[0]);
  data.push_back(reinterpret_cast<const char*>(&sequencenumbercyclescount)[1]);

  data.push_back(reinterpret_cast<const char*>(&highestsequencenumberreceived)[0]);
  data.push_back(reinterpret_cast<const char*>(&highestsequencenumberreceived)[1]);

  data.push_back(reinterpret_cast<const char*>(&interarrivaljitter)[0]);
  data.push_back(reinterpret_cast<const char*>(&interarrivaljitter)[1]);
  data.push_back(reinterpret_cast<const char*>(&interarrivaljitter)[2]);
  data.push_back(reinterpret_cast<const char*>(&interarrivaljitter)[3]);

  data.push_back(reinterpret_cast<const char*>(&lastsenderreporttimestamp)[0]);
  data.push_back(reinterpret_cast<const char*>(&lastsenderreporttimestamp)[1]);
  data.push_back(reinterpret_cast<const char*>(&lastsenderreporttimestamp)[2]);
  data.push_back(reinterpret_cast<const char*>(&lastsenderreporttimestamp)[3]);

  data.push_back(reinterpret_cast<const char*>(&delaysincelastsenderreporttimestamp)[0]);
  data.push_back(reinterpret_cast<const char*>(&delaysincelastsenderreporttimestamp)[1]);
  data.push_back(reinterpret_cast<const char*>(&delaysincelastsenderreporttimestamp)[2]);
  data.push_back(reinterpret_cast<const char*>(&delaysincelastsenderreporttimestamp)[3]);

  return data;
}

bool ReportBlock::operator==(const ReportBlock& rhs) const
{
  return ((sendersrc_ == rhs.sendersrc_) && (fractionlost_ == rhs.fractionlost_) && (totalpacketslost_ == rhs.totalpacketslost_) && (sequencenumbercyclescount_ == rhs.sequencenumbercyclescount_) && (highestsequencenumberreceived_ == rhs.highestsequencenumberreceived_) && (interarrivaljitter_ == rhs.interarrivaljitter_) && (lastsenderreporttimestamp_ == rhs.lastsenderreporttimestamp_) && (delaysincelastsenderreporttimestamp_ == rhs.delaysincelastsenderreporttimestamp_));
}

}

}
