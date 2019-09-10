// testsourcedescriptionitem.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtcp/sourcedescriptionitem.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

namespace tests
{

///// Functions /////

void Compare(const SourceDescriptionItem& lhs)
{
  auto data = lhs.ToData();
  SourceDescriptionItem rhs;
  ASSERT_FALSE(rhs.Init(data.data(), data.size()));
  ASSERT_EQ(lhs, rhs);
}

///// Tests /////

TEST(TestSourceDescriptionItem, Compare)
{
  Compare(SourceDescriptionItem(1, 10, "1234567890"));
  Compare(SourceDescriptionItem(1, 0, nullptr));
}

}

}

}
