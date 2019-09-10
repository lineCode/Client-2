// testnoisereduction.cpp
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

TEST(TestNoiseReduction, Compare)
{
  Compare(NoiseReduction(4.0f));
}

}

}
