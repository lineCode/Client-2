// testbackup.cpp
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

TEST(TestBackup, Compare)
{
  Compare(BackupFile(std::string("name"), AttachmentData(Include(std::string("href"), std::string("data")), std::string("contenttype"))));
}

}

}
