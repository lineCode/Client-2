// testcapabilities.cpp
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

TEST(EventTestCapabilities, Compare)
{
  Compare(event::Capabilities(true, true, true, 5, 6, true));
  CompareElement(event::Capabilities(true, true, true, 5, 6, true));
}

}

}
