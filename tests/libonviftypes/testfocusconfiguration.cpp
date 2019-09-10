// testfocusconfiguration.cpp
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

TEST(TestFocusConfiguration, Compare)
{
  Compare(FocusConfiguration(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f));
}

}

}
