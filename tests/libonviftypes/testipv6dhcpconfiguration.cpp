// testipv6dhcpconfiguration.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestIPv6DHCPConfiguration, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), IPV6DHCPCONFIGURATION_STATEFUL).c_str()));
  ASSERT_EQ(IPV6DHCPCONFIGURATION_STATEFUL, *GetIPv6DHCPConfiguration(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
