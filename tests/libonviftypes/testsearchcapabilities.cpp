// testsearchcapabilities.cpp
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

TEST(TestSearchCapabilities, Compare)
{
  Compare(SearchCapabilities(std::string("xaddr"), true));
}

}

}
