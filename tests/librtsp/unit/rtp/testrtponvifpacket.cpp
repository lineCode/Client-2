// testrtponvifpacket.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtp/onvifpacket.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

namespace tests
{

///// Tests /////

TEST(TestRtpONVIFPacket, Compare)
{
  const bool padding = true;
  const bool extension = false;
  const uint8_t csrccount = 5;
  const bool marker = true;
  const uint8_t payloadtype = 80;
  const uint16_t sequencenumber = 555;
  const uint32_t timestamp = 1234;
  const uint32_t senderssrc = 5678;
  const uint64_t ntptimestamp = 1234567890;
  const bool c = true;
  const bool e = true;
  const bool d = true;
  const uint8_t cseq = 10;
  const ONVIFPacket onvifpacket(Packet(padding, extension, csrccount, marker, payloadtype, sequencenumber, timestamp, senderssrc), 0, ntptimestamp, c, e, d, cseq);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetPadding(), padding);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetExtension(), extension);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetCrcCount(), csrccount);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetMarker(), marker);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetPayloadType(), payloadtype);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetSequenceNumber(), sequencenumber);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetTimestamp(), timestamp);
  ASSERT_EQ(onvifpacket.GetRTPPacket().GetSendersSrc(), senderssrc);
  ASSERT_EQ(onvifpacket.GetNTPTimestamp(), ntptimestamp);
  ASSERT_EQ(onvifpacket.GetC(), c);
  ASSERT_EQ(onvifpacket.GetE(), e);
  ASSERT_EQ(onvifpacket.GetD(), d);
  ASSERT_EQ(onvifpacket.GetCseq(), cseq);
}

}

}

}
