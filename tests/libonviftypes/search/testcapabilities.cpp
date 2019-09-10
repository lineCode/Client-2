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

TEST(SearchTestCapabilities, Compare)
{
  Compare(search::Capabilities(true, true));
  CompareElement(search::Capabilities(true, true));
}

}

}
