// testwidedynamicrange.cpp
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

TEST(TestWideDynamicMode, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), WIDEDYNAMICMODE_OFF).c_str()));
  ASSERT_EQ(WIDEDYNAMICMODE_OFF, *GetWideDynamicMode(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
