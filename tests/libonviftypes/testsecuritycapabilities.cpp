// testsecuritycapabilities.cpp
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

TEST(TestSecurityCapabilities, Compare)
{
  Compare(SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, true));
}

}

}
