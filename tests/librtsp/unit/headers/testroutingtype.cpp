// testroutingtype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/routingtype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestRoutingType, ToString)
{
  ASSERT_EQ(std::string(), ToString(ROUTINGTYPE_INVALID));
  ASSERT_EQ(std::string("unicast"), ToString(ROUTINGTYPE_UNICAST));
  ASSERT_EQ(std::string("multicast"), ToString(ROUTINGTYPE_MULTICAST));
}

TEST(TestRoutingType, FromString)
{
  ASSERT_EQ(ROUTINGTYPE_INVALID, RoutingTypeFromString("test"));
  ASSERT_EQ(ROUTINGTYPE_UNICAST, RoutingTypeFromString("unicast"));
  ASSERT_EQ(ROUTINGTYPE_MULTICAST, RoutingTypeFromString("multicast"));
}

}

}

}
