// rtpinfo.cpp
//

///// Includes /////

#include "rtsp/headers/rtpinfo.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

RtpInfo::RtpInfo()
{

}

RtpInfo::RtpInfo(const std::string& rtpinfo)
{
  // Get all the rtp infos removing the field header
  static const boost::regex regex("[\\s]*RTP-Info[\\s]*:[\\s]*(.*)");
  boost::smatch match;
  if (!boost::regex_search(rtpinfo, match, regex))
  {

    return;
  }

  // Split the individual rtp info data
  std::vector<std::string> rtpinfos;
  boost::algorithm::split_regex(rtpinfos, match[1].str(), boost::regex("[\\s]*,[\\s]*"));

  // Parse the rtpinfos
  for (const auto& i : rtpinfos)
  {
    static const boost::regex rtpregex("[\\s]*url[\\s]*=[\\s]*(.+)[\\s]*;[\\s]*seq[\\s]*=[\\s]*([\\d]+)[\\s]*(?:;[\\s]*rtptime[\\s]*=[\\s]*([\\d]+)[\\s]*)?");
    if (!boost::regex_search(i, match, rtpregex))
    {

      return;
    }

    RTPINFODATA rtpinfodata;
    rtpinfodata.url_ = match[1];
    rtpinfodata.seq_ = std::stoull(match[2]);
    if (match[3].matched)
    {
      rtpinfodata.rtptime_ = std::stoull(match[3]);

    }

    rtpinfo_.push_back(rtpinfodata);
  }
}

RtpInfo::RtpInfo(const std::vector<RTPINFODATA>& rtpinfo) :
  rtpinfo_(rtpinfo)
{

}

RtpInfo::~RtpInfo()
{

}

void RtpInfo::Clear()
{
  rtpinfo_.clear();

}

bool RtpInfo::IsValid() const
{
  for (const auto& rtpinfo : rtpinfo_)
  {
    if (!rtpinfo.IsValid())
    {

      return false;
    }
  }

  return true;
}

std::string RtpInfo::ToString() const
{
  if (rtpinfo_.empty())
  {

    return std::string();
  }

  std::vector<std::string> rtpinfos;
  for (const auto& rtpinfo : rtpinfo_)
  {
    rtpinfos.push_back(rtpinfo.ToString());

  }

  std::string result = boost::algorithm::join(rtpinfos, std::string(", "));

  return (std::string("RTP-Info: ") + result);
}

bool RtpInfo::operator==(const RtpInfo& rhs) const
{
  return (rtpinfo_ == rhs.rtpinfo_);
}

}

}
