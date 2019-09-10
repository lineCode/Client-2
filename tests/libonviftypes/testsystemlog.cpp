// testsystemlog.cpp
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

TEST(TestSystemLog, Compare)
{
  Compare(SystemLog(AttachmentData(Include(std::string("href"), std::string("data")), std::string("contenttype")), std::string("string")));
}

}

}
