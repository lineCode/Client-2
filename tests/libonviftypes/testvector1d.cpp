// testvector1d.cpp
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

TEST(TestVector1d, Compare)
{
  Compare(Vector1d(5, std::string("space")));
}

}

}
