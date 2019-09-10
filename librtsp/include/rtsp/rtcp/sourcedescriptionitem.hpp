// sourcedescriptionitem.hpp
//

#ifndef IDWPP6V6856HUZ7LDIHG1OT5MOO617BOM6
#define IDWPP6V6856HUZ7LDIHG1OT5MOO617BOM6

///// Includes /////

#include <stdint.h>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace rtcp
{

///// Classes /////

class SourceDescriptionItem
{
 public:

  SourceDescriptionItem();
  SourceDescriptionItem(uint8_t cname, uint8_t length, const char* text);
  ~SourceDescriptionItem();

  int Init(const char* buffer, std::size_t size);
  
  unsigned int Size() const;

  std::vector<char> ToData() const;

  bool operator==(const SourceDescriptionItem& rhs) const;

  uint8_t cname_;
  std::vector<char> text_; // This is UTF-8 and should be processed carefully!

};

}

}

#endif
