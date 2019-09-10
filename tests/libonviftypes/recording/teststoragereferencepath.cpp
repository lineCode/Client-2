// teststoragereferencepath.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <types.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(RecordingTestStorageReferencePath, Compare)
{
  Compare(recording::StorageReferencePath(std::string("storagetoken"), std::string("relativepath")));
}

}

}
