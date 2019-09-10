// chunk.hpp
//

#ifndef IDQA61BZPS97ZJTI3MKB8G9IYUPAXJ4H4H
#define IDQA61BZPS97ZJTI3MKB8G9IYUPAXJ4H4H

///// Includes /////

#include <stdint.h>
#include <vector>

#include "sourcedescriptionitem.hpp"

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Classes /////

class Chunk
{
 public:

  Chunk();
  Chunk(uint32_t sendersrc, const std::vector<SourceDescriptionItem>& sourcedescriptionitems);
  ~Chunk();

  int Init(const char* buffer, std::size_t size);
  
  unsigned int Size() const;

  std::vector<char> ToData() const;

  bool operator==(const Chunk& rhs) const;

  uint32_t sendersrc_;
  std::vector<SourceDescriptionItem> sourcedescriptionitems_;

};

}

}

#endif
