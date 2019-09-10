// testreportblock.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtcp/reportblock.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

namespace tests
{

///// Tests /////

TEST(TestReportBlock, Compare)
{
  ReportBlock reportblock(1, 2, 3, 4, 5, 6, 7, 8);
  ASSERT_EQ(reportblock, ReportBlock(reportblock.ToData().data()));
}

}

}

}
