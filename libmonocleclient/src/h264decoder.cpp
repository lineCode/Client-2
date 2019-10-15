// h264decoder.cpp
//

///// Includes /////

#include "monocleclient/h264decoder.h"

#ifdef _WIN32
  #include <boost/asio.hpp>
  #include <windows.h>
#endif

#include <boost/algorithm/string.hpp>
#include <cuda_gl_interop.h>
#include <utility/utility.hpp>
#include <iostream>//TODO remove
extern "C"
{
  #include <libavutil/hwcontext.h>
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

H264Decoder::H264Decoder(const uint64_t id, const utility::PublicKey& publickey) :
  Decoder(id, publickey)
{
  
}

H264Decoder::~H264Decoder()
{
  Destroy();

}

void H264Decoder::Destroy()
{
  Decoder::Destroy();
  hardwaredevice_.reset();
  profilelevelid_.clear();
  spropparametersets_.clear();
}

DECODERERROR H264Decoder::Init(const std::vector<std::string>& parameters, QOpenGLFunctions* openglfunctions, const std::array<GLuint, 3>& textures)
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

  return Init(openglfunctions, textures);
}

ImageBuffer H264Decoder::Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers)
{
  if (hardwaredevice_.is_initialized())
  {
    cudaSetDevice(*hardwaredevice_);//TODO don't want to do this every tiem surely?

  }

  return Decoder::Decode(playrequestindex, marker, time, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, data, size, freeimagebuffers);
}

DECODERERROR H264Decoder::Init(QOpenGLFunctions* openglfunctions, const std::array<GLuint, 3>& textures)//TODO textures needs to go I think, because get_buffer2() will grab the buffers
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
    
    for (auto a : decode)//TODO remove
    {
      std::cout << (int)a << " ";

    }
    std::cout << std::endl;//TODO remove

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
  AVHWDeviceType hardwaredevicetype = AV_HWDEVICE_TYPE_NONE;
  if (openglfunctions && Options::Instance().GetMaxCUDADecodersPerDevice() && MainWindow::Instance()->GetNumCUDADevices() && utility::Contains(types, AV_HWDEVICE_TYPE_CUDA))//TODO this will need to check the textures[3] is available too
  {
    const std::vector< QSharedPointer<View> > views = MainWindow::Instance()->GetVideoWidgetsMgr().GetViews();
    std::vector< std::pair<unsigned int, int> > devices; // <NumViews, Device>
    devices.reserve(MainWindow::Instance()->GetNumCUDADevices());
    for (int i = 0; i < MainWindow::Instance()->GetNumCUDADevices(); ++i)
    {
      unsigned int numcudadecoders = 0;
      for (const QSharedPointer<View>& view : views)
      {
        if (view->GetViewType() == VIEWTYPE_MEDIA)
        {
          QSharedPointer<MediaView> mediaview = view.staticCast<MediaView>();
          if (utility::Contains(mediaview->GetCUDADevices(), i))
          {
            ++numcudadecoders;

          }
        }
        else if (view->GetViewType() == VIEWTYPE_MONOCLE)
        {
          QSharedPointer<VideoView> videoview = view.staticCast<VideoView>();
          if (utility::Contains(videoview->GetCUDADevices(), i))
          {
            ++numcudadecoders;

          }
        }
      }

      if (numcudadecoders >= Options::Instance().GetMaxCUDADecodersPerDevice())
      {

        continue;
      }
      devices.push_back(std::make_pair(numcudadecoders, i));
    }

    if (devices.size())
    {
      // Pick the device with the lowest number of current users
      std::sort(devices.begin(), devices.end());
      const int hardwaredevice = devices.front().second;
      const int ret = av_hwdevice_ctx_create(&hwdevice_, AV_HWDEVICE_TYPE_CUDA, std::to_string(hardwaredevice).c_str(), nullptr, 0);
      if (ret)
      {
        LOG_GUI_WARNING(tr("Unable to initialise CUDA device, falling back to software decoding"));
        hwdevice_ = nullptr; // Just in case
      }
      else
      {
        // Make sure we support the NV12 format required
        AVHWFramesConstraints* constraints = av_hwdevice_get_hwframe_constraints(hwdevice_, nullptr);
        if (!constraints)
        {
          LOG_GUI_WARNING(tr("Unable to retrieve CUDA device constraints, falling back to software decoding"));
          //TODO free hwdevice_
          hwdevice_ = nullptr; // Just in case
        }
        else
        {
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

          if (!utility::Contains(swformats, AV_PIX_FMT_NV12))
          {
            LOG_GUI_WARNING(tr("Unable to initialise CUDA device, NV12 format not supported, falling back to software decoding"));
            //TODO free hwdevice_
            hwdevice_ = nullptr;
          }
          else
          {
            hardwaredevice_ = hardwaredevice;
            hardwaredevicetype = AV_HWDEVICE_TYPE_CUDA;

            //TODO context_->opaque
            //TODO context_->get_buffer2 = [](AVCodecContext* context, AVFrame* frame, int flags) -> int
            //TODO {
            //TODO   push the cuda context
            //TODO   call the view to create an opengl buffer object and map it to cuda and bring back the CUDevicePtr
            //TODO   pop the cuda context...
            //TODO   return 0;
            //TODO };


            //TODO read the SPS to get the width/height and then generate the opengl textures manually here


            //TODO create a widths() and heights() array

            // Map CUDA resources for our textures
            //TODO std::array<cudaGraphicsResource*, 3> resources = { nullptr, nullptr, nullptr };//TODO goes as a member now ?
            //TODO free the resources_ somewhere
            //TODO for (int i = 0; i < textures.size(); ++i)//TODO for nv12 we only need two... remove the loop imo
            //TODO {
            //TODO   openglfunctions->glBindTexture(GL_TEXTURE_2D, textures[i]);
            //TODO   openglfunctions->glTexImage2D(GL_TEXTURE_2D, 0, (i == 0) ? GL_RED : GL_RG, (i == 0) ? 1280 : 640, (i == 0) ? 720 : 180, 0, (i == 0) ? GL_RED : GL_RG, GL_UNSIGNED_BYTE, nullptr);//TODO width and height... we need to read the SPS...
            //TODO   if (cudaGraphicsGLRegisterImage(&resources[i], textures[i], GL_TEXTURE_2D, cudaGraphicsMapFlagsWriteDiscard) != cudaSuccess)
            //TODO   //TODO if (cudaGraphicsGLRegisterImage(&resources[i], textures[i], GL_TEXTURE_2D, cudaGraphicsMapFlagsNone) != cudaSuccess)
            //TODO   {
            //TODO     
            //TODO     return DECODERERROR_INITCODEC;
            //TODO   }
            //TODO 
            //TODO   cudaError ret = cudaGraphicsMapResources(1, &resources[i], 0);
            //TODO   if (ret != cudaSuccess)
            //TODO   {
            //TODO 
            //TODO     return DECODERERROR_INITCODEC;
            //TODO   }
            //TODO   
            //TODO   //TODO it's POSSIBLE this pointer will work for FFMPEG...
            //TODO   cudaArray* ptr = nullptr;//TODO we don't want this kind of pointer... we want a CuDevicePtr or something
            //TODO   size_t size = 1;
            //TODO   ret = cudaGraphicsSubResourceGetMappedArray(&ptr, resources[i], 0, 0);
            //TODO   if (ret != cudaSuccess)
            //TODO   {
            //TODO 
            //TODO     return DECODERERROR_INITCODEC;
            //TODO   }
            //TODO 
            //TODO   //TODO cudaGraphicsUnregisterResource()
            //TODO   //TODO cudaGraphicsUnmapResources()
            //TODO   openglfunctions->glBindTexture(GL_TEXTURE_2D, 0);
            //TODO }

            //TODO sort out the packet_
              //TODO maybe need to set the packet_ every time before we Decode
                //TODO we would need to override Decoder::Decode()
          }
        }
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

  if (hwdevice_)
  {
    const AVPixelFormat format = GetHardwareDeviceFormat(hardwaredevicetype);
    if (format == AV_PIX_FMT_NONE)
    {
      // Shouldn't ever happen...
      return DECODERERROR_INITCONTEXT;
    }

    context_->hw_device_ctx = av_buffer_ref(hwdevice_);
    if (context_->hw_device_ctx == nullptr)
    {
      // Should only happen if we're out of memory
      return DECODERERROR_INITCONTEXT;
    }
    context_->opaque = reinterpret_cast<void*>(format);//TODO probably need to reset this here
    context_->get_format = GetHardwareFormat;
    context_->pix_fmt = format;
    context_->sw_pix_fmt = (hardwaredevicetype == AV_HWDEVICE_TYPE_CUDA) ? AV_PIX_FMT_NV12 : AV_PIX_FMT_NONE;
  }

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

}
