// sdp.cpp
//

///// Includes /////

#include "rtsp/sdp/sdp.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <deque>
#include <utility/utility.hpp>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Sdp::Sdp()
{

}

Sdp::Sdp(const std::string& content)
{
  // Split the fields by line
  std::vector<std::string> fields;
  boost::algorithm::split_regex(fields, content, boost::regex("\r\n|\r[^\n]|[^\r]\n"));

  // Group around each m=
  std::deque< std::vector<std::string> > descriptions(1);
  for (const auto& field : fields)
  {
    static const boost::regex regex("[\\s]*m[\\s]*=.*");
    if (boost::regex_match(field, regex))
    {
      descriptions.push_back(std::vector<std::string>());

    }
    descriptions.back().push_back(field);
  }

  // Session description
  for (const auto& session : descriptions.front())
  {
    static const boost::regex regex("[\\s]*([a-zA-Z])[\\s]*=[\\s]*(.*)");
    boost::smatch match;
    if (!boost::regex_search(session, match, regex))
    {

      continue;
    }

    const std::string character = match[1];
    const std::string value = match[2];

    if (character == std::string("v"))
    {
      try
      {
        version_ = std::stoi(value);

      }
      catch (...)
      {

      }
    }
    else if (character == std::string("o"))
    {
      try
      {
        origin_ = Origin(session);

      }
      catch (...)
      {

      }
    }
    else if (character == std::string("s"))
    {
      try
      {
        sessionname_ = value;

      }
      catch (...)
      {

      }
    }
    else if (character == std::string("e"))
    {
      try
      {
        email_ = value;

      }
      catch (...)
      {

      }
    }
    else if (character == std::string("b"))
    {
      bandwidth_ = Bandwidth(session);

    }
    else if (character == std::string("t"))
    {
      try
      {
        timing_.AddTiming(session);

      }
      catch (...)
      {

      }
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
      if (key == std::string("control"))
      {
        control_ = match[2];

      }
      else if (key == std::string("range"))
      {
        ranges_.push_back(Range(match[2]));

      }
    }
  }

  descriptions.pop_front();

  // Media description
  for (const auto& media : descriptions)
  {
    mediadescriptions_.push_back(MediaDescription(media));

  }
}

Sdp::Sdp(const std::vector<std::string>& fields) :
  Sdp(boost::algorithm::join(fields, std::string("\r\n")))
{

}

Sdp::Sdp(int version, const Origin& origin, const std::string& sessionname, const boost::optional<std::string>& email, const Bandwidth& bandwidth, const Timing& timing, const std::string& control, const std::vector<Range>& ranges, const std::vector<MediaDescription>& mediadescriptions) :
  version_(version),
  origin_(origin),
  sessionname_(sessionname),
  email_(email),
  bandwidth_(bandwidth),
  timing_(timing),
  control_(control),
  ranges_(ranges),
  mediadescriptions_(mediadescriptions)
{

}

Sdp::~Sdp()
{

}

std::vector<MediaDescription> Sdp::GetMediaDescriptions(MEDIATYPE mediatype) const
{
  std::vector<MediaDescription> mediadescriptions;
  for (const MediaDescription& mediadescription : mediadescriptions_)
  {
    if (mediadescription.media_.is_initialized() && mediadescription.media_->mediatype_.is_initialized() && (*mediadescription.media_->mediatype_ == mediatype))
    {
      mediadescriptions.push_back(mediadescription);

    }
  }
  return mediadescriptions;
}


boost::optional<MediaDescription> Sdp::GetMediaDescription(MEDIATYPE mediatype, const std::string& codec) const
{
  for (const auto& mediadescription : mediadescriptions_)
  {
    if (!mediadescription.media_.is_initialized() || !mediadescription.media_->mediatype_.is_initialized())
    {

      continue;
    }

    if (*mediadescription.media_->mediatype_ == mediatype)
    {
      for (const auto& rtpmap : mediadescription.rtpmap_.rtpmaps_)
      {
        if (rtpmap.second.codec_ == codec)
        {

          return mediadescription;
        }
      }
    }
  }

  return boost::none;
}

void Sdp::Clear()
{
  version_.reset();
  origin_.reset();
  sessionname_.reset();
  email_.reset();
  bandwidth_.reset();
  timing_.Clear();
  control_.clear();
  mediadescriptions_.clear();
}

bool Sdp::IsValid() const
{
  return (version_.is_initialized() && origin_.is_initialized() && origin_->IsValid() && sessionname_.is_initialized() && !sessionname_->empty() && !timing_.timings_.empty());
}

std::vector<std::string> Sdp::ToString() const
{
  if (!IsValid())
  {

    return std::vector<std::string>();
  }

  std::vector<std::string> result;
  if (version_.is_initialized())
  {
    result.push_back(std::string("v=") + std::to_string(*version_));

  }

  result.push_back(origin_->ToString());

  if (sessionname_.is_initialized())
  {
    result.push_back(std::string("s=") + *sessionname_);

  }

  if (email_.is_initialized() && !email_->empty())
  {
    result.push_back(std::string("e=") + *email_);

  }

  if (bandwidth_.is_initialized() && bandwidth_->IsValid())
  {
    result.push_back(bandwidth_->ToString());

  }

  const std::vector<std::string> timing = timing_.ToString();
  result.insert(result.end(), timing.begin(), timing.end());

  if (!control_.empty())
  {
    result.push_back(std::string("a=control:") + control_);

  }

  for (const auto& range : ranges_)
  {
    result.push_back(std::string("a=range:") + range.ToString());

  }

  for (const auto& i : mediadescriptions_)
  {
    const std::vector<std::string> mediadescription = i.ToString();
    result.insert(result.end(), mediadescription.begin(), mediadescription.end());
  }
  return result;
}

bool Sdp::operator==(const Sdp& rhs) const
{
  return ((version_ == rhs.version_) && (origin_ == rhs.origin_) && (sessionname_ == rhs.sessionname_) && (email_ == rhs.email_) && (bandwidth_ == rhs.bandwidth_) && (timing_ == rhs.timing_) && (control_ == rhs.control_) && std::is_permutation(ranges_.begin(), ranges_.end(), rhs.ranges_.begin(), rhs.ranges_.end()) && (mediadescriptions_ == rhs.mediadescriptions_));
}

}

}
