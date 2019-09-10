// testimagestabilization.cpp
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

TEST(TestImageStabilization, Compare)
{
  Compare(ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 4.0f));
}

}

}
