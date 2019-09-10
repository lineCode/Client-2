 // testsystemcapabilities.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestSystemCapabilities, Compare)
{
  Compare(SystemCapabilities(true, true, true, true, true, true, { onvif::OnvifVersion(5, 6), onvif::OnvifVersion(7, 8) }, true, true, true, true));
}

}

}
