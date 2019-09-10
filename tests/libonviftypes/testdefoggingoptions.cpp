// testdefoggingoptions.cpp
//

///// Includes /////

#include <defoggingoptions.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDefoggingOptions, Compare)
{
  Compare(DefoggingOptions({ DEFOGGINGMODE_ON, DEFOGGINGMODE_AUTO }, true));
}

}

}
