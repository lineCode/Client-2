// testservice.cpp
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

TEST(DeviceTestService, Compare)
{
  Compare(device::Service(std::string("namespace"), std::string("xaddr"), Element("name", { { "i", "j" },{ "k", "l" } }, "text", {}), OnvifVersion(5, 5)));
}

}

}
