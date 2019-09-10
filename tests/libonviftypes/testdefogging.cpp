// testdefogging.cpp
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

TEST(TestDefogging, Compare)
{
  Compare(Defogging(DEFOGGINGMODE_ON, 4.0f));
}

}

}
