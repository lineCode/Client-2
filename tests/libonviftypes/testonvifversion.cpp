// testonvifversion.cpp
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

TEST(TestOnvifVersion, Compare)
{
  Compare(OnvifVersion(5, 4));
}

TEST(TestOnvifVersion, Version)
{
  ASSERT_EQ(utility::Version(5, 4, 3), OnvifVersion(5, 43).GetVersion());
  ASSERT_EQ(utility::Version(5, 0, 4), OnvifVersion(5, 4).GetVersion());
}

}

}
