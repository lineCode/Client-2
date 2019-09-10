// testinclude.cpp
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

TEST(TestInclude, Compare)
{
  Compare(Include(std::string("href"), std::string("data")));
}

}

}
