// testrealtimestreamingcapabilities.cpp
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

TEST(TestRealTimeStreamingCapabilities, Compare)
{
  Compare(RealTimeStreamingCapabilities(true, true, true));
}

}

}
