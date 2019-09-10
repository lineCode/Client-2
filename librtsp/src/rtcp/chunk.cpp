// chunk.cpp
//

///// Includes /////

#include "rtsp/rtcp/chunk.hpp"

#ifdef _WIN32
 #include <Winsock2.h>
#else
 #include <arpa/inet.h>
#endif

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Methods /////

Chunk::Chunk() :
  sendersrc_(0)
{

}

Chunk::Chunk(uint32_t sendersrc, const std::vector<SourceDescriptionItem>& sourcedescriptionitems) :
  sendersrc_(sendersrc),
  sourcedescriptionitems_(sourcedescriptionitems)
{

}

Chunk::~Chunk()
{

}

int Chunk::Init(const char* buffer, std::size_t size)
{
  if (size < 4)
  {

    return 1;
  }

  sendersrc_ = ntohl(*reinterpret_cast<const uint32_t*>(&buffer[0]));

  std::size_t offset = 4;
  while (offset < size)
  {
    SourceDescriptionItem sourcedescriptionitem;
    if (sourcedescriptionitem.Init(buffer + offset, size - offset))
    {

      return 1;
    }

    if (sourcedescriptionitem.cname_ == 0)
    {

      break;
    }

    sourcedescriptionitems_.push_back(sourcedescriptionitem);

    offset += sourcedescriptionitem.Size();
  }

  return 0;
}

unsigned int Chunk::Size() const
{
  unsigned int size = 5;
  for (const auto& sourcedescriptionitem : sourcedescriptionitems_)
  {
    size += sourcedescriptionitem.Size();

  }
  return size + (4 - (size % 4));
}

std::vector<char> Chunk::ToData() const
{
  std::vector<char> data;

  const uint32_t sendersrc = htonl(sendersrc_);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[0]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[1]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[2]);
  data.push_back(reinterpret_cast<const char*>(&sendersrc)[3]);

  for (const auto& sourcedescriptionitem : sourcedescriptionitems_)
  {
    const auto sourcedescriptionitemdata = sourcedescriptionitem.ToData();
    data.insert(data.end(), sourcedescriptionitemdata.begin(), sourcedescriptionitemdata.end());
  }

  data.push_back(0); // rfc 1889: "The list of items in each chunk is terminated by one or more null octets" to fill the 32 bit word

  const unsigned int padding = 4 - (data.size() % 4);
  for (unsigned int i = 0; i < padding; ++i)
  {
    data.push_back(0);

  }

  return data;
}

bool Chunk::operator==(const Chunk& rhs) const
{
  return ((sendersrc_ == rhs.sendersrc_) && (sourcedescriptionitems_ == rhs.sourcedescriptionitems_));
}

}

}
