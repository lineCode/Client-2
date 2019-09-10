// testptzcontroldirectionoptions.cpp
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

TEST(TestPTZControlDirectionOptions, Compare)
{
  Compare(PTZControlDirectionOptions(EFLIPMODE_ON, REVERSEMODE_ON));
}

}

}
