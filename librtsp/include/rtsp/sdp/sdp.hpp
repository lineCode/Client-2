// sdp.hpp
//

#ifndef IDO0BXTNYIKF5PPC95NGEY73FZ9JDGY337
#define IDO0BXTNYIKF5PPC95NGEY73FZ9JDGY337

///// Includes /////

#include <boost/date_time.hpp>
#include <boost/optional.hpp>
#include <stdint.h>
#include <string>
#include <vector>

#include "rtsp/headers/authenticationtype.hpp"
#include "rtsp/headers/contenttype.hpp"
#include "rtsp/headers/requesttype.hpp"
#include "rtsp/sdp/bandwidth.hpp"
#include "rtsp/sdp/mediadescription.hpp"
#include "rtsp/sdp/origin.hpp"
#include "rtsp/sdp/range.hpp"
#include "rtsp/sdp/sdp.hpp"
#include "rtsp/sdp/timing.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Sdp
{
 public:

  Sdp();
  Sdp(const std::string& content);
  Sdp(const std::vector<std::string>& fields);
  Sdp(int version, const Origin& origin, const std::string& sessionname, const boost::optional<std::string>& email, const Bandwidth& bandwidth, const Timing& timing, const std::string& control, const std::vector<Range>& ranges, const std::vector<MediaDescription>& mediadescriptions);
  ~Sdp();

  inline void SetMediaDescriptions(const std::vector<MediaDescription>& mediadescriptions) { mediadescriptions_ = mediadescriptions; }
  const std::vector<MediaDescription>& GetMediaDescriptions() const { return mediadescriptions_; }
  std::vector<MediaDescription>& GetMediaDescriptions() { return mediadescriptions_; }
  std::vector<MediaDescription> GetMediaDescriptions(MEDIATYPE mediatype) const;
  boost::optional<MediaDescription> GetMediaDescription(MEDIATYPE mediatype, const std::string& codec) const;

  void Clear();
  bool IsValid() const;
  std::vector<std::string> ToString() const;

  bool operator==(const Sdp& rhs) const;

  boost::optional<int> version_;
  boost::optional<Origin> origin_;
  boost::optional<std::string> sessionname_;
  boost::optional<std::string> email_;
  boost::optional<Bandwidth> bandwidth_;
  Timing timing_;
  std::string control_;
  std::vector<Range> ranges_;
  std::vector<MediaDescription> mediadescriptions_;

};

}

}

#endif
