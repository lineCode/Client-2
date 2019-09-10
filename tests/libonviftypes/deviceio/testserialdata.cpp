// testserialdata.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(DeviceIOTestSerialData, Compare)
{
  Compare(deviceio::SerialData(std::string("binary"), std::string("string")));
}

}

}
