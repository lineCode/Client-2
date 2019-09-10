// testchunk.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtcp/chunk.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

namespace tests
{

///// Functions /////

void Compare(const Chunk& lhs)
{
  auto data = lhs.ToData();
  Chunk rhs;
  ASSERT_FALSE(rhs.Init(data.data(), data.size()));
  ASSERT_EQ(lhs, rhs);
}

///// Tests /////

TEST(TestChunk, Compare)
{
  Compare(Chunk(1, { }));
  Compare(Chunk(1, { SourceDescriptionItem(1, 2, "ab") }));
  Compare(Chunk(1, { SourceDescriptionItem(1, 2, "ab"), SourceDescriptionItem(1, 3, "abc") }));
  Compare(Chunk(1, { SourceDescriptionItem(1, 2, "ab"), SourceDescriptionItem(1, 2, "cd"), SourceDescriptionItem(1, 3, "abc"), SourceDescriptionItem(1, 4, "abcd") }));
}

TEST(TestChunk, Size)
{
  Chunk chunk(1, { SourceDescriptionItem(1, 2, "ab"), SourceDescriptionItem(1, 2, "cd"), SourceDescriptionItem(1, 3, "abc"), SourceDescriptionItem(1, 4, "abcd") });
  ASSERT_EQ(chunk.Size(), chunk.ToData().size());
}

}

}

}
