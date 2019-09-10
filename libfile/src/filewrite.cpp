// filewrite.cpp
//

///// Includes /////

#include "file/filewrite.hpp"

#ifdef _WIN32
  #include <windows.h>
  #include <KtmW32.h>
#else
  #include <fcntl.h>
  #include <sys/inotify.h>
  #include <sys/mman.h>
#endif

#include "file/file.hpp"
#include "file/file_generated.h"

///// Namespaces /////

namespace file
{

///// Methods /////

FileWrite::FileWrite() :
#ifdef _WIN32
  file_(INVALID_HANDLE_VALUE),
#else
  file_(-1),
#endif
  currentoffset_(0)
{

}

FileWrite::~FileWrite()
{
  Destroy();

}

int FileWrite::Init(const boost::filesystem::path& path)
{
  Destroy();

#ifdef _WIN32
  file_ = ::CreateFile(path.string().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_NEW, 0, nullptr);
  if (file_ == INVALID_HANDLE_VALUE)
  {

    return 1;
  }
#else
  file_ = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (file_ == -1)
  {

    return 1;
  }
#endif
  return 0;
}

void FileWrite::Destroy()
{
#ifdef _WIN32
  if (file_ != INVALID_HANDLE_VALUE)
  {
    CloseHandle(file_);
    file_ = INVALID_HANDLE_VALUE;
  }
#else
  if (file_ != -1)
  {
    close(file_);
    file_ = -1;
  }
#endif
  currentoffset_ = 0;
}

int FileWrite::Open()
{
#ifdef _WIN32
  if (file_ == INVALID_HANDLE_VALUE)
  {

    return 1;
  }

  if (WriteFile(file_, &MAGIC_BYTES, sizeof(MAGIC_BYTES), nullptr, nullptr) == 0)
  {

    return 2;
  }

  if (WriteFile(file_, &VERSION, sizeof(VERSION), nullptr, nullptr) == 0)
  {

    return 3;
  }
#else
  if (file_ == -1)
  {

    return 1;
  }

  if (write(file_, &MAGIC_BYTES, sizeof(MAGIC_BYTES)) == -1)
  {

    return 2;
  }

  if (write(file_, &VERSION, sizeof(VERSION)) == -1)
  {

    return 3;
  }
#endif
  return 0;
}

int FileWrite::Close(const FILE& file)
{
#ifdef _WIN32
  if (file_ == INVALID_HANDLE_VALUE)
  {

    return 1;
  }
#else
  if (file_ == -1)
  {

    return 2;
  }
#endif
  
  // Build the flat buffer
  flatbuffers::FlatBufferBuilder fbb;
  std::vector< flatbuffers::Offset<file::Device> > devicesfb;
  for (const file::DEVICE& device : file.devices_)
  {
    std::vector< flatbuffers::Offset<file::Recording> > recordingsfb;
    for (const file::RECORDING& recording : device.recordings_)
    {
      std::vector< flatbuffers::Offset<file::Track> > tracksfb;
      for (const file::TRACK& track : recording.tracks_)
      {
        std::vector< flatbuffers::Offset<file::Codec> > codecsfb;
        for (const CODEC& codec : track.codecs_)
        {
          codecsfb.push_back(CreateCodecDirect(fbb, codec.index_, codec.codec_, codec.parameters_.c_str()));

        }

        std::vector< flatbuffers::Offset<H265FrameHeader> > h265frameheadersfb;
        std::map<FRAMEINDEX, std::vector< std::unique_ptr<H265FRAMEHEADER> > >::iterator h265frameheaders = h265frameheaders_.find(FRAMEINDEX(device.index_, recording.index_, track.index_));
        if (h265frameheaders != h265frameheaders_.cend())
        {
          for (const std::unique_ptr<H265FRAMEHEADER>& h265frameheader : h265frameheaders->second)
          {
            h265frameheadersfb.push_back(CreateH265FrameHeader(fbb, h265frameheader->codecindex_, h265frameheader->offset_, h265frameheader->size_, h265frameheader->marker_, h265frameheader->time_, h265frameheader->signature_.size() ? fbb.CreateVector(h265frameheader->signature_) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), h265frameheader->donlfield_, fbb.CreateVector(h265frameheader->offsets_)));

          }
        }

        std::vector< flatbuffers::Offset<H264FrameHeader> > h264frameheadersfb;
        std::map<FRAMEINDEX, std::vector< std::unique_ptr<H264FRAMEHEADER> > >::iterator h264frameheaders = h264frameheaders_.find(FRAMEINDEX(device.index_, recording.index_, track.index_));
        if (h264frameheaders != h264frameheaders_.cend())
        {
          for (const std::unique_ptr<H264FRAMEHEADER>& h264frameheader : h264frameheaders->second)
          {
            h264frameheadersfb.push_back(CreateH264FrameHeader(fbb, h264frameheader->codecindex_, h264frameheader->offset_, h264frameheader->size_, h264frameheader->marker_, h264frameheader->time_, h264frameheader->signature_.size() ? fbb.CreateVector(h264frameheader->signature_) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), fbb.CreateVector(h264frameheader->offsets_)));

          }
        }

        std::vector< flatbuffers::Offset<JPEGFrameHeader> > jpegframeheadersfb;
        std::map<FRAMEINDEX, std::vector< std::unique_ptr<JPEGFRAMEHEADER> > >::iterator jpegframeheaders = jpegframeheaders_.find(FRAMEINDEX(device.index_, recording.index_, track.index_));
        if (jpegframeheaders != jpegframeheaders_.cend())
        {
          for (const std::unique_ptr<JPEGFRAMEHEADER>& jpegframeheader : jpegframeheaders->second)
          {
            jpegframeheadersfb.push_back(CreateJPEGFrameHeader(fbb, jpegframeheader->codecindex_, jpegframeheader->offset_, jpegframeheader->size_, jpegframeheader->marker_, jpegframeheader->time_, jpegframeheader->signature_.size() ? fbb.CreateVector(jpegframeheader->signature_) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), jpegframeheader->restartinterval_, jpegframeheader->typespecificfragmentoffset_, jpegframeheader->type_, jpegframeheader->q_, jpegframeheader->width_, jpegframeheader->height_, fbb.CreateVector(jpegframeheader->lqt_.data(), jpegframeheader->lqt_.size()), fbb.CreateVector(jpegframeheader->cqt_.data(), jpegframeheader->cqt_.size())));

          }
        }

        std::vector< flatbuffers::Offset<MetadataFrameHeader> > metadataframeheadersfb;
        std::map<FRAMEINDEX, std::vector< std::unique_ptr<METADATAFRAMEHEADER> > >::iterator metadataframeheaders = metadataframeheaders_.find(FRAMEINDEX(device.index_, recording.index_, track.index_));
        if (metadataframeheaders != metadataframeheaders_.cend())
        {
          for (const std::unique_ptr<METADATAFRAMEHEADER>& metadataframeheader : metadataframeheaders->second)
          {
            metadataframeheadersfb.push_back(CreateMetadataFrameHeader(fbb, metadataframeheader->codecindex_, metadataframeheader->offset_, metadataframeheader->size_, metadataframeheader->marker_, metadataframeheader->time_, metadataframeheader->signature_.size() ? fbb.CreateVector(metadataframeheader->signature_) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >()));

          }
        }

        std::vector< flatbuffers::Offset<MPEG4FrameHeader> > mpeg4frameheadersfb;
        std::map<FRAMEINDEX, std::vector< std::unique_ptr<MPEG4FRAMEHEADER> > >::iterator mpeg4frameheaders = mpeg4frameheaders_.find(FRAMEINDEX(device.index_, recording.index_, track.index_));
        if (mpeg4frameheaders != mpeg4frameheaders_.cend())
        {
          for (const std::unique_ptr<MPEG4FRAMEHEADER>& mpeg4frameheader : mpeg4frameheaders->second)
          {
            mpeg4frameheadersfb.push_back(CreateMPEG4FrameHeader(fbb, mpeg4frameheader->codecindex_, mpeg4frameheader->offset_, mpeg4frameheader->size_, mpeg4frameheader->marker_, mpeg4frameheader->time_, mpeg4frameheader->signature_.size() ? fbb.CreateVector(mpeg4frameheader->signature_) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >()));

          }
        }

        tracksfb.push_back(CreateTrackDirect(fbb, track.index_, track.description_.c_str(), &codecsfb, &h264frameheadersfb, &jpegframeheadersfb, &metadataframeheadersfb, &mpeg4frameheadersfb, &h265frameheadersfb));
      }
      recordingsfb.push_back(CreateRecording(fbb, recording.index_, fbb.CreateString(recording.name_), fbb.CreateVector(tracksfb), fbb.CreateString(recording.location_)));
    }
    devicesfb.push_back(CreateDeviceDirect(fbb, device.index_, device.name_.c_str(), device.address_.c_str(), device.signingkey_.c_str(), &recordingsfb));
  }
  flatbuffers::Offset<file::File> filefb = CreateFileDirect(fbb, &devicesfb);
  fbb.Finish(filefb);
  const uint64_t size = fbb.GetSize();
#ifdef _WIN32
  if (WriteFile(file_, fbb.GetBufferPointer(), static_cast<DWORD>(size), nullptr, nullptr) == 0)
  {

    return 3;
  }

  if (WriteFile(file_, &size, sizeof(size), nullptr, nullptr) == 0)
  {

    return 4;
  }

  if (CloseHandle(file_) == 0)
  {
    file_ = INVALID_HANDLE_VALUE;
    return 5;
  }
  file_ = INVALID_HANDLE_VALUE;
#else
  if (write(file_, fbb.GetBufferPointer(), size) == -1)
  {

    return 6;
  }

  if (write(file_, &size, sizeof(size)) == -1)
  {

    return 7;
  }

  if (close(file_) == -1)
  {
    file_ = -1;
    return 8;
  }
  file_ = -1;
#endif
  return 0;
}

int FileWrite::WriteH265Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const char* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature, const bool donlfield, const std::vector<uint32_t>& offsets)
{
  const int ret = WriteFrame(data, size);
  if (ret)
  {

    return ret;
  }

  const FRAMEINDEX frameindex(deviceindex, recordingindex, trackindex);
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<H265FRAMEHEADER> > >::iterator h265frameheader = h265frameheaders_.find(frameindex);
  if (h265frameheader == h265frameheaders_.cend())
  {
    std::vector< std::unique_ptr<H265FRAMEHEADER> > tmp;
    tmp.emplace_back(std::make_unique<H265FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, donlfield, offsets));
    h265frameheaders_.emplace(frameindex, std::move(tmp));
  }
  else
  {
    h265frameheader->second.push_back(std::make_unique<H265FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, donlfield, offsets));

  }

  currentoffset_ += size;
  return 0;
}

int FileWrite::WriteH264Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const char* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature, const std::vector<uint32_t>& offsets)
{
  const int ret = WriteFrame(data, size);
  if (ret)
  {

    return ret;
  }

  const FRAMEINDEX frameindex(deviceindex, recordingindex, trackindex);
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<H264FRAMEHEADER> > >::iterator h264frameheader = h264frameheaders_.find(frameindex);
  if (h264frameheader == h264frameheaders_.cend())
  {
    std::vector< std::unique_ptr<H264FRAMEHEADER> > tmp;
    tmp.emplace_back(std::make_unique<H264FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, offsets));
    h264frameheaders_.emplace(frameindex, std::move(tmp));
  }
  else
  {
    h264frameheader->second.push_back(std::make_unique<H264FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, offsets));

  }

  currentoffset_ += size;
  return 0;
}

int FileWrite::WriteJPEGFrame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const char* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const std::array<uint8_t, 64>& lqt, const std::array<uint8_t, 64>& cqt)
{
  const int ret = WriteFrame(data, size);
  if (ret)
  {

    return ret;
  }

  const FRAMEINDEX frameindex(deviceindex, recordingindex, trackindex);
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<JPEGFRAMEHEADER> > >::iterator jpegframeheader = jpegframeheaders_.find(frameindex);
  if (jpegframeheader == jpegframeheaders_.cend())
  {
    std::vector< std::unique_ptr<JPEGFRAMEHEADER> > tmp;
    tmp.emplace_back(std::make_unique<JPEGFRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt));
    jpegframeheaders_.emplace(frameindex, std::move(tmp));
  }
  else
  {
    jpegframeheader->second.push_back(std::make_unique<JPEGFRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature, restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt));

  }

  currentoffset_ += size;
  return 0;
}

int FileWrite::WriteMetadataFrame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const char* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature)
{
  const int ret = WriteFrame(data, size);
  if (ret)
  {

    return ret;
  }

  const FRAMEINDEX frameindex(deviceindex, recordingindex, trackindex);
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<METADATAFRAMEHEADER> > >::iterator metadataframeheader = metadataframeheaders_.find(frameindex);
  if (metadataframeheader == metadataframeheaders_.cend())
  {
    std::vector< std::unique_ptr<METADATAFRAMEHEADER> > tmp;
    tmp.emplace_back(std::make_unique<METADATAFRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature));
    metadataframeheaders_.emplace(frameindex, std::move(tmp));
  }
  else
  {
    metadataframeheader->second.push_back(std::make_unique<METADATAFRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature));

  }

  currentoffset_ += size;
  return 0;
}

int FileWrite::WriteMPEG4Frame(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t codecindex, const char* data, const uint64_t size, const bool marker, const uint64_t time, const std::vector<unsigned char>& signature)
{
  const int ret = WriteFrame(data, size);
  if (ret)
  {

    return ret;
  }

  const FRAMEINDEX frameindex(deviceindex, recordingindex, trackindex);
  std::map<FRAMEINDEX, std::vector< std::unique_ptr<MPEG4FRAMEHEADER> > >::iterator mpeg4frameheader = mpeg4frameheaders_.find(frameindex);
  if (mpeg4frameheader == mpeg4frameheaders_.cend())
  {
    std::vector< std::unique_ptr<MPEG4FRAMEHEADER> > tmp;
    tmp.emplace_back(std::make_unique<MPEG4FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature));
    mpeg4frameheaders_.emplace(frameindex, std::move(tmp));
  }
  else
  {
    mpeg4frameheader->second.push_back(std::make_unique<MPEG4FRAMEHEADER>(codecindex, currentoffset_, size, marker, time, signature));

  }

  currentoffset_ += size;
  return 0;
}

int FileWrite::WriteFrame(const char* data, const uint64_t size)
{
#ifdef _WIN32
  if (file_ == INVALID_HANDLE_VALUE)
  {

    return 1;
  }

  if (WriteFile(file_, data, static_cast<DWORD>(size), nullptr, nullptr) == 0)
  {

    return 2;
  }
#else
  if (file_ == -1)
  {

    return 1;
  }

  if (write(file_, data, size) == -1)
  {

    return 2;
  }
#endif
  return 0;
}

FileWrite::FRAMEINDEX::FRAMEINDEX(const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex) :
  deviceindex_(deviceindex),
  recordingindex_(recordingindex),
  trackindex_(trackindex)
{

}

bool FileWrite::FRAMEINDEX::operator<(const FileWrite::FRAMEINDEX& rhs) const
{
  if (deviceindex_ < rhs.deviceindex_)
  {

    return true;
  }
  else if (deviceindex_ == rhs.deviceindex_)
  {
    if (recordingindex_ < rhs.recordingindex_)
    {

      return true;
    }
    else if (recordingindex_ == rhs.recordingindex_)
    {
      if (trackindex_ < rhs.trackindex_)
      {

        return true;
      }
    }
  }
  return false;
}

}

