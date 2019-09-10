// testreplayconfiguration.cpp
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

TEST(ReplayTestReplayConfiguration, Compare)
{
  Compare(replay::ReplayConfiguration(Duration(5, 5, 5, 5, 5, 5, 0)));
}

}

}
