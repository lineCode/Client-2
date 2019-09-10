// testrecordingsummary.cpp
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

TEST(TestRecordingSummary, Compare)
{
  Compare(RecordingSummary(ws::DateTime(1900, 1, 2, 3, 4, 5, 6), ws::DateTime(1901, 1, 2, 3, 4, 5, 6), 5));
}

}

}
