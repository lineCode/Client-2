 // testmisccapabilities.cpp
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

TEST(DeviceTestMiscCapabilities, Compare)
{
  Compare(device::MiscCapabilities(device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" })))));
  CompareElement(device::MiscCapabilities(device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" })))));
}

}

}
