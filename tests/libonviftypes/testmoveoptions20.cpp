// testmoveoptions20.cpp
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

TEST(TestMoveOptions20, Compare)
{
  Compare(MoveOptions20(AbsoluteFocusOptions(FloatRange(5.0f, 6.0f), FloatRange(7.0f, 8.0f)), RelativeFocusOptions20(FloatRange(5.0f, 6.0f), FloatRange(7.0f, 8.0f)), ContinuousFocusOptions(FloatRange(5.0f, 6.0f))));
}

}

}
