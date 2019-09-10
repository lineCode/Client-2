// testuserlevel.cpp
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

TEST(TestUserLevel, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), USERLEVEL_ADMINISTRATOR).c_str()));
  ASSERT_EQ(USERLEVEL_ADMINISTRATOR, *GetUserLevel(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
