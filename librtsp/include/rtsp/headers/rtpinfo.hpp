// rtpinfo.hpp
//

#ifndef ID2BL9YOCYIATR428CG5JUPSDI5EGIW452
#define ID2BL9YOCYIATR428CG5JUPSDI5EGIW452

///// Includes /////

#include <boost/optional.hpp>
#include <stdint.h>
#include <string>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Structures /////

struct RTPINFODATA
{
  RTPINFODATA() :
    seq_(0)
  {

  }

  RTPINFODATA(const std::string& url, int seq, const boost::optional<uint64_t>& rtptime) :
    url_(url),
    seq_(seq),
    rtptime_(rtptime)
  {

  }

  bool IsValid() const
  {
    if (url_.empty())
    {

      return false;
    }

    return true;
  }

  std::string ToString() const
  {
    if (!IsValid())
    {

      return std::string();
    }

    return (std::string("url=") + url_ + std::string(";seq=") + std::to_string(seq_) + std::string(";") + (rtptime_.is_initialized() ? (std::string("rtptime=") + std::to_string(*rtptime_)) : std::string()));
  }

  bool operator==(const RTPINFODATA& rhs) const
  {
    return ((url_ == rhs.url_) && (seq_ == rhs.seq_) && (rtptime_ == rhs.rtptime_));
  }

  std::string url_;
  uint64_t seq_;
  boost::optional<uint64_t> rtptime_;
};

///// Classes /////

class RtpInfo
{
public:

  RtpInfo();
  RtpInfo(const std::string& rtpinfo);
  RtpInfo(const std::vector<RTPINFODATA>& rtpinfo);
  ~RtpInfo();

  void Clear();
  bool IsValid() const;
  std::string ToString() const;

  bool operator==(const RtpInfo& rhs) const;

  std::vector<RTPINFODATA> rtpinfo_;

};

}

}

#endif
