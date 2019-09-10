// testparameter.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/parameter.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestParameter, Compare)
{
  Compare(Parameter(std::string("key"), std::string("value")));
}

}

}

}
