// media.cpp
//

///// Includes /////

#include "rtsp/sdp/media.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Media::Media()
{

}

Media::Media(const std::string& text)
{
  static const boost::regex tokensregex("[\\s]*m[\\s]*=[\\s]*([\\w]+)[\\s]+([\\d]+)(?:\\/([\\d]+))?[\\s]+([\\w\\/]+)[\\s]+(.*)");
  boost::smatch match;
  if (!boost::regex_search(text, match, tokensregex))
  {

    return;
  }

  mediatype_ = MediaTypeFromString(match[1]);
  port_ = std::stoi(match[2]);

  if (match[3].matched)
  {
    numports_ = std::stoi(match[3]);

  }
  else
  {
    numports_ = 1;

  }

  mediaprotocol_ = MediaProtocolFromString(match[4]);

  static const boost::regex formatsregex("[\\d]+");
  std::string formats = match[5];
  for (boost::sregex_token_iterator i(formats.begin(), formats.end(), formatsregex); i != boost::sregex_token_iterator(); ++i)
  {
    formats_.push_back(std::stoi(i->str()));

  }
}

Media::Media(MEDIATYPE mediatype, const boost::optional<uint16_t>& port, const boost::optional<uint16_t>& numports, MEDIAPROTOCOL mediaprotocol, const std::vector<unsigned int>& formats) :
  mediatype_(mediatype),
  port_(port),
  numports_(numports),
  mediaprotocol_(mediaprotocol),
  formats_(formats)
{

}

Media::~Media()
{

}

void Media::Clear()
{
  mediatype_.reset();
  port_.reset();
  numports_.reset();
  mediaprotocol_.reset();
  formats_.clear();
}

bool Media::IsValid() const
{
  return (mediatype_.is_initialized() && (mediatype_ != MEDIATYPE_INVALID) && mediaprotocol_.is_initialized() && (mediaprotocol_ != MEDIAPROTOCOL_INVALID) && !formats_.empty());
}

std::string Media::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  // Ports
  uint16_t port = 0;
  if (port_.is_initialized())
  {
    port = *port_;

  }

  uint16_t numports = 1;
  if (numports_.is_initialized())
  {
    numports = numports;

  }

  std::string ports;
  if (numports == 1)
  {
    ports = std::to_string(port);

  }
  else
  {
    ports = std::to_string(port) + std::string("/") + std::to_string(numports);

  }

  // Formats
  std::vector<std::string> formats;
  for (const auto format : formats_)
  {
    formats.push_back(std::to_string(format));

  }

  return (std::string("m=") + sdp::ToString(*mediatype_) + std::string(" ") + ports + std::string(" ") + sdp::ToString(*mediaprotocol_) + std::string(" ") + boost::algorithm::join(formats, std::string(" ")));
}

bool Media::operator==(const Media& rhs) const
{
  return ((mediatype_ == rhs.mediatype_) && (port_ == rhs.port_) && (numports_ == rhs.numports_) && (mediaprotocol_ == rhs.mediaprotocol_) && (formats_ == rhs.formats_));
}

}

}
