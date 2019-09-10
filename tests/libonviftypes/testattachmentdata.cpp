// testattachmentdata.cpp
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

TEST(TestAttachmentData, Compare)
{
  Compare(AttachmentData(Include(std::string("href"), std::string("data")), std::string("contenttype")));
}

}

}
