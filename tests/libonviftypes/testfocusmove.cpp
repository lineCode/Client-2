// testfocusmove.cpp
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

TEST(TestFocusMove, Compare)
{
  Compare(FocusMove(AbsoluteFocus(5.0f, 6.0f), RelativeFocus(5.0f, 6.0f), ContinuousFocus(5.0f)));
}

}

}
