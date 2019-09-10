// testmediaprotocol.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/mediaprotocol.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestMediaProtocol, ToString)
{
  ASSERT_EQ(std::string(), ToString(MEDIAPROTOCOL_INVALID));
  ASSERT_EQ(std::string("udp"), ToString(MEDIAPROTOCOL_UDP));
  ASSERT_EQ(std::string("RTP/AVP"), ToString(MEDIAPROTOCOL_RTPAVP));
  ASSERT_EQ(std::string("RTP/SAVP"), ToString(MEDIAPROTOCOL_RTPSAVP));
}

TEST(TestMediaProtocol, FromString)
{
  ASSERT_EQ(MEDIAPROTOCOL_INVALID, MediaProtocolFromString("test"));
  ASSERT_EQ(MEDIAPROTOCOL_UDP, MediaProtocolFromString("udp"));
  ASSERT_EQ(MEDIAPROTOCOL_RTPAVP, MediaProtocolFromString("RTP/AVP"));
  ASSERT_EQ(MEDIAPROTOCOL_RTPSAVP, MediaProtocolFromString("RTP/SAVP"));
}

}

}

}
