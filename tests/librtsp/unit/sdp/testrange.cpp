// testrange.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/range.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

///// Namespaces /////

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestSdpRange, Compare)
{
  Compare(Range(0.0, 5.0));
  Compare(Range(0.0, boost::none));
}

}

}

}
