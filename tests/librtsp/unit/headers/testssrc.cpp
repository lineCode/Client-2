// testssrc.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/ssrc.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestSSrc, Compare)
{
  Compare(SSrc(5555));

}

}

}

}
