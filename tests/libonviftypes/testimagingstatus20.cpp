// testimagingstatus20.cpp
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

TEST(TestImagingStatus20, Compare)
{
  Compare(ImagingStatus20(FocusStatus20(5.0f, MOVESTATUS_MOVING, std::string("error"))));
}

}

}
