// testeventcapabilities.cpp
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

TEST(TestEventCapabilities, Compare)
{
  Compare(EventCapabilities(std::string("xaddr"), true, true, true));
}

}

}
