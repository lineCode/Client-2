// testrtcppacket.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtcp/packet.hpp>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

namespace tests
{

///// Functions /////

void Compare(const Packet& lhs)
{
  const auto data = lhs.ToData();
  Packet rhs;
  ASSERT_FALSE(rhs.Init(data.data(), data.size()));
  ASSERT_EQ(lhs, rhs);
}

///// Tests /////

TEST(TestRtcpPacket, Compare)
{
  Compare(Packet(true, 4, 5, 6, 7, 8, { }));
  Compare(Packet(true, 4, 5, 6, 7, 8, { ReportBlock(1, 2, 3, 4, 5, 6, 7, 8) , ReportBlock(2, 3, 4, 5, 6, 7, 8, 8) }));
  Compare(Packet(true, 4, { }));
  Compare(Packet(true, 4, { ReportBlock(1, 2, 3, 4, 5, 6, 7, 8) , ReportBlock(2, 3, 4, 5, 6, 7, 8, 8) }));
  Compare(Packet(true, {}));
  Compare(Packet(true, { Chunk(1, { }) }));
  Compare(Packet(true, { Chunk(1, { SourceDescriptionItem(1, 2, "ab") }) }));
  Compare(Packet(true, { Chunk(1, { SourceDescriptionItem(1, 2, "ab"), SourceDescriptionItem(1, 3, "abc") }) }));
  Compare(Packet(true, { Chunk(1, { SourceDescriptionItem(1, 2, "ab"), SourceDescriptionItem(1, 3, "abc") }), Chunk(1, { SourceDescriptionItem(1, 2, "cd"), SourceDescriptionItem(1, 3, "def") }), Chunk(1, { SourceDescriptionItem(1, 2, "gh"), SourceDescriptionItem(1, 3, "ghi") }) }));
  Compare(Packet(true, { }, { }));
  Compare(Packet(true, { 1, 2, 3 }, { 't', 'e', 's', 't' }));
}

}

}

}
