// testfocusstatus20.cpp
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

TEST(TestFocusStatus20, Compare)
{
  Compare(FocusStatus20(5.0f, MOVESTATUS_MOVING, std::string("error")));
}

}

}
