// testexposureoptions20.cpp
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

TEST(TestExposureOptions20, Compare)
{
  Compare(ExposureOptions20({ EXPOSUREMODE_AUTO, EXPOSUREMODE_MANUAL },
                            { EXPOSUREPRIORITY_LOWNOISE },
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f),
                            FloatRange(5.0f, 6.0f)));
}

}

}
