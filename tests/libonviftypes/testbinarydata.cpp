// testbinarydata.cpp
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

TEST(TestBinaryData, Compare)
{
  Compare(BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")));
}

}

}
