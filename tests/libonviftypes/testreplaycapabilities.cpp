// testreplaycapabilities.cpp
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

TEST(TestReplayCapabilities, Compare)
{
  Compare(ReplayCapabilities(std::string("xaddr")));
}

}

}
