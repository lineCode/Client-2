// testtransportprotocol.cpp
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

TEST(TestTransportProtocol, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), TRANSPORTPROTOCOL_HTTP).c_str()));
  ASSERT_EQ(TRANSPORTPROTOCOL_HTTP, *GetTransportProtocol(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
