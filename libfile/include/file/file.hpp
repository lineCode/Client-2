// file.hpp
//

#ifndef ID4354654654654654XCXXCXVXT0STS5UA
#define ID4354654654654654XCXXCXVXT0STS5UA

///// Includes /////

#include <array>
#include <boost/container/flat_set.hpp>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

#include "file_generated.h"

///// Namespaces /////

namespace file
{

///// Globals /////

const extern uint64_t MAGIC_BYTES;
const extern uint64_t VERSION;

///// Structures /////

struct CODEC
{
  CODEC(const uint64_t index, const int codec, const std::string& parameters);

  bool operator<(const CODEC& rhs) const;
  bool operator==(const CODEC& rhs) const;

  uint64_t index_;
  int codec_;
  std::string parameters_;

};

struct FRAMEHEADER
{
  FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature);
  virtual ~FRAMEHEADER();

  virtual FrameHeaderType Type() = 0;

  uint64_t codecindex_;
  uint64_t offset_;
  uint64_t size_;
  bool marker_;
  uint64_t time_;
  std::vector<uint8_t> signature_;

};

struct H265FRAMEHEADER : public FRAMEHEADER
{
  H265FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature, const bool donlfield, const std::vector<uint32_t>& offsets);

  virtual FrameHeaderType Type() { return FrameHeaderType::H265; };

  bool donlfield_;
  std::vector<uint32_t> offsets_;

};

struct H264FRAMEHEADER : public FRAMEHEADER
{
  H264FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature, const std::vector<uint32_t>& offsets);

  virtual FrameHeaderType Type() { return FrameHeaderType::H264; };

  std::vector<uint32_t> offsets_;

};

struct JPEGFRAMEHEADER : public FRAMEHEADER
{
  JPEGFRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const uint64_t time, const std::vector<uint8_t>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const std::array<uint8_t, 64>& lqt, const std::array<uint8_t, 64>& cqt);

  virtual FrameHeaderType Type() { return FrameHeaderType::JPEG; };

  uint16_t restartinterval_;
  uint32_t typespecificfragmentoffset_;
  uint8_t type_;
  uint8_t q_;
  uint8_t width_;
  uint8_t height_;
  std::array<uint8_t, 64> lqt_;
  std::array<uint8_t, 64> cqt_;

};

struct METADATAFRAMEHEADER : public FRAMEHEADER
{
  METADATAFRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const uint64_t time, const std::vector<uint8_t>& signature);

  virtual FrameHeaderType Type() { return FrameHeaderType::METADATA; };

};

struct MPEG4FRAMEHEADER : public FRAMEHEADER
{
  MPEG4FRAMEHEADER(const uint64_t codecindex, const uint64_t offset, const uint64_t size, const bool marker, const uint64_t time, const std::vector<uint8_t>& signature);

  virtual FrameHeaderType Type() { return FrameHeaderType::MPEG4; };

};

struct TRACK
{
  TRACK();
  TRACK(const uint64_t index, const std::string& description, const boost::container::flat_set<CODEC>& codecs);
  TRACK(const uint64_t index, const std::string& description, const boost::container::flat_set<CODEC>& codecs, const std::vector< std::shared_ptr<FRAMEHEADER> >& frameheaders);

  std::vector< std::pair<uint64_t, uint64_t> > GetIndices() const;

  bool operator<(const TRACK& rhs) const;
  bool operator==(const TRACK& rhs) const;

  uint64_t index_;
  std::string description_;
  boost::container::flat_set<CODEC> codecs_;
  std::vector< std::shared_ptr<FRAMEHEADER> > frameheaders_;

};

struct RECORDING
{
  RECORDING();
  RECORDING(const uint64_t index, const std::string& name, const std::string& location);
  RECORDING(const uint64_t index, const std::string& name, const std::string& location, const boost::container::flat_set<TRACK>& tracks);

  bool operator<(const RECORDING& rhs) const;
  bool operator==(const RECORDING& rhs) const;

  uint64_t index_;
  std::string name_;
  std::string location_;
  boost::container::flat_set<TRACK> tracks_;
  
};

struct DEVICE
{
  DEVICE();
  DEVICE(const uint64_t index, const std::string& name, const std::string& address, const std::string& signingkey);
  DEVICE(const uint64_t index, const std::string& name, const std::string& address, const std::string& signingkey, const boost::container::flat_set<RECORDING>& recordings);

  bool operator<(const DEVICE& rhs) const;
  bool operator==(const DEVICE& rhs) const;

  uint64_t index_;
  std::string name_;
  std::string address_;
  std::string signingkey_;
  boost::container::flat_set<RECORDING> recordings_;

};

struct FILE
{
  FILE();
  FILE(const boost::container::flat_set<DEVICE>& devices);

  boost::container::flat_set<DEVICE> devices_;

};

}

#endif
