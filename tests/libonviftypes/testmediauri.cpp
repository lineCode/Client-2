// testmediauri.cpp
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

TEST(TestMediaUri, Compare)
{
  Compare(MediaUri(std::string("uri"), true, true, Duration(0, 1, 2, 3, 4, 5, 0)));
}

}

}
