// testg726decoptions.cpp
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

TEST(TestG726DecOptions, Compare)
{
  Compare(G726DecOptions(IntList({ 4, 5, 6 }), IntList({ 7, 8, 9 })));
}

}

}
