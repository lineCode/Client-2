// savevideo.h
//

#ifndef ID2VVC4FDASDSAKDAJ23B4QAES19WKDN9G
#define ID2VVC4FDASDSAKDAJ23B4QAES19WKDN9G

///// Includes /////

#include <chrono>
#include <monocleprotocol/monocleprotocol.hpp>
#include <socket/proxyparams.hpp>
#include <QSharedPointer>
#include <QString>

extern "C"
{
  #include <libavformat/avformat.h>
}

#include "h264decoder.h"
#include "mjpegdecoder.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Recording;

///// Classes /////

// To use this class, instance it, then call Init with appropriate parameters, then pass in H264 and MJPEG frames with AddH264Frame and AddMJPEGFrame
// Destroy then closes the file, make sure to check for failure
class SaveVideo : public QObject
{
 Q_OBJECT

 public:

  SaveVideo();
  ~SaveVideo();

  int Init(const QString& directory, const QString& filename, const int width, const int height, const double framerate);
  int Destroy();

  int AddFrame(const ImageBuffer& imagebuffer, const uint64_t frame);

 private:

  AVOutputFormat* format_;
  AVFormatContext* context_;
  AVCodec* codec_;
  AVStream* videostream_;
  AVCodecContext* cctx_;
  AVFrame* frame_;

};

}

QT_END_NAMESPACE

#endif
