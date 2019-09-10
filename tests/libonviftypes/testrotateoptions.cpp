// testrotateoptions.cpp
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

TEST(TestRotateOptions, Compare)
{
  Compare(RotateOptions({ ROTATE_ON, ROTATE_AUTO }, IntList({ 90, 180 })));
}

}

}
