// testdot11authandmanagementsuite.cpp
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

TEST(TestDot11AuthAndManagementSuite, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), DOT11AUTHANDMANAGEMENTSUITE_DOT1X).c_str()));
  ASSERT_EQ(DOT11AUTHANDMANAGEMENTSUITE_DOT1X, *GetDot11AuthAndManagementSuite(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
