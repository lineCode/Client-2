// decoder.h
//

#ifndef IDLIY0KUFUWXZI6TDZ26GRYZ1RUCV1EPVI
#define IDLIY0KUFUWXZI6TDZ26GRYZ1RUCV1EPVI

///// Includes /////

#include <boost/next_prior.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <monocleprotocol/codec_generated.h>
#include <QObject>
#include <QString>
#include <stdint.h>
#include <utility/publickey.hpp>
#include <utility/privatekey.hpp>
#include <vector>

extern "C"
{
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
  #include <libavutil/imgutils.h>
}

#include "imagebuffer.h"

///// Defines /////

#define IMAGEQUEUESIZE 50

///// Globals /////

const unsigned int MAXFRAMEBUFFERSIZE = 10;
const AVPixelFormat AVPIXELFORMAT = AV_PIX_FMT_RGBA;
const int AVALIGNMENT = 32;

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum FRAMEPOSITION
{
  FRAMEPOSITION_START,
  FRAMEPOSITION_INTERMEDIATE,
  FRAMEPOSITION_END
};

///// Enumerations /////

enum DECODERERROR
{
  DECODERERROR_SUCCESS = 0,
  DECODERERROR_INITCODEC,
  DECODERERROR_INITCONTEXT,
  DECODERERROR_RESETCODEC,
  DECODERERROR_INVALIDRTPMAPS,
  DECODERERROR_INVALIDFMTP,
  DECODERERROR_INVALIDPACKETIZATIONMODE,
  DECODERERROR_INVALIDPROFILELEVELID,
  DECODERERROR_INVALIDSPROPPARAMETERSETS,
  DECODERERROR_INVALIDCONFIG,
  DECODERERROR_OPENCODEC
};

///// Prototypes /////

QString ToString(DECODERERROR decodererror);
int32_t FrameDifference(uint16_t frame1, uint16_t frame2);
AVPixelFormat ToFormat(const IMAGEBUFFERTYPE type);

///// Classes /////

class FreeImageBuffers
{
 public:

  FreeImageBuffers();
  virtual ~FreeImageBuffers();
  
  virtual ImageBuffer GetFreeImage() = 0;
  virtual void AddFreeImage(ImageBuffer& imagebuffer) = 0;

};

class SPSCFreeFrameBuffers : public FreeImageBuffers
{
 public:

  SPSCFreeFrameBuffers();
  ~SPSCFreeFrameBuffers();

  void Destroy();

  virtual ImageBuffer GetFreeImage() override;
  virtual void AddFreeImage(ImageBuffer& imagebuffer) override;

 private:

  boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> > queue_;

};

class VectorFreeFrameBuffer : public FreeImageBuffers
{
 public:

  VectorFreeFrameBuffer();
  ~VectorFreeFrameBuffer();

  virtual ImageBuffer GetFreeImage() override;
  virtual void AddFreeImage(ImageBuffer& imagebuffer) override;

 private:

  std::vector<ImageBuffer> queue_;

};

class Decoder : public QObject
{
 Q_OBJECT

 public:

  Decoder(const uint32_t trackid, const uint64_t id, const utility::PublicKey& publickey, CUcontext cudacontext);
  virtual ~Decoder();

  virtual void Destroy();

  virtual void Reset() { avcodec_flush_buffers(context_); }

  virtual monocle::Codec GetCodec() const = 0;
  virtual AVCodecID GetCodecId() const = 0;
  
  uint32_t GetTrackId() const { return trackid_; }
  uint64_t GetId() const { return id_; }

  bool IsInitialised() const { return context_; }

  int GetWidth() const { return context_->width; }
  int GetHeight() const { return context_->height; }

  double GetRtpClockrate() const { return context_->time_base.num; }

 protected:

  ImageBuffer Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers);
  CUcontext GetCUDAContext() const { return cudacontext_; }

  const uint32_t trackid_;
  const uint64_t id_;
  utility::PublicKey publickey_;
  CUcontext cudacontext_;

  AVCodec* codec_;
  AVCodecContext* context_;
  AVBufferRef* hwdevice_;
  AVPacket packet_;
  AVFrame* frame_;
  AVFrame* swsframe_;

  SwsContext* swscontext_;

};

}

#endif
