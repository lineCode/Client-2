// testserialporttype.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(DeviceIOTestSerialPortType, Compare)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(ToXmlEnum(testing::UnitTest::GetInstance()->current_test_info()->name(), deviceio::SERIALPORTTYPE_RS485HALFDUPLEX).c_str()));
  ASSERT_EQ(deviceio::SERIALPORTTYPE_RS485HALFDUPLEX, *deviceio::GetSerialPortType(doc.child(::testing::UnitTest::GetInstance()->current_test_info()->name()).text().get()));
}

}

}
