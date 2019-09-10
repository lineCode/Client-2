// testircutfilterautoadjustmentoptions.cpp
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

TEST(TestIrCutFilterAutoAdjustmentOptions, Compare)
{
  Compare(IrCutFilterAutoAdjustmentOptions({ IRCUTFILTERAUTOBOUNDARYTYPE_TOON, IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF }, true, DurationRange(Duration(0, 1, 2, 3, 4, 5, 0), Duration(1, 2, 3, 4, 5, 6, 0))));
}

}

}
