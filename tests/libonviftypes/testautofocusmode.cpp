// testautofocusmode.cpp
//

///// Includes /////

#include <autofocusmode.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestTypes, AutoFocusMode)
{
  pugi::xml_document doc;
  doc.load(ToXml(testing::UnitTest::GetInstance()->current_test_info()->name(), AUTOFOCUSMODE_AUTO).c_str());
  GetAutoFocusMode(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()));
}

}

}
