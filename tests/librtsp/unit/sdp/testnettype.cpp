// testnettype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/nettype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestNetType, ToString)
{
  ASSERT_EQ(std::string(), ToString(NETTYPE_INVALID));
  ASSERT_EQ(std::string("IN"), ToString(NETTYPE_INTERNET));
}

TEST(TestNetType, FromString)
{
  ASSERT_EQ(NETTYPE_INVALID, NetTypeFromString("test"));
  ASSERT_EQ(NETTYPE_INTERNET, NetTypeFromString("IN"));
}

}

}

}
