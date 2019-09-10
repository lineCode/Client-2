// testprotocoltype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/protocoltype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestProtocolType, ToString)
{
  ASSERT_EQ(std::string(), ToString(PROTOCOLTYPE_INVALID));
  ASSERT_EQ(std::string("TCP"), ToString(PROTOCOLTYPE_TCP));
  ASSERT_EQ(std::string("UDP"), ToString(PROTOCOLTYPE_UDP));
}

TEST(TestProtocolType, FromString)
{
  ASSERT_EQ(PROTOCOLTYPE_INVALID, ProtocolTypeFromString("test"));
  ASSERT_EQ(PROTOCOLTYPE_TCP, ProtocolTypeFromString("TCP"));
  ASSERT_EQ(PROTOCOLTYPE_UDP, ProtocolTypeFromString("UDP"));
}

}

}

}
