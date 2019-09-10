// testbandwidthtype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/bandwidthtype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestBandwidthType, ToString)
{
  ASSERT_EQ(std::string(), ToString(BANDWIDTHTYPE_INVALID));
  ASSERT_EQ(std::string("AS"), ToString(BANDWIDTHTYPE_AS));
  ASSERT_EQ(std::string("CT"), ToString(BANDWIDTHTYPE_CT));
}

TEST(TestBandwidthType, FromString)
{
  ASSERT_EQ(BANDWIDTHTYPE_INVALID, BandwidthTypeFromString("test"));
  ASSERT_EQ(BANDWIDTHTYPE_AS, BandwidthTypeFromString("AS"));
  ASSERT_EQ(BANDWIDTHTYPE_CT, BandwidthTypeFromString("CT"));
}

}

}

}
