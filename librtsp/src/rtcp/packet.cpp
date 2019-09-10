// packet.cpp
//

///// Includes /////

#include "rtsp/rtcp/packet.hpp"

#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <arpa/inet.h>
#endif

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Methods /////

Packet::Packet() :
  version_(0),
  padding_(0),
  payloadtype_(0),
  length_(0),
  sendersrc_(0),
  ntptimestamp_(0),
  rtptimestamp_(0),
  senderspacketcount_(0),
  sendersoctetcount_(0)
{

}

Packet::Packet(bool padding, uint32_t sendersrc, uint64_t ntptimestamp, uint32_t rtptimestamp, uint32_t senderspacketcount, uint32_t sendersoctetcount, const std::vector<ReportBlock>& reportblocks) :
  version_(2),
  padding_(padding),
  payloadtype_(RTCPPAYLOADTYPE_SENDERREPORT),
  sendersrc_(sendersrc),
  ntptimestamp_(ntptimestamp),
  rtptimestamp_(rtptimestamp),
  senderspacketcount_(senderspacketcount),
  sendersoctetcount_(sendersoctetcount),
  reportblocks_(reportblocks)
{
  length_ = static_cast<uint16_t>(7 + (reportblocks_.size() * 6));

}

Packet::Packet(bool padding, uint32_t sendersrc, const std::vector<ReportBlock>& reportblocks) :
  version_(2),
  padding_(padding),
  payloadtype_(RTCPPAYLOADTYPE_RECEIVERREPORT),
  sendersrc_(sendersrc),
  ntptimestamp_(0),
  rtptimestamp_(0),
  senderspacketcount_(0),
  sendersoctetcount_(0),
  reportblocks_(reportblocks)
{
  length_ = static_cast<uint16_t>(1 + (reportblocks_.size() * 6));

}

Packet::Packet(bool padding, const std::vector<Chunk>& chunks) :
  version_(2),
  padding_(padding),
  payloadtype_(RTCPPAYLOADTYPE_SOURCEDESCRIPTION),
  sendersrc_(0),
  ntptimestamp_(0),
  rtptimestamp_(0),
  senderspacketcount_(0),
  sendersoctetcount_(0),
  chunks_(chunks)
{
  uint16_t length = 4;
  for (const auto& chunk : chunks_)
  {
    length += chunk.Size();

  }
  length_ = (length / 4) - 1;
}

Packet::Packet(bool padding, std::vector<uint32_t> goodbyesendersrc, const std::vector<char>& goodbyereason) :
  version_(2),
  padding_(padding),
  payloadtype_(RTCPPAYLOADTYPE_GOODBYE),
  sendersrc_(0),
  ntptimestamp_(0),
  rtptimestamp_(0),
  senderspacketcount_(0),
  sendersoctetcount_(0),
  goodbyesendersrc_(goodbyesendersrc),
  goodbyereason_(goodbyereason)
{
  length_ = static_cast<uint16_t>(1 + goodbyesendersrc_.size() + (goodbyereason_.size() / 4) + ((goodbyereason_.size() % 4) ? 1 : 0));

}

int Packet::Init(const char* buffer, std::size_t size)
{
  if (size < 4)
  {

    return 1;
  }

  version_ = (buffer[0] & 0b11000000) >> 6;
  if (version_ != 2)
  {

    return 1;
  }

  padding_ = (buffer[0] & 0b00100000) ? true : false;
  uint8_t reportcount = (buffer[0] & 0b00011111);
  payloadtype_ = buffer[1];
  length_ = ntohs(*reinterpret_cast<const uint16_t*>(&buffer[2]));
  switch (payloadtype_)
  {
    case RTCPPAYLOADTYPE_SENDERREPORT:
    {
      if (size < 28)
      {

        return 1;
      }

      sendersrc_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[4]));
      uint32_t ntptimestamphigh = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[8]));
      uint32_t ntptimestamplow = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[12]));
      ntptimestamp_ = (static_cast<uint64_t>(ntptimestamphigh) << 32) | ntptimestamplow;
      rtptimestamp_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[16]));
      senderspacketcount_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[20]));
      sendersoctetcount_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[24]));

      for (int i = 0; i < reportcount; ++i)
      {
        if ((28 + ((i + 1) * 24)) > size)
        {

          return 1;
        }
        reportblocks_.push_back(ReportBlock(buffer + 28 + (i * 24)));
      }
      break;
    }
    case RTCPPAYLOADTYPE_RECEIVERREPORT:
    {
      if (size < 8)
      {

        return 1;
      }

      sendersrc_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[4]));

      for (int i = 0; i < reportcount; ++i)
      {
        if ((8 + ((i + 1) * 24)) > size)
        {

          return 1;
        }
        reportblocks_.push_back(ReportBlock(buffer + 8 + (i * 24)));
      }
      break;
    }
    case RTCPPAYLOADTYPE_SOURCEDESCRIPTION:
    {
      std::size_t offset = 4;
      while (offset < size)
      {
        Chunk chunk;
        if (chunk.Init(buffer + offset, size - offset))
        {

          return 1;
        }
        chunks_.push_back(chunk);
        offset += chunk.Size();
      }
      break;
    }
    case RTCPPAYLOADTYPE_GOODBYE:
    {
      if ((4 + (4 * reportcount)) > size)
      {

        return 1;
      }

      for (int i = 0; i < reportcount; ++i)
      {
        goodbyesendersrc_.push_back(ntohl(*reinterpret_cast<const uint32_t*>(&buffer[4 + (4 * i)])));

      }

      // To check if there is an optional reason for leaving, we "guess" by checking the length of the packet, and if there is possible room for it
      if (length_ > (1 + reportcount))
      {
        uint8_t goodbyereasonlength = buffer[4 + (4 * reportcount)];
        if ((5 + (4 * reportcount) + goodbyereasonlength) > size)
        {

          return 1;
        }

        for (int i = 0; i < goodbyereasonlength; ++i)
        {
          goodbyereason_.push_back(buffer[5 + (4 * reportcount) + i]);

        }
      }

      break;
    }
    default:
    {

      return 1;
    }
  }

  return 0;
}

Packet::~Packet()
{

}

std::vector<char> Packet::ToData() const
{
  uint8_t reportblocks = 0;
  if (payloadtype_ == RTCPPAYLOADTYPE_SOURCEDESCRIPTION)
  {
    reportblocks = static_cast<uint8_t>(chunks_.size());

  }
  else if (payloadtype_ == RTCPPAYLOADTYPE_GOODBYE)
  {
    reportblocks = static_cast<uint8_t>(goodbyesendersrc_.size());

  }
  else
  {
    reportblocks = static_cast<uint8_t>(reportblocks_.size());

  }

  std::vector<char> data;
  data.push_back(((version_ & 0b00000011) << 6) | (padding_ ? 0b00100000 : 0b00000000) | (reportblocks & 0b00011111));
  data.push_back(static_cast<char>(payloadtype_));

  switch (payloadtype_)
  {
    case RTCPPAYLOADTYPE_SENDERREPORT:
    {
      const uint16_t length = htons(static_cast<uint16_t>(7 + (reportblocks_.size() * 6)));
      data.push_back(reinterpret_cast<const char*>(&length)[0]);
      data.push_back(reinterpret_cast<const char*>(&length)[1]);

      const uint32_t sendersrc = htonl(sendersrc_);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[0]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[1]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[2]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[3]);

      const uint32_t ntptimestamphigh = htonl((ntptimestamp_ & 0xFFFFFFFF00000000) >> 32);
      const uint32_t ntptimestamplow = htonl(ntptimestamp_ & 0xFFFFFFFF);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamphigh)[0]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamphigh)[1]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamphigh)[2]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamphigh)[3]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamplow)[0]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamplow)[1]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamplow)[2]);
      data.push_back(reinterpret_cast<const char*>(&ntptimestamplow)[3]);

      const uint32_t rtptimestamp = htonl(rtptimestamp_);
      data.push_back(reinterpret_cast<const char*>(&rtptimestamp)[0]);
      data.push_back(reinterpret_cast<const char*>(&rtptimestamp)[1]);
      data.push_back(reinterpret_cast<const char*>(&rtptimestamp)[2]);
      data.push_back(reinterpret_cast<const char*>(&rtptimestamp)[3]);

      const uint32_t senderspacketcount = htonl(senderspacketcount_);
      data.push_back(reinterpret_cast<const char*>(&senderspacketcount)[0]);
      data.push_back(reinterpret_cast<const char*>(&senderspacketcount)[1]);
      data.push_back(reinterpret_cast<const char*>(&senderspacketcount)[2]);
      data.push_back(reinterpret_cast<const char*>(&senderspacketcount)[3]);

      const uint32_t sendersoctetcount = htonl(sendersoctetcount_);
      data.push_back(reinterpret_cast<const char*>(&rtptimestamp)[0]);
      data.push_back(reinterpret_cast<const char*>(&sendersoctetcount)[1]);
      data.push_back(reinterpret_cast<const char*>(&sendersoctetcount)[2]);
      data.push_back(reinterpret_cast<const char*>(&sendersoctetcount)[3]);

      for (const auto& reportblock : reportblocks_)
      {
        auto reportblockdata = reportblock.ToData();
        data.insert(data.end(), reportblockdata.begin(), reportblockdata.end());
      }
      break;
    }
    case RTCPPAYLOADTYPE_RECEIVERREPORT:
    {
      const uint16_t length = htons(static_cast<uint16_t>(1 + (reportblocks_.size() * 6)));
      data.push_back(reinterpret_cast<const char*>(&length)[0]);
      data.push_back(reinterpret_cast<const char*>(&length)[1]);

      const uint32_t sendersrc = htonl(sendersrc_);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[0]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[1]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[2]);
      data.push_back(reinterpret_cast<const char*>(&sendersrc)[3]);

      for (const auto& reportblock : reportblocks_)
      {
        auto reportblockdata = reportblock.ToData();
        data.insert(data.end(), reportblockdata.begin(), reportblockdata.end());
      }
      break;
    }
    case RTCPPAYLOADTYPE_SOURCEDESCRIPTION:
    {
      uint16_t length = 4;
      for (const auto& chunk : chunks_)
      {
        length += chunk.Size();

      }
      length = htons(static_cast<uint16_t>((length / 4) - 1));
      data.push_back(reinterpret_cast<const char*>(&length)[0]);
      data.push_back(reinterpret_cast<const char*>(&length)[1]);

      for (const auto& chunk : chunks_)
      {
        auto chunkdata = chunk.ToData();
        data.insert(data.end(), chunkdata.begin(), chunkdata.end());
      }
      break;
    }
    case RTCPPAYLOADTYPE_GOODBYE:
    {
      uint32_t length = static_cast<uint16_t>(1 + goodbyesendersrc_.size() + (goodbyereason_.size() / 4) + ((goodbyereason_.size() % 4) ? 1 : 0));
      unsigned int padding = 4 - (goodbyereason_.size() % 4);
      length = htons(length);
      data.push_back(reinterpret_cast<const char*>(&length)[0]);
      data.push_back(reinterpret_cast<const char*>(&length)[1]);

      for (const auto& goodbyesendersrc : goodbyesendersrc_)
      {
        const uint32_t sendersrc = htonl(goodbyesendersrc);
        data.push_back(reinterpret_cast<const char*>(&sendersrc)[0]);
        data.push_back(reinterpret_cast<const char*>(&sendersrc)[1]);
        data.push_back(reinterpret_cast<const char*>(&sendersrc)[2]);
        data.push_back(reinterpret_cast<const char*>(&sendersrc)[3]);
      }

      if (goodbyereason_.size())
      {
        data.push_back(static_cast<uint8_t>(goodbyereason_.size()));
        for (const auto goodbyereason : goodbyereason_)
        {
          data.push_back(goodbyereason);

        }
      }

      for (unsigned int i = 0; i < padding; ++i)
      {
        data.push_back('\0');

      }

      break;
    }
  }

  return data;
}

bool Packet::operator==(const Packet& rhs) const
{
  return ((version_ == rhs.version_) && (padding_ == rhs.padding_) && (payloadtype_ == rhs.payloadtype_) && (length_ == rhs.length_) && (sendersrc_ == rhs.sendersrc_) && (ntptimestamp_ == rhs.ntptimestamp_) && (rtptimestamp_ == rhs.rtptimestamp_) && (senderspacketcount_ == rhs.senderspacketcount_) && (sendersoctetcount_ == rhs.sendersoctetcount_) && (reportblocks_ == rhs.reportblocks_) && (chunks_ == rhs.chunks_) && (goodbyesendersrc_ == rhs.goodbyesendersrc_) && (goodbyereason_ == rhs.goodbyereason_));
}

}

}
