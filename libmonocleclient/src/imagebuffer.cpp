// imagebuffer.cpp
//

///// Includes /////

#include "monocleclient/imagebuffer.h"

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ImageBuffer::ImageBuffer() :
  playrequestindex_(0),
  type_(IMAGEBUFFERTYPE_INVALID),
  data_({ nullptr, nullptr, nullptr }),
  buffer_(nullptr),
  widths_({ 0, 0, 0 }),
  heights_({ 0, 0, 0 }),
  strides_({ 0, 0, 0 }),
  originalsize_(0),
  time_(0),
  sequencenum_(0),
  marker_(true),
  cudacontext_(nullptr)
{

}

ImageBuffer::ImageBuffer(const ImageBuffer& imagebuffer) :
  playrequestindex_(imagebuffer.playrequestindex_),
  codec_(imagebuffer.codec_),
  type_(imagebuffer.type_),
  data_(imagebuffer.data_),
  buffer_(imagebuffer.buffer_),
  widths_(imagebuffer.widths_),
  heights_(imagebuffer.heights_),
  strides_(imagebuffer.strides_),
  originalsize_(imagebuffer.originalsize_),
  time_(imagebuffer.time_),
  sequencenum_(imagebuffer.sequencenum_),
  digitallysigned_(imagebuffer.digitallysigned_),
  marker_(imagebuffer.marker_),
  cudacontext_(imagebuffer.cudacontext_)
{

}

ImageBuffer::~ImageBuffer()
{
  // We don't call Destroy here, because it must be done manually when passed through the SPSC queue

}

void ImageBuffer::Destroy()
{
  if (cudacontext_ && (type_ == IMAGEBUFFERTYPE_NV12))
  {
    if (MainWindow::Instance()->cuctxpushcurrent_(cudacontext_) == CUDA_SUCCESS)
    {
      if (data_[0])
      {
        MainWindow::Instance()->cumemfree_(reinterpret_cast<CUdeviceptr>(data_[0]));

      }
      if (data_[1])
      {
        MainWindow::Instance()->cumemfree_(reinterpret_cast<CUdeviceptr>(data_[1]));

      }
      CUcontext dummy;
      MainWindow::Instance()->cuctxpopcurrent_(&dummy);
    }
  }
  else
  {
    if (buffer_)
    {
      delete[] buffer_;

    }
  }

  type_ = IMAGEBUFFERTYPE_INVALID;
  data_.fill(nullptr);
  widths_.fill(0);
  heights_.fill(0);
  strides_.fill(0);
  originalsize_ = 0;
  buffer_ = nullptr;
  cudacontext_ = nullptr;
}

}
