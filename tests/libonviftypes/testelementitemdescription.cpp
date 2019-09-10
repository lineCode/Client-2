// testelementitemdescription.cpp
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

TEST(TestElementItemDescription, Compare)
{
  Compare(ElementItemDescription(std::string("name"), std::string("type")));
}

}

}
