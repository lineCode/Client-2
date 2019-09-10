// testtrackinformation.cpp
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

TEST(TestTrackInformation, Compare)
{
  Compare(TrackInformation(std::string("1"), TRACKTYPE_VIDEO, std::string("description"), ws::DateTime(1900, 1, 2, 3, 4, 5, 6), ws::DateTime(1901, 1, 2, 3, 4, 5, 6)));
}

}

}
