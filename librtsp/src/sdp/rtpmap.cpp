// rtpmap.cpp
//

///// Includes /////

#include "rtsp/sdp/rtpmap.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

RtpMap::RtpMap()
{

}

RtpMap::RtpMap(const std::map<int, RTPMAPDATA>& rtpmaps) :
  rtpmaps_(rtpmaps)
{

}

RtpMap::RtpMap(const std::vector<std::string>& rtpmaps)
{
  for (const auto& rtpmap : rtpmaps)
  {
    AddRtpMap(rtpmap);

  }
}

RtpMap::~RtpMap()
{

}

void RtpMap::AddRtpMap(const std::string& rtpmap)
{
  static const boost::regex regex("[\\s]*a[\\s]*=[\\s]*rtpmap[\\s]*:[\\s]*([\\d]+)[\\s]*([\\w\\-\\.]+)\\/([\\d]+)(?:\\/([\\d]+))?");
  boost::smatch match;
  if (!boost::regex_search(rtpmap, match, regex))
  {

    return;
  }

  RTPMAPDATA rtpmapdata;
  rtpmapdata.codec_ = match[2];
  rtpmapdata.clockrate_ = std::stoi(match[3]);
  if (match[4].matched)
  {
    rtpmapdata.numchannels_ = std::stoi(match[4]);

  }

  rtpmaps_[std::stoi(match[1])] = rtpmapdata;
}

void RtpMap::Clear()
{
  rtpmaps_.clear();

}

bool RtpMap::IsValid() const
{
  for (const auto& rtpmap : rtpmaps_)
  {
    if (!rtpmap.second.IsValid())
    {

      return false;
    }
  }

  return true;
}

std::vector<std::string> RtpMap::ToString() const
{
  std::vector<std::string> rtpmaps;
  for (const auto& rtpmap : rtpmaps_)
  {
    if (!rtpmap.second.IsValid())
    {

      continue;
    }

    std::string fmtp = std::string("a=rtpmap:") + std::to_string(rtpmap.first) + std::string(" ") + rtpmap.second.codec_ + std::string("/") + std::to_string(rtpmap.second.clockrate_);
    if (rtpmap.second.numchannels_.is_initialized())
    {
      fmtp += std::string("/") + std::to_string(*rtpmap.second.numchannels_);

    }

    rtpmaps.push_back(fmtp);
  }

  return rtpmaps;
}

bool RtpMap::operator==(const RtpMap& rhs) const
{
  return (rtpmaps_ == rhs.rtpmaps_);
}

}

}
