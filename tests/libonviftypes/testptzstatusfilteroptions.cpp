// testptzstatusfilteroptions.cpp
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

TEST(TestPTZStatusFilterOptions, Compare)
{
  Compare(PTZStatusFilterOptions(true, false, true, false));
}

}

}
