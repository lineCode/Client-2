// h264decoder.cpp
//

///// Includes /////

#include "monocleclient/h264decoder.h"

#ifdef _WIN32
  #include <boost/asio.hpp>
  #include <windows.h>
#endif

#include <boost/algorithm/string.hpp>
#include <utility/utility.hpp>

extern "C"
{
  #include <libavutil/hwcontext.h>
  #include <libavutil/hwcontext_cuda.h>
  #include <libavutil/hwcontext_internal.h>
}

#include "monocleclient/mainwindow.h"
#include "monocleclient/mediaview.h"
#include "monocleclient/videoview.h"

///// Namespaces /////

namespace client
{

///// Globals /////

static const uint8_t STARTSEQUENCE[] = { 0, 0, 0, 1 };

///// Globals /////

static enum AVPixelFormat GetHardwareFormat(AVCodecContext* context, const enum AVPixelFormat* formats)
{
  const enum AVPixelFormat* p;
  for (p = formats; *p != -1; p++)
  {
    if (*p == *reinterpret_cast<AVPixelFormat*>(&context->opaque))
    {

      return *p;
    }
  }
  return AV_PIX_FMT_NONE;
}

static enum AVPixelFormat GetHardwareDeviceFormat(const enum AVHWDeviceType type)
{
  switch (type)
  {
    case AV_HWDEVICE_TYPE_CUDA:
    {

      return AV_PIX_FMT_CUDA;
    }
    case AV_HWDEVICE_TYPE_QSV:
    {

      return AV_PIX_FMT_QSV;
    }
    default:
    {

      return AV_PIX_FMT_NONE;
    }
  }
}

///// Methods /////

H264Decoder::H264Decoder(const uint32_t trackid, const uint64_t id, const utility::PublicKey& publickey, CUcontext cudacontext) :
  Decoder(trackid, id, publickey, cudacontext)
{
  
}

H264Decoder::~H264Decoder()
{
  Destroy();

}

void H264Decoder::Destroy()
{
  Decoder::Destroy();
  profilelevelid_.clear();
  spropparametersets_.clear();
}

DECODERERROR H264Decoder::Init(const std::vector<std::string>& parameters)
{
  Destroy();

  const std::string PROFILELEVELID = "profile-level-id=";
  std::vector<std::string>::const_iterator profilelevelid = std::find_if(parameters.cbegin(), parameters.cend(), [&PROFILELEVELID](const std::string& parameter) { return (boost::istarts_with(parameter, PROFILELEVELID)); });
  if (profilelevelid != parameters.cend())
  {
    profilelevelid_ = profilelevelid->substr(PROFILELEVELID.size(), std::string::npos);

  }

  const std::string SPROPPARAMETERSETS = "sprop-parameter-sets=";
  std::vector<std::string>::const_iterator spropparametersets = std::find_if(parameters.cbegin(), parameters.cend(), [&SPROPPARAMETERSETS](const std::string& parameter) { return (boost::istarts_with(parameter, SPROPPARAMETERSETS)); });
  if (spropparametersets != parameters.cend())
  {
    spropparametersets_ = spropparametersets->substr(SPROPPARAMETERSETS.size(), std::string::npos);

  }

  return Init();
}

ImageBuffer H264Decoder::Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers)
{

  return Decoder::Decode(playrequestindex, marker, time, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, data, size, freeimagebuffers);
}

DECODERERROR H264Decoder::Init()
{
  codec_ = avcodec_find_decoder_by_name("h264");
  if (codec_ == nullptr)
  {

    return DECODERERROR_INITCODEC;
  }

  context_ = avcodec_alloc_context3(codec_);
  if (context_ == nullptr)
  {

    return DECODERERROR_INITCONTEXT;
  }

  if (avcodec_get_context_defaults3(context_, codec_) < 0)
  {

    return DECODERERROR_RESETCODEC;
  }

  // sprop-parameter-sets
  const size_t PADDING_SIZE = 32;
  std::vector<uint8_t> extradata;
  extradata.reserve(512);
  std::vector<std::string> split;
  boost::algorithm::split(split, spropparametersets_, boost::is_any_of(","));
  for (const std::string& data : split)
  {
    extradata.insert(extradata.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
    const std::vector<unsigned char> decode = utility::Base64Decode(data);
    extradata.insert(extradata.end(), decode.begin(), decode.end());
  }

  std::array<uint8_t, PADDING_SIZE> padding;
  padding.fill(0);
  extradata.insert(extradata.end(), padding.cbegin(), padding.cend());

  // Grab all the hardware encoder possibilities that were compiled into FFMPEG(this does not represent what is available on the local system)
  std::vector<AVHWDeviceType> types;
  types.reserve(10);
  AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
  for (int i = 0; i < 50; ++i) // Just in case FFMPEG misbehaves infinite loops
  {
    AVHWDeviceType tmp = av_hwdevice_iterate_types(type);
    if (tmp == AV_HWDEVICE_TYPE_NONE)
    {

      break;
    }
    types.push_back(tmp);
    type = tmp;
  }

  // Do we support CUDA decoding?
  if (cudacontext_ && Options::Instance().GetMaxCUDADecodersPerDevice() && MainWindow::Instance()->GetNumCUDADevices() && utility::Contains(types, AV_HWDEVICE_TYPE_CUDA) && MainWindow::Instance()->nvcudadll_)
  {
    // Count the number of views using this cuda context as their hardware decoder
    const std::vector< QSharedPointer<View> > views = MainWindow::Instance()->GetVideoWidgetsMgr().GetViews();
    unsigned int numcudadecoders = 0;
    for (const QSharedPointer<View>& view : views)
    {
      if (view->GetViewType() == VIEWTYPE_MEDIA)
      {
        QSharedPointer<MediaView> mediaview = view.staticCast<MediaView>();
        if (mediaview->HasHardwareDecoder() && (cudacontext_ == mediaview->GetCUDAContext()))
        {
          ++numcudadecoders;

        }
      }
      else if (view->GetViewType() == VIEWTYPE_MONOCLE)
      {
        QSharedPointer<VideoView> videoview = view.staticCast<VideoView>();
        if (videoview->HasHardwareDecoder() && (cudacontext_ == videoview->GetCUDAContext()))
        {
          ++numcudadecoders;

        }
      }
    }

    if (numcudadecoders < Options::Instance().GetMaxCUDADecodersPerDevice())
    {
      if (InitCUDA())
      {
        LOG_GUI_WARNING(tr("Unable to initialise CUDA device, falling back to software decoding"));

      }
    }
  }

  //TODO QuickSync
  //if ((hwdevice_ == nullptr) && utility::Contains(types, AV_HWDEVICE_TYPE_QSV))
  //{
  //  const int ret = av_hwdevice_ctx_create(&hwdevice_, AV_HWDEVICE_TYPE_QSV, nullptr, nullptr, 0);
  //  if (ret)
  //  {
  //    // Don't even warn here, we expect this to happen so often
  //    hwdevice_ = nullptr; // Just in case
  //  }
  //  else
  //  {
  //    hardwaredevicetype = AV_HWDEVICE_TYPE_QSV;
  //
  //  }
  //}

  context_->width = 0;
  context_->height = 0;
  context_->thread_count = 1;

  context_->ticks_per_frame = 2;

  context_->time_base.num = 1;
  context_->time_base.den = 90000.0;

  if (codec_->capabilities & AV_CODEC_CAP_TRUNCATED)
  {
    context_->flags |= AV_CODEC_FLAG_TRUNCATED;

  }

  // profile-level-id
  if ((profilelevelid_.size() != 6) || !std::all_of(profilelevelid_.begin(), profilelevelid_.end(), [](char c) { return std::isxdigit(c); }))
  {

    return DECODERERROR_INVALIDPROFILELEVELID;
  }

  try
  {
    context_->profile = std::stoi(profilelevelid_.substr(0, 2).c_str(), nullptr, 16);
    context_->level = std::stoi(profilelevelid_.substr(4, 2).c_str(), nullptr, 16);
    //const int profileiop = std::stoi(profilelevelid_->value_.substr(2, 2).c_str(), nullptr, 16); // Not required at the moment. It describes requirements in the decoder, and we just assume ffmpeg can decode everything... https://tools.ietf.org/html/rfc6184
  }
  catch (...)
  {

    return DECODERERROR_INVALIDPROFILELEVELID;
  }

  context_->extradata_size = static_cast<int>(extradata.size());
  context_->extradata = new uint8_t[extradata.size()];
  memcpy(context_->extradata, extradata.data(), extradata.size());

  if (avcodec_open2(context_, codec_, nullptr) < 0)
  {

    return DECODERERROR_OPENCODEC;
  }

  return DECODERERROR_SUCCESS;
}

DECODERERROR H264Decoder::InitCUDA()
{
  // Pick the device with the lowest number of current users
  AVBufferRef* hwdevice = av_hwdevice_ctx_alloc(AV_HWDEVICE_TYPE_CUDA);
  if (!hwdevice)
  {

    return DECODERERROR_INITCONTEXT;
  }
  AVHWDeviceContext* hwdevicectx = reinterpret_cast<AVHWDeviceContext*>(hwdevice->data);
  AVCUDADeviceContext* cudadevicectx = reinterpret_cast<AVCUDADeviceContext*>(hwdevicectx->hwctx);
  cudadevicectx->stream = nullptr;
  cudadevicectx->cuda_ctx = cudacontext_;
  //hwctx->internal->is_allocated = 0; // This set to zero automatically by ffmpeg internally with mallocz, but leaving it here for reference
  if (av_hwdevice_ctx_init(hwdevice))
  {

    return DECODERERROR_INITCONTEXT;
  }
    
  // Make sure we support the NV12 format required
  AVHWFramesConstraints* constraints = av_hwdevice_get_hwframe_constraints(hwdevice, nullptr);
  if (constraints == nullptr)
  {
    av_buffer_unref(&hwdevice);
    return DECODERERROR_INITCONTEXT;
  }

  AVPixelFormat* hwformat = constraints->valid_hw_formats;
  std::vector<AVPixelFormat> hwformats;
  while (*hwformat != AV_PIX_FMT_NONE)
  {
    hwformats.push_back(*hwformat);
    ++hwformat;
  }

  AVPixelFormat* swformat = constraints->valid_sw_formats;
  std::vector<AVPixelFormat> swformats;
  while (*swformat != AV_PIX_FMT_NONE)
  {
    swformats.push_back(*swformat);
    ++swformat;
  }

  av_hwframe_constraints_free(&constraints);
  constraints = nullptr;

  if (!utility::Contains(swformats, AV_PIX_FMT_NV12))
  {
    av_buffer_unref(&hwdevice);
    return DECODERERROR_INITCONTEXT;
  }

  const AVPixelFormat format = GetHardwareDeviceFormat(AV_HWDEVICE_TYPE_CUDA);
  if (format == AV_PIX_FMT_NONE)
  {
    av_buffer_unref(&hwdevice);
    return DECODERERROR_INITCONTEXT;
  }

  context_->hw_device_ctx = av_buffer_ref(hwdevice);
  hwdevice_ = hwdevice;
  context_->opaque = reinterpret_cast<void*>(format);
  context_->get_format = GetHardwareFormat;
  context_->pix_fmt = format;
  context_->sw_pix_fmt = AV_PIX_FMT_NV12;
  return DECODERERROR_SUCCESS;
}

}
