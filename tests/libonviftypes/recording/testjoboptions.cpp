// testjoboptions.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(RecordingTestJobOptions, Compare)
{
  Compare(recording::JobOptions(5, StringAttrList(std::vector<std::string>({ "test1", "test2" }))));
}

}

}
