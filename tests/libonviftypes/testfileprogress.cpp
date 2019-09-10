// testfileprogress.cpp
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

TEST(RecordingTestFileProgress, Compare)
{
  Compare(FileProgress(std::string("FileName"), 0.5f));
}

}

}
