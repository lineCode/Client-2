// testtiming.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/timing.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestTiming, Compare)
{
  Compare(sdp::Timing(std::vector< std::pair<uint64_t, uint64_t> >({
                                                                     { 0, 0 },
                                                                     { 0, 0 },
                                                                     { 1, 2 },
                                                                     { 0, 4 }
                                                                   })));

}

}

}
