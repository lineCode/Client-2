// testinterleaved.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/interleaved.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestInterleaved, Compare)
{
  Compare(Interleaved(5, 6));
  Compare(Interleaved(5, boost::none));
}

}

}

}
