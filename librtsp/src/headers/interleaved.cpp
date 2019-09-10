// interleaved.cpp
//

///// Includes /////

#include "rtsp/headers/interleaved.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

Interleaved::Interleaved()
{

}

Interleaved::Interleaved(const boost::optional<uint8_t>& rtp, const boost::optional<uint8_t>& rtcp) :
  rtp_(rtp),
  rtcp_(rtcp)
{

}

Interleaved::Interleaved(const std::string& text)
{
  static const boost::regex clientportregex("[\\s]*([\\d]+)(?:[\\s]*-[\\s]*([\\d]+)[\\s]*)?");
  boost::smatch match;
  if (boost::regex_search(text, match, clientportregex))
  {
    try
    {
      rtp_ = boost::numeric_cast<uint8_t>(boost::lexical_cast<int>(match[1].str())); // Straight to uint8_t goes to char and fails horridly
      if (match[2].matched)
      {
        rtcp_ = boost::numeric_cast<uint8_t>(boost::lexical_cast<int>(match[2].str()));

      }
    }
    catch (...)
    {

    }
  }
}

Interleaved::~Interleaved()
{

}

std::string Interleaved::ToString() const
{
  std::string result;
  if (rtp_.is_initialized())
  {
    result += std::to_string(*rtp_);
    if (rtcp_.is_initialized())
    {
      result += std::string("-") + std::to_string(*rtcp_);

    }
  }
  return result;
}

Interleaved::operator bool() const
{
  return rtp_.is_initialized();
}

bool Interleaved::operator==(const Interleaved& rhs) const
{
  return ((rtp_ == rhs.rtp_) && (rtcp_ == rhs.rtcp_));
}

}

}
