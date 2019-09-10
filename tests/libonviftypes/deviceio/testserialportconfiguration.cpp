// testserialportconfiguration.cpp
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

TEST(DeviceIOTestSerialPortConfiguration, Compare)
{
  Compare(deviceio::SerialPortConfiguration(std::string("token"), deviceio::SERIALPORTTYPE_RS485HALFDUPLEX, 5, deviceio::PARITYBIT_ODD, 5, 5.0f));
}

}

}
