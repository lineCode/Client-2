// mediadescription.cpp
//

///// Includes /////

#include "rtsp/sdp/mediadescription.hpp"

#include <boost/regex.hpp>
#include <network/uri.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

MediaDescription::MediaDescription()
{

}

MediaDescription::MediaDescription(const std::vector<std::string>& fields)
{
  // Parse all session description fields and if we come accross a media description, back out and continue to parse those
  for (const auto& field : fields)
  {
    static const boost::regex regex("[\\s]*([a-zA-Z])[\\s]*=[\\s]*(.*)");
    boost::smatch match;
    if (!boost::regex_search(field, match, regex))
    {

      continue;
    }

    const std::string character = match[1];
    const std::string value = match[2];

    if (character == std::string("m"))
    {
      media_ = Media(field);

    }
    else if (character == std::string("c"))
    {
      connection_.AddConnection(field);

    }
    else if (character == std::string("b"))
    {
      bandwidth_ = Bandwidth(field);

    }
    else if (character == std::string("a"))
    {
      // Parse attributes
      static const boost::regex attributeregex("([\\w]+)[\\s]*:[\\s]*(.*)[\\s]*");
      if (!boost::regex_search(value, match, attributeregex))
      {

        continue;
      }

      const std::string key = match[1];
      if (key == std::string("framerate"))
      {
        try
        {
          framerate_ = std::stod(match[2]);

        }
        catch (...)
        {


        }
      }
      else if (key == std::string("range"))
      {
        range_ = sdp::Range(field);

      }
      else if (key == std::string("rtpmap"))
      {
        rtpmap_.AddRtpMap(field);

      }
      else if (key == std::string("control"))
      {
        control_ = match[2];

      }
      else if (key == std::string("fmtp"))
      {
        fmtp_.AddFmtp(field);

      }
    }
  }
}

MediaDescription::MediaDescription(const Media& media, const Connection& connection, const boost::optional<Bandwidth>& bandwidth, const boost::optional<double>& framerate, const boost::optional<Range>& range, const RtpMap& rtpmap, const std::string& control, const Fmtp& fmtp) :
  media_(media),
  connection_(connection),
  bandwidth_(bandwidth),
  framerate_(framerate),
  range_(range),
  rtpmap_(rtpmap),
  control_(control),
  fmtp_(fmtp)
{

}

MediaDescription::~MediaDescription()
{

}

void MediaDescription::Clear()
{
  media_.reset();
  connection_.Clear();
  bandwidth_.reset();
  framerate_.reset();
  range_.reset();
  rtpmap_.Clear();
  control_.clear();
  fmtp_.Clear();
}

bool MediaDescription::IsValid() const
{
  return (media_.is_initialized() && media_->IsValid() && rtpmap_.IsValid() && !control_.empty() && fmtp_.IsValid());
}

std::vector<std::string> MediaDescription::ToString() const
{
  if (!IsValid())
  {

    return std::vector<std::string>();
  }

  std::vector<std::string> result;

  result.push_back(media_->ToString());

  const std::vector<std::string> connections = connection_.ToString();
  result.insert(result.end(), connections.begin(), connections.end());

  if (bandwidth_.is_initialized())
  {
    result.push_back(bandwidth_->ToString());

  }

  if (framerate_.is_initialized())
  {
    result.push_back(std::string("a=framerate:") + std::to_string(*framerate_));

  }

  if (range_.is_initialized())
  {
    result.push_back("a=range:" + range_->ToString());

  }

  const std::vector<std::string> rtpmaps = rtpmap_.ToString();
  result.insert(result.end(), rtpmaps.begin(), rtpmaps.end());

  if (!control_.empty())
  {
    result.push_back(std::string("a=control:") + control_);

  }

  const std::vector<std::string> fmtps = fmtp_.ToString();
  result.insert(result.end(), fmtps.begin(), fmtps.end());

  return result;
}

std::string MediaDescription::GetControl(const std::string& contentbase) const
{
  std::string delimiter;
  if (contentbase.size() && contentbase.back() != '/')
  {
    delimiter = '/';

  }

  try
  {
    if (network::uri(control_).has_scheme())
    {

      return control_;
    }
    else
    {

      return (contentbase + delimiter + control_);
    }
  }
  catch (...)
  {

    return (contentbase + delimiter + control_);
  }
}

bool MediaDescription::operator==(const MediaDescription& rhs) const
{
  return ((media_ == rhs.media_) && (connection_ == rhs.connection_) && (bandwidth_ == rhs.bandwidth_) && (framerate_ == rhs.framerate_) && (range_ == rhs.range_) && (rtpmap_ == rhs.rtpmap_) && (fmtp_ == rhs.fmtp_));
}

}

}
