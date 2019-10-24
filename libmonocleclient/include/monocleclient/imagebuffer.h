// imagebuffer.h
//

#ifndef IDLIY0KUASD3ASDHSADSDSXXXRUCV1EPVI
#define IDLIY0KUASD3ASDHSADSDSXXXRUCV1EPVI

///// Includes /////

#include <array>
#include <boost/optional.hpp>
#include <cuda.h>
#include <monocleprotocol/codec_generated.h>
#include <stdint.h>

///// Declarations /////



///// Namespaces /////

namespace client
{

///// Eumerations /////

enum IMAGEBUFFERTYPE
{
  IMAGEBUFFERTYPE_INVALID,
  IMAGEBUFFERTYPE_TEXT,
  IMAGEBUFFERTYPE_RGBA,
  IMAGEBUFFERTYPE_YUV,
  IMAGEBUFFERTYPE_NV12
};

///// Classes /////

class ImageBuffer
{
 public:

  ImageBuffer();
  ImageBuffer(const ImageBuffer& imagebuffer);
  ~ImageBuffer();

  void Destroy();

  uint64_t playrequestindex_;
  monocle::Codec codec_;
  IMAGEBUFFERTYPE type_;
  std::array<uint8_t*, 3> data_;
  uint8_t* buffer_;
  std::array<int, 3> widths_;
  std::array<int, 3> heights_;
  std::array<int, 3> strides_;
  size_t originalsize_; // The size received down the pipe
  uint64_t time_;
  int64_t sequencenum_;
  boost::optional<bool> digitallysigned_; // boost::none represents no attempt at a digital signature, true is verified, false has failed verification
  bool marker_;
  CUcontext cudacontext_;
 
};

}

#endif
