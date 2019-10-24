// imagecache.cpp
//

///// Includes /////

#include <algorithm>

#include "monocleclient/imagecache.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ImageCache::ImageCache()
{

}

ImageCache::~ImageCache()
{
  Clear();

}

void ImageCache::AddImage(const ImageBuffer& imagebuffer)
{
  cache_.push_back(imagebuffer);

}

ImageBuffer ImageCache::GetImage(const bool forwards, const uint64_t playrequestindex, const int64_t sequencenum)
{
  std::vector<ImageBuffer>::iterator imagebuffer = cache_.end();
  for (std::vector<ImageBuffer>::iterator i = cache_.begin(); i != cache_.end(); ++i)
  {
    if (i->playrequestindex_ != playrequestindex)
    {

      continue;
    }

    if (forwards)
    {
      if ((i->sequencenum_ > sequencenum) && ((imagebuffer == cache_.end()) || ((imagebuffer != cache_.end()) && (i->sequencenum_ < imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
    else
    {
      if ((i->sequencenum_ < sequencenum) && ((imagebuffer == cache_.end()) || ((imagebuffer != cache_.end()) && (i->sequencenum_ > imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
  }

  if (imagebuffer == cache_.end())
  {

    return ImageBuffer();
  }
  else
  {

    return *imagebuffer;
  }
}

ImageBuffer ImageCache::GetImage(const uint64_t playrequestindex, const int64_t sequencenum)
{
  std::vector<ImageBuffer>::const_iterator imagebuffer = std::find_if(cache_.cbegin(), cache_.cend(), [playrequestindex, sequencenum](const ImageBuffer& imagebuffer) { return ((imagebuffer.playrequestindex_ == playrequestindex) && (imagebuffer.sequencenum_ == sequencenum)); });
  if (imagebuffer == cache_.end())
  {

    return ImageBuffer();
  }
  else
  {

    return *imagebuffer;
  }
}

ImageBuffer ImageCache::GetLatestImage(const uint64_t playrequestindex)
{
  if (cache_.empty())
  {

    return ImageBuffer();
  }

  std::sort(cache_.begin(), cache_.end(), [](const ImageBuffer& lhs, const ImageBuffer& rhs)
  {
    if (lhs.playrequestindex_ == rhs.playrequestindex_)
    {

      return (lhs.time_ < rhs.time_);
    }
    else
    {

      return (lhs.playrequestindex_ < rhs.playrequestindex_);
    }
  });
  return cache_.back();
}

ImageBuffer ImageCache::GetLatestImageBySequence(const uint64_t playrequestindex)
{
  std::vector<ImageBuffer>::const_iterator imagebuffer = cache_.end();
  for (std::vector<ImageBuffer>::const_iterator i = cache_.begin(); i != cache_.end(); ++i)
  {
    if (i->playrequestindex_ != playrequestindex)
    {

      continue;
    }

    if (imagebuffer == cache_.end())
    {
      imagebuffer = i;

    }
    else
    {
      if (i->sequencenum_ > imagebuffer->sequencenum_)
      {
        imagebuffer = i;

      }
    }
  }

  if (imagebuffer == cache_.end())
  {

    return ImageBuffer();
  }
  else
  {

    return *imagebuffer;
  }
}

void ImageCache::Clear()
{
  std::for_each(cache_.begin(), cache_.end(), [](ImageBuffer& imagebuffer) { imagebuffer.Destroy(); });
  cache_.clear();
}

}
