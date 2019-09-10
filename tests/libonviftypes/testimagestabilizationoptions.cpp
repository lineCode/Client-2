// testimagestabilizationoptions.cpp
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

TEST(TestImageStabilizationOptions, Compare)
{
  Compare(ImageStabilizationOptions({ IMAGESTABILIZATIONMODE_ON, IMAGESTABILIZATIONMODE_AUTO }, FloatRange(5.0f, 6.0f)));
}

}

}
