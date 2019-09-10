// testdevicecapabilities.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDeviceCapabilities, Compare)
{
  Compare(DeviceCapabilities(std::string("xaddr"),
                             NetworkCapabilities(true, true, true, true, true),
                             SystemCapabilities(true, true, true, true, true, true, { onvif::OnvifVersion(5, 6), onvif::OnvifVersion(7, 8) }, true, true, true, true),
                             IOCapabilities(5, 5),
                             SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, true)));
}

}

}
