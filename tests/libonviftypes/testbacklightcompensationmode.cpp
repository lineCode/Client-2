// testbacklightcompensationmode.cpp
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

TEST(TestBacklightCompensationMode, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), BACKLIGHTCOMPENSATIONMODE_OFF).c_str()));
  ASSERT_EQ(BACKLIGHTCOMPENSATIONMODE_OFF, *GetBacklightCompensationMode(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
