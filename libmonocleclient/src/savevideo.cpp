// savevideo.cpp
//

///// Includes /////

#include "monocleclient/savevideo.h"

#include <boost/algorithm/string.hpp>
#include <boost/scope_exit.hpp>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

SaveVideo::SaveVideo() :
  format_(nullptr),
  context_(nullptr),
  codec_(nullptr),
  videostream_(nullptr),
  cctx_(nullptr),
  frame_(av_frame_alloc())
{

}

SaveVideo::~SaveVideo()
{
  Destroy();
}

int SaveVideo::Init(const QString& directory, const QString& filename, const int width, const int height, const double framerate)
{
  format_ = av_guess_format(nullptr, filename.toStdString().c_str(), nullptr);
  if (!format_)
  {

    return 1;
  }

  context_ = avformat_alloc_context();
  if (!context_)
  {

    return 2;
  }
  context_->oformat = format_;

  videostream_ = avformat_new_stream(context_, nullptr);
  if (!videostream_)
  {

    return 3;
  }

  videostream_->id = 0;
  cctx_ = videostream_->codec;
  cctx_->codec_id = format_->video_codec;
  cctx_->codec_type = AVMEDIA_TYPE_VIDEO;
  cctx_->bit_rate = 1000 * 1000 * 1000;
  cctx_->width = width;
  cctx_->height = height;
  cctx_->time_base.num = 1;
  cctx_->time_base.den = framerate;
  videostream_->time_base.num = 1;
  videostream_->time_base.den = framerate;
  videostream_->codec->time_base.num = 1;
  videostream_->codec->time_base.den = framerate;
  videostream_->codec->gop_size = 5;
  videostream_->codec->pix_fmt = AV_PIX_FMT_YUV420P;

  if (context_->oformat->flags & AVFMT_GLOBALHEADER)
  {
    context_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

  }

  codec_ = avcodec_find_encoder(videostream_->codec->codec_id);
  if (!codec_)
  {

    return 4;
  }

  int ret = avcodec_open2(cctx_, codec_, nullptr);
  if (ret < 0)
  {

    return 5;
  }

  ret = avio_open(&context_->pb, (directory + "/" + filename).toStdString().c_str(), AVIO_FLAG_WRITE);
  if (ret < 0)
  {

    return 6;
  }

  if (avformat_write_header(context_, nullptr))
  {

    return 7;
  }

  return 0;
}

int SaveVideo::Destroy()
{
  if (context_)
  {
    if (av_write_trailer(context_))
    {

      return 1;
    }

    if (context_->pb)
    {
      if (avio_close(context_->pb) < 0)
      {

        return 3;
      }
    }
  }

  if (cctx_)
  {
    if (avcodec_close(cctx_))
    {

      return 2;
    }
    cctx_ = nullptr;
  }

  if (frame_)
  {
    av_frame_free(&frame_);
    frame_ = nullptr;
  }

  if (videostream_)
  {
    if (videostream_->codec)
    {
      avcodec_close(videostream_->codec);
      videostream_->codec = nullptr;
    }
    videostream_ = nullptr;
  }

  if (context_)
  {
    av_free(context_);
    context_ = nullptr;
  }

  return 0;
}

int SaveVideo::AddFrame(const ImageBuffer& imagebuffer, const uint64_t frame)
{
  AVPacket packet;
  av_init_packet(&packet);
  packet.data = nullptr;
  packet.size = 0;
  packet.dts = frame;
  BOOST_SCOPE_EXIT(&packet)
  {
    av_free_packet(&packet);
  } BOOST_SCOPE_EXIT_END

  const AVPixelFormat format = ToFormat(imagebuffer.type_);
  if ((frame_->format != format) || (frame_->width != imagebuffer.widths_[0]) || (frame_->height != imagebuffer.heights_[0]))
  {
    frame_->format = format;
    frame_->width = imagebuffer.widths_[0];
    frame_->height = imagebuffer.heights_[0];
    const int size = avpicture_get_size(format, imagebuffer.strides_[0], imagebuffer.heights_[0]);
    avpicture_free(reinterpret_cast<AVPicture*>(frame_));
    const uint8_t* buffer = static_cast<uint8_t*>(av_malloc(size));
    if (avpicture_fill(reinterpret_cast<AVPicture*>(frame_), buffer, format, imagebuffer.strides_[0], imagebuffer.heights_[0]) < 0)
    {

      return 1;
    }
  }

  if (imagebuffer.type_ == IMAGEBUFFERTYPE_RGBA)
  {
    memcpy(frame_->data[0], imagebuffer.data_[0], imagebuffer.strides_[0] * imagebuffer.heights_[0]);

  }
  else if (imagebuffer.type_ == IMAGEBUFFERTYPE_YUV)
  {
    memcpy(frame_->data[0], imagebuffer.data_[0], imagebuffer.strides_[0] * imagebuffer.heights_[0]);
    memcpy(frame_->data[1], imagebuffer.data_[1], imagebuffer.strides_[1] * imagebuffer.heights_[1]);
    memcpy(frame_->data[2], imagebuffer.data_[2], imagebuffer.strides_[2] * imagebuffer.heights_[2]);
  }
  else
  {

    return 2;
  }

  int output = 0;
  const int ret = avcodec_encode_video2(cctx_, &packet, frame_, &output);
  if (ret < 0)
  {

    return 3;
  }

  if (output > 0) // Were any frames encoded
  {
    if (av_interleaved_write_frame(context_, &packet) < 0)
    {

      return 4;
    }
  }
  return 0;
}

}

QT_END_NAMESPACE
