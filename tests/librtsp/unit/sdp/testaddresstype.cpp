// testaddresstype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/addresstype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestAddressType, ToString)
{
  ASSERT_EQ(std::string(), ToString(ADDRESSTYPE_INVALID));
  ASSERT_EQ(std::string("IP4"), ToString(sdp::ADDRESSTYPE_IP4));
  ASSERT_EQ(std::string("IP6"), ToString(sdp::ADDRESSTYPE_IP6));
}

TEST(TestAddressType, FromString)
{
  ASSERT_EQ(ADDRESSTYPE_INVALID, AddressTypeFromString("test"));
  ASSERT_EQ(ADDRESSTYPE_IP4, AddressTypeFromString("IP4"));
  ASSERT_EQ(ADDRESSTYPE_IP6, AddressTypeFromString("IP6"));
}

}

}

}
