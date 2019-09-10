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

TEST(ReplayTestCapabilities, Compare)
{
  Compare(replay::Capabilities(true, FloatAttrList(std::vector<float>({ 5.0f, 6.0f, 7.0f })), true));
  CompareElement(replay::Capabilities(true, FloatAttrList(std::vector<float>({ 5.0f, 6.0f, 7.0f })), true));
}

}

}
