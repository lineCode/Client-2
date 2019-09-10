// testcontinuousfocus.cpp
//

///// Includes /////

#include <continuousfocus.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestContinuousFocus, Compare)
{
  Compare(ContinuousFocus(5.0f));
}

}

}
