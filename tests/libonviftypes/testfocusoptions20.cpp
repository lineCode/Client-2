// testfocusoptions20.cpp
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

TEST(TestFocusOptions20, Compare)
{
  Compare(FocusOptions20({ AUTOFOCUSMODE_AUTO, AUTOFOCUSMODE_MANUAL },
                         FloatRange(5.0f, 6.0f),
                         FloatRange(5.0f, 6.0f),
                         FloatRange(5.0f, 6.0f)));
}

}

}
