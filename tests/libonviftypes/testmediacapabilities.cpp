// testmediacapabilities.cpp
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

TEST(TestMediaCapabilities, Compare)
{
  Compare(MediaCapabilities(std::string("xaddr"), RealTimeStreamingCapabilities(true, true, true), ProfileCapabilities(5)));
}

}

}
