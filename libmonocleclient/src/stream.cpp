// stream.cpp
//

///// Includes /////

#include "monocleclient/stream.h"

#include <rtsp/rtsp.hpp>

///// Namespaces /////

namespace client
{

///// Globals /////

// H264
const uint8_t STARTSEQUENCE[] = { 0, 0, 0, 1 };

///// Functions /////

int BuildH265Frame(const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size, std::vector<unsigned char>& buffer)
{
  buffer.clear();
  for (int i = -1; i < static_cast<int>(numoffsets); ++i)
  {
    // Offset zero doesn't get included, so we hack it in here
    size_t offset = 0;
    if (i >= 0)
    {
      offset = offsets[i];

    }
    
    if (offset >= size)
    {
      // Ignore illegal packet
      return 1;
    }
    const char* dataoffset = data + offset;
    size_t datasize;
    if (i == (numoffsets - 1))
    {
      datasize = size - offset;

    }
    else
    {
      datasize = offsets[i + 1] - offset;

    }

    if ((offset + datasize) > size)
    {
      // Ignore illegal frame
      return 2;
    }

    // Parse the H265 into a single buffer with all the bits required...
    const int naltype = (dataoffset[0] >> 1) & 0x3f;
    if (naltype > 50)
    {

      return 3;
    }

    const int lid = ((dataoffset[0] << 5) & 0x20) | ((dataoffset[1] >> 3) & 0x1f);
    if (lid)
    {
      // Currently unsupported(3D video coding extensions)
      return 4;
    }

    const int tid = dataoffset[1] & 0x07;
    if (!tid)
    {
      // Only support this kind of type
      return 5;
    }

    switch (naltype)
    {
      case 48:
      {
        // Ignore this for the time being, no encoders seem to create this at the moment
        break;
      }
      case 49:
      {
        if (datasize < 3)
        {
          // Ignore illegal packets
          return 6;
        }

        if (dataoffset[2] & 0x80)
        {
          buffer.insert(buffer.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
          const char futype = dataoffset[2] & 0x3f;
          buffer.push_back((dataoffset[0] & 0x81) | (futype << 1));
          buffer.push_back(dataoffset[1]);
        }

        dataoffset += 3;
        datasize -= 3;

        if (donlfield)
        {
          dataoffset += 1;
          datasize -= 1;
        }

        buffer.insert(buffer.end(), reinterpret_cast<const unsigned char*>(dataoffset), reinterpret_cast<const unsigned char*>(dataoffset + datasize));
        break;
      }
      case 50:
      {
        // Unsupported
        return 7;
      }
      // case 32: // VPS
      // case 33: // SPS
      // case 34: // PPS
      // case 39: // SEI
      default:
      {
        buffer.insert(buffer.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
        buffer.insert(buffer.end(), reinterpret_cast<const unsigned char*>(dataoffset), reinterpret_cast<const unsigned char*>(dataoffset + datasize));
        break;
      }
    }
  }
  return 0;
}

int BuildH264Frame(const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size, std::vector<unsigned char>& buffer)
{
  buffer.clear();
  for (int i = -1; i < static_cast<int>(numoffsets); ++i)
  {
    // Offset zero doesn't get included, so we hack it in here
    size_t offset = 0;
    if (i >= 0)
    {
      offset = offsets[i];

    }
    
    if (offset >= size)
    {
      // Ignore illegal packet
      return 1;
    }
    const char* dataoffset = data + offset;
    size_t datasize;
    if (i == (numoffsets - 1))
    {
      datasize = size - offset;

    }
    else
    {
      datasize = offsets[i + 1] - offset;

    }

    if ((offset + datasize) > size)
    {
      // Ignore illegal frame
      return 2;
    }

    // Parse the H264 into a single buffer with all the bits required...
    const uint8_t naltype = dataoffset[0] & 0x1f;
    if ((naltype >= 0) && (naltype <= 23)) // Single packet
    {
      buffer.insert(buffer.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
      buffer.insert(buffer.end(), reinterpret_cast<const unsigned char*>(dataoffset), reinterpret_cast<const unsigned char*>(dataoffset + datasize));
    }
    else if (naltype == 24) // STAP-A NAL
    {
      // Ignore this for the moment

    }
    else if (naltype == 28) // Fragmented packet
    {
      const uint8_t start = dataoffset[1] & 0x80;
      if (start)
      {
        buffer.insert(buffer.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
        buffer.push_back((dataoffset[0] & 0xe0) | (dataoffset[1] & 0x1f));
      }
      buffer.insert(buffer.end(), reinterpret_cast<const unsigned char*>(dataoffset + 2), reinterpret_cast<const unsigned char*>(dataoffset + datasize));
    }
    else
    {

      return 1;
    }
  }
  return 0;
}

void JPEGHeader(const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, std::vector<unsigned char>& buffer)
{
  buffer.clear();

  uint16_t ri = 0;
  if ((type >= 0) && (type <= 63)) // Parse restart marker header
  {
    // No extra headers

  }
  else if ((type >= 64) && (type <= 127)) // Parse restart marker header
  {
    ri = restartinterval;

  }
  else
  {
    // Ignore dynamically defined session setup protocols, which are not defined in RFC2435
    return;
  }

  if ((q >= 0) && (q <= 127)) // Generate quantization table header
  {
    uint8_t lqt[64];
    uint8_t cqt[64];
    rtsp::MakeTables(q, lqt, cqt);
    rtsp::MakeHeaders(buffer, type, width, height, lqt, cqt, restartinterval);
  }
  else // ((q >= 128) && (q <= 255)) // Parse quantization table header
  {
    if ((type == 0) || (type == 1) || (type == 64) || (type == 65))
    {
      if ((lqt == nullptr) || (cqt == nullptr))
      {
        // Ignore illegal packet
        return;
      }
      rtsp::MakeHeaders(buffer, type, width, height, lqt, cqt, ri);
    }
    else
    {
      // Ignore packets with unknown types
      return;
    }
  }
}

///// Methods /////

Stream::Stream(const uint64_t token, const CONTROLSTREAMEND controlstreamendcallback, const H265CALLBACK h265callback, const H264CALLBACK h264callback, const METADATACALLBACK metadatacallback, const JPEGCALLBACK jpegcallback, const MPEG4CALLBACK mpeg4callback, const NEWCODECINDEX newcodecindexcallback, void* callbackdata) :
  token_(token),
  controlstreamendcallback_(controlstreamendcallback),
  h265callback_(h265callback),
  h264callback_(h264callback),
  metadatacallback_(metadatacallback),
  jpegcallback_(jpegcallback),
  mpeg4callback_(mpeg4callback),
  newcodecindexcallback_(newcodecindexcallback),
  callbackdata_(callbackdata)
{
  
}

Stream::Stream(const Stream& rhs) :
  token_(rhs.token_),
  controlstreamendcallback_(rhs.controlstreamendcallback_),
  h265callback_(rhs.h265callback_),
  h264callback_(rhs.h264callback_),
  metadatacallback_(rhs.metadatacallback_),
  jpegcallback_(rhs.jpegcallback_),
  mpeg4callback_(rhs.mpeg4callback_),
  newcodecindexcallback_(rhs.newcodecindexcallback_),
  callbackdata_(rhs.callbackdata_)
{

}

Stream::~Stream()
{

}

void Stream::ControlStreamEnd(const uint64_t playrequestindex, const monocle::ErrorCode error)
{
  if (controlstreamendcallback_)
  {
    controlstreamendcallback_(token_, playrequestindex, error, callbackdata_);

  }
}

void Stream::H265Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) const
{
  if (h265callback_)
  {
    if (BuildH265Frame(donlfield, offsets, numoffsets, data, size, buffer_))
    {
      // Ignore packets that suck
      return;
    }

    if (buffer_.empty())
    {
      // Shouldn't happen, but ok...
      return;
    }

    h265callback_(token_, playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, data, size, donlfield, offsets, numoffsets, reinterpret_cast<const char*>(buffer_.data()), buffer_.size(), callbackdata_);
  }
}

void Stream::H264Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) const
{
  if (h264callback_)
  {
    if (BuildH264Frame(offsets, numoffsets, data, size, buffer_))
    {
      // Ignore packets that suck
      return;
    }

    if (buffer_.empty())
    {
      // Shouldn't happen, but ok...
      return;
    }

    h264callback_(token_, playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, data, size, offsets, numoffsets, reinterpret_cast<const char*>(buffer_.data()), buffer_.size(), callbackdata_);
  }
}

void Stream::MetadataFrame(const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) const
{
  if (metadatacallback_)
  {
    metadatacallback_(token_, playrequestindex, codecindex, timestamp, sequencenum, progress, signature, signaturesize, data, size, data, size, callbackdata_);

  }
}

void Stream::JPEGFrame(const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, size_t size) const
{
  if (jpegcallback_)
  {
    if (size < 8)
    {
      // Ignore illegal frame
      return;
    }

    JPEGHeader(restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt, buffer_);
    buffer_.insert(buffer_.end(), data, data + size);
    jpegcallback_(token_, playrequestindex, codecindex, timestamp, sequencenum, progress, signature, signaturesize, data, size, restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt, reinterpret_cast<const char*>(buffer_.data()), buffer_.size(), callbackdata_);
  }
}

void Stream::MPEG4Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) const
{
  if (mpeg4callback_)
  {
    mpeg4callback_(token_, playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, data, size, data, size, callbackdata_);

  }
}

void Stream::NewCodecIndex(const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  if (newcodecindexcallback_)
  {
    newcodecindexcallback_(token_, id, codec, parameters, timestamp, callbackdata_);

  }
}

Stream& Stream::operator=(const Stream& rhs)
{
  token_ = rhs.token_;
  controlstreamendcallback_ = rhs.controlstreamendcallback_;
  h265callback_ = rhs.h265callback_;
  h264callback_ = rhs.h264callback_;
  metadatacallback_ = rhs.metadatacallback_;
  jpegcallback_ = rhs.jpegcallback_;
  mpeg4callback_ = rhs.mpeg4callback_;
  callbackdata_ = rhs.callbackdata_;
  return *this;
}

}
