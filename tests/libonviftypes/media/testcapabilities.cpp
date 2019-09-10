// testcapabilities.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(MediaTestCapabilities, Compare)
{
  Compare(media::Capabilities(ProfileCapabilities(5), StreamingCapabilities(true, true, true, true, true), true, true, true, true, true));
}

}

}
