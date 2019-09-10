// rtpmap.hpp
//

#ifndef IDXY4E7EDWNQBR5SYGJNVZ9G2BAEVLX48O
#define IDXY4E7EDWNQBR5SYGJNVZ9G2BAEVLX48O

///// Includes /////

#include <boost/optional.hpp>
#include <map>
#include <string>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Structures /////

struct RTPMAPDATA
{
  RTPMAPDATA() :
    clockrate_(0)
  {

  }

  RTPMAPDATA(const std::string& codec, int clockrate, const boost::optional<int>& numchannels) :
    codec_(codec),
    clockrate_(clockrate),
    numchannels_(numchannels)
  {

  }

  bool IsValid() const
  {
    return !codec_.empty();
  }

  bool operator==(const RTPMAPDATA& rhs) const
  {
    return ((codec_ == rhs.codec_) && (clockrate_ == rhs.clockrate_) && (numchannels_ == rhs.numchannels_));
  }

  std::string codec_;
  int clockrate_;
  boost::optional<int> numchannels_;
};

///// Classes /////

class RtpMap
{
public:

  RtpMap();
  RtpMap(const std::map<int, RTPMAPDATA>& rtpmaps);
  RtpMap(const std::vector<std::string>& rtpmaps);
  ~RtpMap();

  void AddRtpMap(const std::string& rtpmap);

  void Clear();
  bool IsValid() const;
  std::vector<std::string> ToString() const;

  bool operator==(const RtpMap& rhs) const;

  std::map<int, RTPMAPDATA> rtpmaps_;

};

}

}

#endif
