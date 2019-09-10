// testircutfilterautoboundarytype.cpp
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

TEST(TestIrCutFilterAutoBoundaryType, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), IRCUTFILTERAUTOBOUNDARYTYPE_TOON).c_str()));
  ASSERT_EQ(IRCUTFILTERAUTOBOUNDARYTYPE_TOON, *GetIrCutFilterAutoBoundaryType(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
