// file.cpp
//

///// Includes /////

#include "file/file.hpp"

///// Namespaces /////

namespace file
{

///// Globals /////

const uint64_t MAGIC_BYTES = 0b1110110100001110010011000010001011001001101100110111000101110000;
const uint64_t VERSION = 1;

///// Methods /////

CODEC::CODEC(const uint64_t index, const int codec, const std::string& parameters) :
  index_(index),
  codec_(codec),
  parameters_(parameters)
{

}

bool CODEC::operator<(const CODEC& rhs) const
{
  return (index_ < rhs.index_);
}

bool CODEC::operator==(const CODEC& rhs) const
{
  return ((index_ == rhs.index_) && (codec_ == rhs.codec_) && (parameters_ == rhs.parameters_));
}

FRAMEHEADER::FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature) :
  codecindex_(codecindex),
  offset_(offset),
  size_(size),
  marker_(marker),
  time_(time),
  signature_(signature)
{

}

FRAMEHEADER::~FRAMEHEADER()
{

}

H265FRAMEHEADER::H265FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature, const bool donlfield, const std::vector<uint32_t>& offsets) :
  FRAMEHEADER(codecindex, offset, size, marker, time, signature),
  donlfield_(donlfield),
  offsets_(offsets)
{

}

H264FRAMEHEADER::H264FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature, const std::vector<uint32_t>& offsets) :
  FRAMEHEADER(codecindex, offset, size, marker, time, signature),
  offsets_(offsets)
{

}

JPEGFRAMEHEADER::JPEGFRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const uint64_t time, const std::vector<uint8_t>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const std::array<uint8_t, 64>& lqt, const std::array<uint8_t, 64>& cqt) :
  FRAMEHEADER(codecindex, offset, size, true, time, signature),
  restartinterval_(restartinterval),
  typespecificfragmentoffset_(typespecificfragmentoffset),
  type_(type),
  q_(q),
  width_(width),
  height_(height),
  lqt_(lqt),
  cqt_(cqt)
{

}

METADATAFRAMEHEADER::METADATAFRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const uint64_t time, const file::MetadataFrameType metadataframetype, const std::vector<uint8_t>& signature) :
  FRAMEHEADER(codecindex, offset, size, true, time, signature),
  metadataframetype_(metadataframetype)
{

}

MPEG4FRAMEHEADER::MPEG4FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature) :
  FRAMEHEADER(codecindex, offset, size, marker, time, signature)
{

}

TRACK::TRACK() :
  index_(0)
{

}

TRACK::TRACK(const uint64_t index, const std::string& description, const boost::container::flat_set<CODEC>& codecs) :
  index_(index),
  description_(description),
  codecs_(codecs)
{

}

TRACK::TRACK(const uint64_t index, const std::string& description, const boost::container::flat_set<CODEC>& codecs, const std::vector< std::shared_ptr<FRAMEHEADER> >& frameheaders) :
  index_(index),
  description_(description),
  codecs_(codecs),
  frameheaders_(std::move(frameheaders))
{

}

std::vector< std::pair<uint64_t, uint64_t> > TRACK::GetIndices() const
{
  std::vector< std::pair<uint64_t, uint64_t> > indices;
  for (const std::shared_ptr<FRAMEHEADER>& frameheader : frameheaders_)
  {
    if (indices.empty())
    {
      indices.push_back(std::make_pair(frameheader->time_, frameheader->time_));

    }
    else if ((frameheader->time_ - indices.back().second) < 5000)
    {
      indices.back().second = frameheader->time_;

    }
    else
    {
      indices.push_back(std::make_pair(frameheader->time_, frameheader->time_));

    }
  }
  return indices;
}

bool TRACK::operator<(const TRACK& rhs) const
{
  return (index_ < rhs.index_);
}

bool TRACK::operator==(const TRACK& rhs) const
{
  return ((index_ == rhs.index_) && (description_ == rhs.description_) && (codecs_ == rhs.codecs_));
}

RECORDING::RECORDING() :
  index_(0)
{

}

RECORDING::RECORDING(const uint64_t index, const std::string& name, const std::string& location) :
  index_(index),
  name_(name),
  location_(location)
{

}

RECORDING::RECORDING(const uint64_t index, const std::string& name, const std::string& location, const boost::container::flat_set<TRACK>& videotracks, const boost::container::flat_set<TRACK>& audiotracks, const boost::container::flat_set<TRACK>& metadatatracks) :
  index_(index),
  name_(name),
  location_(location),
  videotracks_(videotracks),
  audiotracks_(audiotracks),
  metadatatracks_(metadatatracks)
{

}

boost::container::flat_set<TRACK> RECORDING::GetTracks() const
{
  boost::container::flat_set<TRACK> tracks;
  tracks.reserve(videotracks_.size() + audiotracks_.size() + metadatatracks_.size());
  tracks.insert(videotracks_.begin(), videotracks_.end());
  tracks.insert(audiotracks_.begin(), audiotracks_.end());
  tracks.insert(metadatatracks_.begin(), metadatatracks_.end());
  return tracks;
}

bool RECORDING::operator<(const RECORDING& rhs) const
{
  return (index_ < rhs.index_);
}

bool RECORDING::operator==(const RECORDING& rhs) const
{
  return ((index_ == rhs.index_) &&
          (name_ == rhs.name_) &&
          (location_ == rhs.location_) &&
          std::is_permutation(videotracks_.cbegin(), videotracks_.cend(), rhs.videotracks_.cbegin(), rhs.videotracks_.cend()) &&
          std::is_permutation(audiotracks_.cbegin(), audiotracks_.cend(), rhs.audiotracks_.cbegin(), rhs.audiotracks_.cend()) &&
          std::is_permutation(metadatatracks_.cbegin(), metadatatracks_.cend(), rhs.metadatatracks_.cbegin(), rhs.metadatatracks_.cend()));
}

DEVICE::DEVICE() :
  index_(0)
{

}

DEVICE::DEVICE(const uint64_t index, const std::string& name, const std::string& address, const std::string& signingkey) :
  index_(index),
  name_(name),
  address_(address),
  signingkey_(signingkey)
{

}

DEVICE::DEVICE(const uint64_t index, const std::string& name, const std::string& address, const std::string& signingkey, const boost::container::flat_set<RECORDING>& recordings) :
  index_(index),
  name_(name),
  address_(address),
  signingkey_(signingkey),
  recordings_(recordings)
{

}

bool DEVICE::operator<(const DEVICE& rhs) const
{
  return (index_ < rhs.index_);
}

bool DEVICE::operator==(const DEVICE& rhs) const
{
  return ((index_ == rhs.index_) && (signingkey_ == rhs.signingkey_) && (recordings_ == rhs.recordings_));
}

FILE::FILE()
{

}

FILE::FILE(const boost::container::flat_set<DEVICE>& devices) :
  devices_(devices)
{

}

}
