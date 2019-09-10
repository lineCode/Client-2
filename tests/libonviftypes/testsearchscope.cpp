// testsearchscope.cpp
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

TEST(RecordingTestSearchScope, Compare)
{
  Compare(SearchScope({ SourceReference(std::string("type1"), std::string("token1")), SourceReference(std::string("type2"), std::string("token2")) }, { std::string("IncludedRecordings1"), std::string("IncludedRecordings2") }, std::string("RecordingInformationFilter")));
}

}

}
