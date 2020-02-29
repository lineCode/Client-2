// fileread.cpp
//

///// Includes /////

#include "file/fileread.hpp"

#ifdef _WIN32

#else
  #include <fcntl.h>
#endif

#include "file/file.hpp"
#include "file/file_generated.h"

///// Namespace /////

namespace file
{

///// Methods /////

FileRead::FileRead() :
#ifdef _WIN32
  handle_(INVALID_HANDLE_VALUE),
#else
  handle_(-1),
#endif
  size_(0),
  running_(false)
{

}

FileRead::~FileRead()
{
  Destroy();

}

std::future<int> FileRead::Init(const boost::filesystem::path& path, const std::shared_ptr<float>& progress)
{
  Destroy();

  running_ = true;
  *progress = 0.0f;
  std::shared_ptr< std::promise<int> > promise = std::make_shared< std::promise<int> >();
  thread_ = std::thread([this, promise, path, progress]()
  {
    // Open file
#ifdef _WIN32
    handle_ = ::CreateFile(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (handle_ == INVALID_HANDLE_VALUE)
    {
      promise->set_value(1);
      return;
    }

    LARGE_INTEGER s;
    if (GetFileSizeEx(handle_, &s) == FALSE)
    {
      promise->set_value(2);
      return;
    }
    size_ = s.QuadPart;
#else
    handle_ = open(path.c_str(), O_RDONLY);
    if (handle_ == -1)
    {
      promise->set_value(3);
      return;
    }

    size_ = lseek(handle_, 0, SEEK_END);
    if (size_ == -1)
    {
      promise->set_value(4);
      return;
    }
#endif
    if (size_ < (sizeof(MAGIC_BYTES) + sizeof(VERSION) + sizeof(uint64_t)))
    {
      promise->set_value(5);
      return;
    }

    // Read magic bytes
    std::remove_const<decltype(MAGIC_BYTES)>::type magicbytes = 0;
#ifdef _WIN32
    if (SetFilePointerEx(handle_, { 0 }, nullptr, FILE_BEGIN) == 0)
    {
      promise->set_value(6);
      return;
    }

    if (::ReadFile(handle_, &magicbytes, sizeof(magicbytes), nullptr, nullptr) == 0)
    {
      promise->set_value(7);
      return;
    }
#else
    if (lseek(handle_, 0, SEEK_SET) == -1)
    {
      promise->set_value(8);
      return;
    }

    if (read(handle_, &magicbytes, sizeof(magicbytes)) == -1)
    {
      promise->set_value(9);
      return;
    }
#endif
    if (magicbytes != MAGIC_BYTES)
    {
      promise->set_value(10);
      return;
    }

    // Read the version and flat buffer size at the end of the file
    // We are currently forwards *and* backwards compatible, so we don't need to compare the version, but we'll grab it anyway for handy future use
    std::remove_const<decltype(VERSION)>::type version = 0;
#ifdef _WIN32
    if (::ReadFile(handle_, &version, sizeof(version), nullptr, nullptr) == 0)
    {
      promise->set_value(11);
      return;
    }

    LARGE_INTEGER tmp;
    tmp.QuadPart = -static_cast<int64_t>(sizeof(uint64_t));
    if (SetFilePointerEx(handle_, tmp, nullptr, FILE_END) == 0)
    {
      promise->set_value(12);
      return;
    }

    uint64_t filesize = 0;
    if (::ReadFile(handle_, &filesize, sizeof(filesize), nullptr, nullptr) == 0)
    {
      promise->set_value(13);
      return;
    }
#else
    if (read(handle_, &version, sizeof(version)) == -1)
    {
      promise->set_value(14);
      return;
    }

    if (lseek(handle_, -sizeof(uint64_t), SEEK_END) == -1)
    {
      promise->set_value(15);
      return;
    }

    uint64_t filesize = 0;
    if (read(handle_, &filesize, sizeof(filesize)) == -1)
    {
      promise->set_value(16);
      return;
    }
#endif

    if (filesize > (size_ - sizeof(uint64_t) + sizeof(MAGIC_BYTES) + sizeof(VERSION)))
    {
      promise->set_value(17);
      return;
    }

#ifdef _WIN32
    tmp.QuadPart = -static_cast<int64_t>(filesize + sizeof(filesize));
    if (SetFilePointerEx(handle_, tmp, nullptr, FILE_END) == 0)
    {
      promise->set_value(18);
      return;
    }
#else
    if (lseek(handle_, -(filesize + sizeof(filesize)), SEEK_END) == -1)
    {
      promise->set_value(19);
      return;
    }
#endif

    // Parse the flat buffer
    std::vector<uint8_t> buf;
    buf.resize(filesize);
#ifdef _WIN32
    if (::ReadFile(handle_, buf.data(), static_cast<DWORD>(filesize), nullptr, nullptr) == 0)
    {
      promise->set_value(20);
      return;
    }
#else
    if (read(handle_, buf.data(), filesize) == -1)
    {
      promise->set_value(21);
      return;
    }
#endif
    // We don't do this check, becausee it fails on large files
    //if (flatbuffers::Verifier(buf.data(), filesize).VerifyBuffer<File>() == false)
    //{
    //  promise->set_value(22);
    //  return;
    //}

    const file::File* file = file::GetFile(buf.data());
    if (file == nullptr)
    {
      promise->set_value(23);
      return;
    }

    if (file->devices() == nullptr)
    {
      promise->set_value(24);
      return;
    }

    // Count the total frames
    uint64_t totalframes = 0;
    for (const file::Device* device : *file->devices())
    {
      if (device->recordings())
      {
        for (const file::Recording* recording : *device->recordings())
        {
          if (recording->videotracks())
          {
            for (const file::Track* track : *recording->videotracks())
            {
              if (track->h265frameheaders())
              {
                totalframes += track->h265frameheaders()->size();

              }

              if (track->h264frameheaders())
              {
                totalframes += track->h264frameheaders()->size();

              }

              if (track->jpegframeheaders())
              {
                totalframes += track->jpegframeheaders()->size();

              }

              if (track->mpeg4frameheaders())
              {
                totalframes += track->mpeg4frameheaders()->size();

              }
            }
          }

          if (recording->metadatatracks())
          {
            for (const file::Track* track : *recording->metadatatracks())
            {
              if (track->metadataframeheaders())
              {
                totalframes += track->metadataframeheaders()->size();

              }
            }
          }
        }
      }
    }

    uint64_t currentframe = 0;
    std::vector<uint8_t> buffer;
    for (const file::Device* device : *file->devices())
    {
      if (std::find_if(file_.devices_.cbegin(), file_.devices_.cend(), [device](const DEVICE& d) { return (d.index_ == device->index()); }) != file_.devices_.cend()) // Make sure the index is unique
      {
        promise->set_value(25);
        return;
      }

      if (device->recordings() == nullptr)
      {
        promise->set_value(26);
        return;
      }

      boost::container::flat_set<RECORDING> recordings;
      for (const file::Recording* recording : *device->recordings())
      {
        if (std::find_if(recordings.cbegin(), recordings.cend(), [recording](const RECORDING& r) { return (r.index_ == recording->index()); }) != recordings.cend()) // Make sure the index is unique
        {
          promise->set_value(27);
          return;
        }

        boost::container::flat_set<TRACK> videotracks;
        if (recording->videotracks())
        {
          videotracks.reserve(recording->videotracks()->size());
          for (const file::Track* videotrack : *recording->videotracks())
          {
            if (std::find_if(videotracks.cbegin(), videotracks.cend(), [videotrack](const TRACK& t) { return (t.index_ == videotrack->index()); }) != videotracks.cend()) // Make sure the index is unique
            {
              promise->set_value(28);
              return;
            }

            // Codecs
            if (videotrack->codecs() == nullptr)
            {
              promise->set_value(29);
              return;
            }

            boost::container::flat_set<CODEC> codecs;
            for (const file::Codec* codec : *videotrack->codecs())
            {
              if (std::find_if(codecs.cbegin(), codecs.cend(), [codec](const CODEC& c) { return (c.index_ == codec->index()); }) != codecs.cend()) // Make sure the index is unique
              {
                promise->set_value(30);
                return;
              }
              codecs.insert(CODEC(codec->index(), codec->codec(), codec->parameters() ? codec->parameters()->str() : std::string()));
            }

            // Frame headers
            std::vector< std::shared_ptr<FRAMEHEADER> > frameheaders;
            if (videotrack->h265frameheaders())
            {
              for (const file::FrameHeader* h265frameheader : *videotrack->h265frameheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (h265frameheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(32);
                  return;
                }

                buffer.resize(h265frameheader->size());
                if (Read(h265frameheader->offset(), h265frameheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::H265Frame* h265frame = flatbuffers::GetRoot<H265Frame>(buffer.data());
                if (!h265frame || !h265frame->data())
                {
                  // Ignore I guess?
                  continue;
                }

                std::vector<uint32_t> offsets;
                if (h265frame->offsets())
                {
                  offsets = std::vector<uint32_t>(h265frame->offsets()->cbegin(), h265frame->offsets()->cend());

                }

                frameheaders.push_back(std::make_shared<H265FRAMEHEADER>(h265frame->codecindex(), h265frameheader->offset() + (h265frame->data()->data() - buffer.data()), h265frame->data()->size(), h265frame->marker(), h265frame->time(), (h265frame->signature() && h265frame->signature()->size()) ? std::vector<uint8_t>(h265frame->signature()->data(), h265frame->signature()->data() + h265frame->signature()->size()) : std::vector<uint8_t>(), h265frame->donlfield(), offsets));
              }
            }

            if (videotrack->h264frameheaders())
            {
              for (const file::FrameHeader* h264frameheader : *videotrack->h264frameheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (h264frameheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(33);
                  return;
                }

                buffer.resize(h264frameheader->size());
                if (Read(h264frameheader->offset(), h264frameheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::H264Frame* h264frame = flatbuffers::GetRoot<H264Frame>(buffer.data());
                if (!h264frame || !h264frame->data())
                {
                  // Ignore I guess?
                  continue;
                }

                std::vector<uint32_t> offsets;
                if (h264frame->offsets())
                {
                  offsets = std::vector<uint32_t>(h264frame->offsets()->cbegin(), h264frame->offsets()->cend());

                }

                frameheaders.push_back(std::make_shared<H264FRAMEHEADER>(h264frame->codecindex(), h264frameheader->offset() + (h264frame->data()->data() - buffer.data()), h264frame->data()->size(), h264frame->marker(), h264frame->time(), (h264frame->signature() && h264frame->signature()->size()) ? std::vector<uint8_t>(h264frame->signature()->data(), h264frame->signature()->data() + h264frame->signature()->size()) : std::vector<uint8_t>(), offsets));
              }
            }

            if (videotrack->jpegframeheaders())
            {
              for (const file::FrameHeader* jpegframeheader : *videotrack->jpegframeheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (jpegframeheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(34);
                  return;
                }

                buffer.resize(jpegframeheader->size());
                if (Read(jpegframeheader->offset(), jpegframeheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::JPEGFrame* jpegframe = flatbuffers::GetRoot<JPEGFrame>(buffer.data());
                if (!jpegframe || !jpegframe->data())
                {
                  // Ignore I guess?
                  continue;
                }

                std::array<uint8_t, 64> lqt;
                if (jpegframe->lqt() && jpegframe->lqt()->size() == 64)
                {
                  memcpy(lqt.data(), jpegframe->lqt()->data(), lqt.size());

                }
                else
                {
                  lqt.fill(0);

                }

                std::array<uint8_t, 64> cqt;
                if (jpegframe->cqt() && jpegframe->cqt()->size() == 64)
                {
                  memcpy(cqt.data(), jpegframe->cqt()->data(), cqt.size());

                }
                else
                {
                  cqt.fill(0);

                }

                frameheaders.push_back(std::make_shared<JPEGFRAMEHEADER>(jpegframe->codecindex(), jpegframeheader->offset() + (jpegframe->data()->data() - buffer.data()), jpegframe->data()->size(), jpegframe->time(), (jpegframe->signature() && jpegframe->signature()->size()) ? std::vector<uint8_t>(jpegframe->signature()->data(), jpegframe->signature()->data() + jpegframe->signature()->size()) : std::vector<uint8_t>(), jpegframe->restartinterval(), jpegframe->typespecificfragmentoffset(), jpegframe->type(), jpegframe->q(), jpegframe->width(), jpegframe->height(), lqt, cqt));
              }
            }

            if (videotrack->mpeg4frameheaders())
            {
              for (const file::FrameHeader* mpeg4frameheader : *videotrack->mpeg4frameheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (mpeg4frameheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(36);
                  return;
                }

                buffer.resize(mpeg4frameheader->size());
                if (Read(mpeg4frameheader->offset(), mpeg4frameheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::MPEG4Frame* mpeg4frame = flatbuffers::GetRoot<MPEG4Frame>(buffer.data());
                if (!mpeg4frame || !mpeg4frame->data())
                {
                  // Ignore I guess?
                  continue;
                }

                frameheaders.push_back(std::make_shared<MPEG4FRAMEHEADER>(mpeg4frame->codecindex(), mpeg4frameheader->offset() + (mpeg4frame->data()->data() - buffer.data()), mpeg4frame->data()->size(), mpeg4frame->marker(), mpeg4frame->time(), (mpeg4frame->signature() && mpeg4frame->signature()->size()) ? std::vector<uint8_t>(mpeg4frame->signature()->data(), mpeg4frame->signature()->data() + mpeg4frame->signature()->size()) : std::vector<uint8_t>()));
              }
            }

            std::sort(frameheaders.begin(), frameheaders.end(), [](const std::shared_ptr<FRAMEHEADER>& lhs, const std::shared_ptr<FRAMEHEADER>& rhs) { return (lhs->time_ < rhs->time_); }); // Order the frame headers so the user can index them properly
            videotracks.insert(TRACK(videotrack->index(), videotrack->description() ? videotrack->description()->str() : std::string(), codecs, frameheaders));
          }
        }

        boost::container::flat_set<TRACK> metadatatracks;
        if (recording->metadatatracks())
        {
          metadatatracks.reserve(recording->metadatatracks()->size());
          for (const file::Track* metadatatrack : *recording->metadatatracks())
          {
            if (std::find_if(metadatatracks.cbegin(), metadatatracks.cend(), [metadatatrack](const TRACK& t) { return (t.index_ == metadatatrack->index()); }) != metadatatracks.cend()) // Make sure the index is unique
            {
              promise->set_value(28);
              return;
            }

            // Codecs
            if (metadatatrack->codecs() == nullptr)
            {
              promise->set_value(29);
              return;
            }

            boost::container::flat_set<CODEC> codecs;
            for (const file::Codec* codec : *metadatatrack->codecs())
            {
              if (std::find_if(codecs.cbegin(), codecs.cend(), [codec](const CODEC& c) { return (c.index_ == codec->index()); }) != codecs.cend()) // Make sure the index is unique
              {
                promise->set_value(30);
                return;
              }
              codecs.insert(CODEC(codec->index(), codec->codec(), codec->parameters() ? codec->parameters()->str() : std::string()));
            }

            // Frame headers
            std::vector< std::shared_ptr<FRAMEHEADER> > frameheaders;
            if (metadatatrack->metadataframeheaders())
            {
              for (const file::FrameHeader* metadataframeheader : *metadatatrack->metadataframeheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (metadataframeheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(35);
                  return;
                }

                buffer.resize(metadataframeheader->size());
                if (Read(metadataframeheader->offset(), metadataframeheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::MetadataFrame* metadataframe = flatbuffers::GetRoot<MetadataFrame>(buffer.data());
                if (!metadataframe || !metadataframe->data())
                {
                  // Ignore I guess?
                  continue;
                }

                frameheaders.push_back(std::make_shared<METADATAFRAMEHEADER>(metadataframe->codecindex(), metadataframeheader->offset() + (metadataframe->data()->data() - buffer.data()), metadataframe->data()->size(), metadataframe->time(), metadataframe->metadataframetype(), (metadataframe->signature() && metadataframe->signature()->size()) ? std::vector<uint8_t>(metadataframe->signature()->data(), metadataframe->signature()->data() + metadataframe->signature()->size()) : std::vector<uint8_t>()));
              }
            }

            std::sort(frameheaders.begin(), frameheaders.end(), [](const std::shared_ptr<FRAMEHEADER>& lhs, const std::shared_ptr<FRAMEHEADER>& rhs) { return (lhs->time_ < rhs->time_); }); // Order the frame headers so the user can index them properly
            metadatatracks.insert(TRACK(metadatatrack->index(), metadatatrack->description() ? metadatatrack->description()->str() : std::string(), codecs, frameheaders));
          }
        }

        boost::container::flat_set<TRACK> objectdetectortracks;
        if (recording->objectdetectortracks())
        {
          objectdetectortracks.reserve(recording->objectdetectortracks()->size());
          for (const file::Track* objectdetectortrack : *recording->objectdetectortracks())
          {
            if (std::find_if(objectdetectortracks.cbegin(), objectdetectortracks.cend(), [objectdetectortrack](const TRACK& t) { return (t.index_ == objectdetectortrack->index()); }) != objectdetectortracks.cend()) // Make sure the index is unique
            {
              promise->set_value(28);
              return;
            }

            // Codecs
            if (objectdetectortrack->codecs() == nullptr)
            {
              promise->set_value(29);
              return;
            }

            boost::container::flat_set<CODEC> codecs;
            for (const file::Codec* codec : *objectdetectortrack->codecs())
            {
              if (std::find_if(codecs.cbegin(), codecs.cend(), [codec](const CODEC& c) { return (c.index_ == codec->index()); }) != codecs.cend()) // Make sure the index is unique
              {
                promise->set_value(30);
                return;
              }
              codecs.insert(CODEC(codec->index(), codec->codec(), codec->parameters() ? codec->parameters()->str() : std::string()));
            }

            // Frame headers
            std::vector< std::shared_ptr<FRAMEHEADER> > frameheaders;
            if (objectdetectortrack->objectdetectorframeheaders())
            {
              for (const file::FrameHeader* objectdetectorframeheader : *objectdetectortrack->objectdetectorframeheaders())
              {
                ++currentframe;
                *progress = static_cast<float>(currentframe) / static_cast<float>(totalframes);

                if (!running_)
                {
                  promise->set_value(999);
                  return;
                }

                if (objectdetectorframeheader->offset() > (size_ - (sizeof(MAGIC_BYTES) + sizeof(VERSION) + filesize + sizeof(filesize)))) // Make sure the frame remains inside the correct bounds
                {
                  promise->set_value(35);
                  return;
                }

                buffer.resize(objectdetectorframeheader->size());
                if (Read(objectdetectorframeheader->offset(), objectdetectorframeheader->size(), buffer.data()))
                {
                  promise->set_value(33);
                  return;
                }

                const file::ObjectDetectorFrame* objectdetectorframe = flatbuffers::GetRoot<ObjectDetectorFrame>(buffer.data());
                if (!objectdetectorframe || !objectdetectorframe->data())
                {
                  // Ignore I guess?
                  continue;
                }

                frameheaders.push_back(std::make_shared<OBJECTDETECTORFRAMEHEADER>(objectdetectorframe->codecindex(), objectdetectorframeheader->offset() + (objectdetectorframe->data()->data() - buffer.data()), objectdetectorframe->data()->size(), objectdetectorframe->time(), objectdetectorframe->objectdetectorframetype(), (objectdetectorframe->signature() && objectdetectorframe->signature()->size()) ? std::vector<uint8_t>(objectdetectorframe->signature()->data(), objectdetectorframe->signature()->data() + objectdetectorframe->signature()->size()) : std::vector<uint8_t>()));
              }
            }

            std::sort(frameheaders.begin(), frameheaders.end(), [](const std::shared_ptr<FRAMEHEADER>& lhs, const std::shared_ptr<FRAMEHEADER>& rhs) { return (lhs->time_ < rhs->time_); }); // Order the frame headers so the user can index them properly
            objectdetectortracks.insert(TRACK(objectdetectortrack->index(), objectdetectortrack->description() ? objectdetectortrack->description()->str() : std::string(), codecs, frameheaders));
          }
        }
        recordings.insert(RECORDING(recording->index(), recording->name() ? recording->name()->str() : std::string(), recording->location() ? recording->location()->str() : std::string(), videotracks, boost::container::flat_set<file::TRACK>(), metadatatracks, objectdetectortracks));
      }
      file_.devices_.insert(DEVICE(device->index(), device->name() ? device->name()->str() : std::string(), device->address() ? device->address()->str() : std::string(), device->signingkey() ? device->signingkey()->str() : std::string(), recordings));
    }
    promise->set_value(0); // Success!
  });
  return promise->get_future();
}

void FileRead::Destroy()
{
  running_ = false;
  if (thread_.joinable())
  {
    thread_.join();

  }

#ifdef _WIN32
  if (handle_ != INVALID_HANDLE_VALUE)
  {
    CloseHandle(handle_);
    handle_ = INVALID_HANDLE_VALUE;
  }
#else
  if (handle_ != -1)
  {
    close(handle_);
    handle_ = -1;
  }
#endif
  file_.devices_.clear();
}

int FileRead::Read(const uint64_t offset, const uint64_t size, uint8_t* buffer)
{
  if ((offset + size) > size_)
  {

    return 1;
  }

#ifdef _WIN32
  LARGE_INTEGER tmp;
  tmp.QuadPart = sizeof(MAGIC_BYTES) + sizeof(VERSION) + static_cast<int64_t>(offset);
  if (SetFilePointerEx(handle_, tmp, nullptr, FILE_BEGIN) == 0)
  {

    return 2;
  }

  if (::ReadFile(handle_, buffer, static_cast<DWORD>(size), nullptr, nullptr) == 0)
  {

    return 3;
  }
#else
  if (lseek(handle_, sizeof(MAGIC_BYTES) + sizeof(VERSION) + offset, SEEK_SET) == -1)
  {

    return 2;
  }

  if (read(handle_, buffer, size) == -1)
  {

    return 3;
  }
#endif
  return 0;
}

boost::optional<DEVICE> FileRead::GetDevice(const uint64_t deviceindex) const
{
  boost::container::flat_set<DEVICE>::const_iterator device = std::find_if(file_.devices_.cbegin(), file_.devices_.cend(), [deviceindex](const DEVICE& device) { return (device.index_ == deviceindex); });
  if (device == file_.devices_.cend())
  {

    return boost::none;
  }
  return *device;
}

boost::optional<RECORDING> FileRead::GetRecording(const uint64_t deviceindex, const uint64_t recordingindex) const
{
  boost::container::flat_set<DEVICE>::const_iterator device = std::find_if(file_.devices_.cbegin(), file_.devices_.cend(), [deviceindex](const DEVICE& device) { return (device.index_ == deviceindex); });
  if (device == file_.devices_.cend())
  {

    return boost::none;
  }

  boost::container::flat_set<RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [recordingindex](const RECORDING& recording) { return (recording.index_ == recordingindex); });
  if (recording == device->recordings_.cend())
  {

    return boost::none;
  }
  return *recording;
}

}

