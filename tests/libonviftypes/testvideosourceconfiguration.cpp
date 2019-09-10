// testvideosourcefonfiguration.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestVideoSourceConfiguration, Compare)
{
  Compare(VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)));
}

}

}
