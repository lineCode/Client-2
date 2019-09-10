// bandwidth.cpp
//

///// Includes /////

#include "rtsp/sdp/bandwidth.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Bandwidth::Bandwidth() :
  bandwidthtype_(BANDWIDTHTYPE_INVALID),
  bandwidth_(0)
{

}

Bandwidth::Bandwidth(const std::string& text) :
  bandwidthtype_(BANDWIDTHTYPE_INVALID),
  bandwidth_(0)
{
  static const boost::regex regex("[\\s]*b[\\s]*=[\\s]*([\\w]*):([\\w]*)[\\s]*");
  boost::smatch match;
  if (!boost::regex_search(text, match, regex))
  {

    return;
  }

  bandwidthtype_ = BandwidthTypeFromString(match[1]);
  bandwidth_ = std::stoi(match[2]);
}

Bandwidth::Bandwidth(BANDWIDTHTYPE bandwidthtype, int bandwidth) :
  bandwidthtype_(bandwidthtype),
  bandwidth_(bandwidth)
{

}

Bandwidth::~Bandwidth()
{

}

void Bandwidth::Clear()
{
  bandwidthtype_ = BANDWIDTHTYPE_INVALID;
  bandwidth_ = 0;
}

bool Bandwidth::IsValid() const
{
  return ((bandwidthtype_ != BANDWIDTHTYPE_INVALID) && (bandwidth_ > 0));
}

std::string Bandwidth::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  return (std::string("b=") + sdp::ToString(bandwidthtype_) + std::string(":") + std::to_string(bandwidth_));
}

bool Bandwidth::operator==(const Bandwidth& rhs) const
{
  return ((bandwidthtype_ == rhs.bandwidthtype_) && (bandwidth_ == rhs.bandwidth_));
}

}

}
