// testdynamicdnstype.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDynamicDNSType, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), DYNAMICDNSTYPE_CLIENTUPDATES).c_str()));
  ASSERT_EQ(DYNAMICDNSTYPE_CLIENTUPDATES, *GetDynamicDNSType(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
