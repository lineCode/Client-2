// testcapabilities.cpp
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

TEST(PTZTestCapabilities, Compare)
{
  Compare(ptz::Capabilities(true, true, true, true, true));
}

}

}
