// reportblock.hpp
//

#ifndef ID0S2G2WA0XFMCY3YRU0VM5P12X7INWJQA
#define ID0S2G2WA0XFMCY3YRU0VM5P12X7INWJQA

///// Includes /////

#include <stdint.h>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Classes /////

class ReportBlock
{
 public:

  ReportBlock();
  ReportBlock(uint32_t sendersrc, uint8_t fractionlost, uint32_t totalpacketslost, uint16_t sequencenumbercyclescount, uint16_t highestsequencenumberreceived, uint32_t interarrivaljitter, uint32_t lastsenderreporttimestamp, uint32_t delaysincelastsenderreporttimestamp);
  ReportBlock(const char* buffer);
  ~ReportBlock();
  
  std::vector<char> ToData() const;

  bool operator==(const ReportBlock& rhs) const;

  uint32_t sendersrc_;
  uint8_t fractionlost_;
  uint32_t totalpacketslost_;
  uint16_t sequencenumbercyclescount_;
  uint16_t highestsequencenumberreceived_;
  uint32_t interarrivaljitter_;
  uint32_t lastsenderreporttimestamp_;
  uint32_t delaysincelastsenderreporttimestamp_;

};

}

}

#endif
