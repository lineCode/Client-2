// testdigitalinput.cpp
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

TEST(TestDigitalInput, Compare)
{
  Compare(DigitalInput(std::string("token")));
}

}

}
