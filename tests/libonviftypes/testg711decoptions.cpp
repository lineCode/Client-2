// testg711decoptions.cpp
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

TEST(TestG711DecOptions, Compare)
{
  Compare(G711DecOptions(IntList({ 4, 5, 6 }), IntList({ 7, 8, 9 })));
}

}

}
