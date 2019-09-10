// testserialport.cpp
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

TEST(DeviceIOTestSerialPort, Compare)
{
  Compare(deviceio::SerialPort(std::string("token")));
}

}

}
