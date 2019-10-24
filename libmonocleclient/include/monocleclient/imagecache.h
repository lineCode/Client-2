// imagecache.h
//

#ifndef IDLIY0KUASD343243SDSDSDZZRUCV1EPVI
#define IDLIY0KUASD343243SDSDSDZZRUCV1EPVI

///// Includes /////

#include <vector>

#include "imagebuffer.h"

///// Declarations /////



///// Namespaces /////

namespace client
{

///// Classes /////

class ImageCache
{
 public:

  ImageCache();
  ~ImageCache();

  void AddImage(const ImageBuffer& imagebuffer);
  ImageBuffer GetImage(const bool forwards, const uint64_t playrequestindex, const int64_t sequencenum);
  ImageBuffer GetImage(const uint64_t playrequestindex, const int64_t sequencenum);
  ImageBuffer GetLatestImage(const uint64_t playrequestindex);
  ImageBuffer GetLatestImageBySequence(const uint64_t playrequestindex);
  void Clear();
  
 private:

  std::vector<ImageBuffer> cache_;
 
};

}

#endif
