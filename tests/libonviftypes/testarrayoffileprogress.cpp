// testarrayoffileprogress.cpp
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

TEST(RecordingTestArrayOfFileProgress, Compare)
{
  Compare(ArrayOfFileProgress({ FileProgress(std::string("FileName1"), 0.5f), FileProgress(std::string("FileName2"), 0.5f) }));
}

}

}
