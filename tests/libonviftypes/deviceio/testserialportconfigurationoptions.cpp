// testserialportconfigurationoptions.cpp
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

TEST(DeviceIOTestSerialPortConfigurationOptions, Compare)
{
  Compare(deviceio::SerialPortConfigurationOptions(IntList({ 5, 6, 7 }), deviceio::ParityBitList({ deviceio::PARITYBIT_MARK, deviceio::PARITYBIT_ODD }), IntList({ 5, 6, 7 }), FloatList({ 5.0f, 6.0f, 7.0f }), std::string("token")));
}

}

}
