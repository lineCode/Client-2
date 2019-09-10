// testircutfilterautoadjustment.cpp
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

TEST(TestIrCutFilterAutoAdjustment, Compare)
{
  Compare(IrCutFilterAutoAdjustment(std::string("test"), 4.0f, Duration(0, 1, 2, 3, 4, 5, 0)));
}

}

}
