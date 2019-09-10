// testparitybitlist.cpp
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

TEST(DeviceIOTestParityBitList, Compare)
{
  Compare(deviceio::ParityBitList({ deviceio::PARITYBIT_ODD, deviceio::PARITYBIT_MARK }));
}

}

}
