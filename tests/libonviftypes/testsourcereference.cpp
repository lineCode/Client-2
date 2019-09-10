// testsourcereference.cpp
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

TEST(RecordingTestSourceReference, Compare)
{
  Compare(SourceReference(std::string("type"), std::string("token")));
}

}

}
