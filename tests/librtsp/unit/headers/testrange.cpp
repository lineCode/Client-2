// testrange.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/range.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestHeadersRange, Compare)
{
  Compare(Range("Range: clock=20180424T172517.789Z-"));
  Compare(Range("Range: clock=20180424T172517.123Z-20190525T182618.456"));
  Compare(Range("Range: npt=0.000-"));
  Compare(Range("Range: npt=0.000-1.000"));
  Compare(Range(true, 1000, 5000));
  Compare(Range(true, 0, 5000));
  Compare(Range(true, 0, boost::none));
  Compare(Range(true, 5000, boost::none));
}

}

}

}
