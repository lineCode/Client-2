// sourcedescriptionitem.cpp
//

///// Includes /////

#include "rtsp/rtcp/sourcedescriptionitem.hpp"

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

SourceDescriptionItem::SourceDescriptionItem() :
  cname_(0)
{

}

SourceDescriptionItem::SourceDescriptionItem(uint8_t cname, uint8_t length, const char* text) :
  cname_(cname)
{
  if (text == nullptr)
  {

    return;
  }

  for (int i = 0; i < length; ++i)
  {
    text_.push_back(text[i]);

  }
}

SourceDescriptionItem::~SourceDescriptionItem()
{

}

int SourceDescriptionItem::Init(const char* buffer, std::size_t size)
{
  cname_ = 0;
  text_.clear();

  if (size < 1)
  {

    return 1;
  }

  cname_ = buffer[0];

  if (cname_ == 0)
  {

    return 0;
  }

  if (size < 2)
  {

    return 1;
  }

  uint8_t length = buffer[1];

  if (size < (length + 2))
  {

    return 1;
  }

  for (int i = 0; i < length; ++i)
  {
    text_.push_back(buffer[i + 2]);

  }

  return 0;
}

unsigned int SourceDescriptionItem::Size() const
{
  return (static_cast<unsigned int>(text_.size()) + 2);
}

std::vector<char> SourceDescriptionItem::ToData() const
{
  std::vector<char> data;
  data.reserve(Size());
  data.push_back(cname_);
  data.push_back(static_cast<uint8_t>(text_.size()));
  data.insert(data.end(), text_.begin(), text_.end());
  return data;
}

bool SourceDescriptionItem::operator==(const SourceDescriptionItem& rhs) const
{
  return ((cname_ == rhs.cname_) && (text_ == rhs.text_));
}

}

}
