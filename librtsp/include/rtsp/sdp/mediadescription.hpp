// mediadescription.hpp
//

#ifndef IDD4JWOHH0JW9MB2TI6RVE9XLEA22Q3JC3
#define IDD4JWOHH0JW9MB2TI6RVE9XLEA22Q3JC3

///// Includes /////

#include <boost/optional.hpp>
#include <string>
#include <vector>

#include "bandwidth.hpp"
#include "connection.hpp"
#include "fmtp.hpp"
#include "media.hpp"
#include "range.hpp"
#include "rtpmap.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class MediaDescription
{
 public:

  MediaDescription();
  MediaDescription(const std::vector<std::string>& fields);
  MediaDescription(const Media& media, const Connection& connection, const boost::optional<Bandwidth>& bandwidth, const boost::optional<double>& framerate, const boost::optional<Range>& range, const RtpMap& rtpmap, const std::string& control, const Fmtp& fmtp);
  ~MediaDescription();

  void Clear();
  bool IsValid() const;
  std::vector<std::string> ToString() const;

  std::string GetControl(const std::string& contentbase) const; // This will attempt to get the control url, give all the inputs

  bool operator==(const MediaDescription& rhs) const;

  boost::optional<Media> media_;
  Connection connection_;
  boost::optional<Bandwidth> bandwidth_;
  boost::optional<double> framerate_; // In frames per second
  boost::optional<Range> range_;
  RtpMap rtpmap_;
  std::string control_;
  Fmtp fmtp_;

};

}

}

#endif
