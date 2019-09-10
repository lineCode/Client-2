// testrtppacket.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtp/packet.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

namespace tests
{

///// Tests /////

TEST(TestRtpPacket, Compare)
{
  const bool padding = true;
  const bool extension = false;
  const uint8_t csrccount = 5;
  const bool marker = true;
  const uint8_t payloadtype = 80;
  const uint16_t sequencenumber = 555;
  const uint32_t timestamp = 1234;
  const uint32_t senderssrc = 5678;
  const Packet packet(padding, extension, csrccount, marker, payloadtype, sequencenumber, timestamp, senderssrc);
  ASSERT_EQ(packet.GetPadding(), padding);
  ASSERT_EQ(packet.GetExtension(), extension);
  ASSERT_EQ(packet.GetCrcCount(), csrccount);
  ASSERT_EQ(packet.GetMarker(), marker);
  ASSERT_EQ(packet.GetPayloadType(), payloadtype);
  ASSERT_EQ(packet.GetSequenceNumber(), sequencenumber);
  ASSERT_EQ(packet.GetTimestamp(), timestamp);
  ASSERT_EQ(packet.GetSendersSrc(), senderssrc);
}

TEST(TestRtpPacket, SetPayload)
{
  Packet packet(true, true, 1, true, 1, 1, 1, 1);
  packet.SetPayloadType(2);
  ASSERT_EQ(2, packet.GetPayloadType());
}

}

}

}
