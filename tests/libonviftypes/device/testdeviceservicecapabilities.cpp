// testdeviceservicecapabilities.cpp
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

TEST(DeviceTestDeviceServiceCapabilities, Compare)
{
  Compare(device::DeviceServiceCapabilities(device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true),
                                            device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5),
                                            device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true),
                                            device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" })))));

  CompareElement(device::DeviceServiceCapabilities(device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true),
                                                   device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5),
                                                   device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true),
                                                   device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" })))));
}

}

}
