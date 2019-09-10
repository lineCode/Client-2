// testparameters.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/parameters.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestParameters, Compare)
{
  Compare(Parameters({
                       Parameter(std::string("key1"), std::string("value1")),
                       Parameter(std::string("key2"), std::string("value2")),
                       Parameter(std::string("key3"), std::string("value3")),
                       Parameter(std::string("key4"), std::string("value4"))
                     }));
}

}

}

}
