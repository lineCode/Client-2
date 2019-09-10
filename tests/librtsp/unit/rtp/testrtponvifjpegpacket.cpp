// testrtponvifjpegpacket.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtp/onvifjpegpacket.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtp
{

namespace tests
{

///// Tests /////

TEST(TestRtpONVIFJPEGPacket, Compare)
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
  const uint8_t typespecific = 5;
  const uint32_t fragmentoffset = 6;
  const uint8_t type = 7;
  const uint8_t q = 8;
  const uint8_t width = 9;
  const uint8_t height = 10;
  const ONVIFJPEGPacket onvifjpegpacket(ONVIFPacket(Packet(padding, extension, csrccount, marker, payloadtype, sequencenumber, timestamp, senderssrc), 0, ntptimestamp, c, e, d, cseq), typespecific, fragmentoffset, type, q, width, height);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetPadding(), padding);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetExtension(), extension);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetCrcCount(), csrccount);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetMarker(), marker);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetPayloadType(), payloadtype);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetSequenceNumber(), sequencenumber);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetTimestamp(), timestamp);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetRTPPacket().GetSendersSrc(), senderssrc);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetNTPTimestamp(), ntptimestamp);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetC(), c);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetE(), e);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetD(), d);
  ASSERT_EQ(onvifjpegpacket.GetRTPONVIFPacket().GetCseq(), cseq);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetTypeSpecific(), typespecific);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetFragmentOffset(), fragmentoffset);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetType(), type);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetQ(), q);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetWidth(), width);
  ASSERT_EQ(onvifjpegpacket.GetJPEGHeader().GetHeight(), height);
}

}

}

}
