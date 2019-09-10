// rtspresponse.hpp
//

#ifndef IDG0ZY448MYE3VU72JDZAN9KA8VENUP10B
#define IDG0ZY448MYE3VU72JDZAN9KA8VENUP10B

///// Includes /////

#include <boost/date_time.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/optional.hpp>
#include <set>
#include <string>
#include <vector>

#include "headers/authenticationtype.hpp"
#include "headers/contenttype.hpp"
#include "headers/parameters.hpp"
#include "headers/requesttype.hpp"
#include "headers/rtpinfo.hpp"
#include "headers/transport.hpp"
#include "headers/wwwauthenticate.hpp"
#include "sdp/sdp.hpp"

///// Namespaces /////

namespace rtsp
{

///// Globals /////

const unsigned int CONTINUE                            = 100;
const unsigned int OK                                  = 200;
const unsigned int CREATED                             = 201;
const unsigned int LOW_ON_STORAGE_SPACE                = 250;
const unsigned int MULTIPLE_CHOICES                    = 300;
const unsigned int MOVED_PERMANENTLY                   = 301;
const unsigned int MOVED_TEMPORARILY                   = 302;
const unsigned int SEE_OTHER                           = 303;
const unsigned int NOT_MODIFIED                        = 304;
const unsigned int USE_PROXY                           = 305;
const unsigned int BAD_REQUEST                         = 400;
const unsigned int UNAUTHORIZED                        = 401;
const unsigned int PAYMENT_REQUIRED                    = 402;
const unsigned int FORBIDDEN                           = 403;
const unsigned int NOT_FOUND                           = 404;
const unsigned int METHOD_NOT_ALLOWED                  = 405;
const unsigned int NOT_ACCEPTABLE                      = 406;
const unsigned int PROXY_AUTHENTICATION_REQUIRED       = 407;
const unsigned int REQUEST_TIMEOUT                     = 408;
const unsigned int GONE                                = 410;
const unsigned int LENGTH_REQUIRED                     = 411;
const unsigned int PRECONDITION_FAILED                 = 412;
const unsigned int REQUEST_ENTITY_TOO_LARGE            = 413;
const unsigned int REQUEST_URI_TOO_LARGE               = 414;
const unsigned int UNSUPPORTED_MEDIA_TYPE              = 415;
const unsigned int PARAMETER_NOT_UNDERSTOOD            = 451;
const unsigned int CONFERENCE_NOT_FOUND                = 452;
const unsigned int NOT_ENOUGH_BANDWIDTH                = 453;
const unsigned int SESSION_NOT_FOUND                   = 454;
const unsigned int METHOD_NOT_VALID_IN_THIS_STATE      = 455;
const unsigned int HEADER_FIELD_NOT_VALID_FOR_RESOURCE = 456;
const unsigned int INVALID_RANGE                       = 457;
const unsigned int PARAMETER_IS_READONLY               = 458;
const unsigned int AGGREGATE_OPERATION_NOT_ALLOWED     = 459;
const unsigned int ONLY_AGGREGATE_OPERATION_ALLOWED    = 460;
const unsigned int UNSUPPORTED_TRANSPORT               = 461;
const unsigned int DESTINATION_UNREACHABLE             = 462;
const unsigned int KEY_MANAGEMENT_FAILURE              = 463;
const unsigned int INTERNAL_SERVER_ERROR               = 500;
const unsigned int NOT_IMPLEMENTED                     = 501;
const unsigned int BAD_GATEWAY                         = 502;
const unsigned int SERVICE_UNAVAILABLE                 = 503;
const unsigned int GATEWAY_TIMEOUT                     = 504;
const unsigned int RTSP_VERSION_NOT_SUPPORTED          = 505;
const unsigned int OPTION_NOT_SUPPORTED                = 551;

///// Classes /////

class RtspResponse
{
 public:

  RtspResponse();
  RtspResponse(const std::string& text);
  RtspResponse(unsigned int code, uint64_t cseq, const boost::container::flat_set<headers::REQUESTTYPE>& options, const boost::posix_time::ptime& date, const std::string& realm, const std::string& nonce, bool stale, const std::string& contentbase, headers::CONTENTTYPE contenttype, const boost::optional<unsigned long>& contentlength, const std::string& session, unsigned int timeout, const headers::RtpInfo& rtpinfo);
  RtspResponse(unsigned int code, uint64_t cseq);
  RtspResponse(unsigned int code, uint64_t cseq, const boost::container::flat_set<headers::REQUESTTYPE>& options);
  RtspResponse(unsigned int code, uint64_t cseq, const headers::CONTENTTYPE contenttype, const sdp::Sdp& sdp);
  RtspResponse(unsigned int code, uint64_t cseq, const headers::Transport& transport, const std::string& session, const unsigned int timeout);
  RtspResponse(unsigned int code, uint64_t cseq, const headers::CONTENTTYPE contenttype, const headers::Parameters& parameters);
  RtspResponse(unsigned int code, uint64_t cseq, const std::string& session, const headers::RtpInfo& rtpinfo);
  RtspResponse(unsigned int code, uint64_t cseq, const std::string& session);
  ~RtspResponse();

  headers::WWWAuthenticate GetStrongestAuthentication() const;

  void ParseContent(const std::string& content);

  bool IsValid() const;

  std::string ToString() const; // This will ignore the currently set content-length and use it's own internally calculated value

  bool operator==(const RtspResponse& rhs) const;

  boost::optional<unsigned int> code_;
  std::string message_;
  boost::optional<uint64_t> cseq_;
  boost::container::flat_set<headers::REQUESTTYPE> options_;
  boost::posix_time::ptime date_; // http://www.w3.org/Protocols/rfc2616/rfc2616.txt: "All HTTP date/time stamps MUST be represented in Greenwich Mean Time(GMT), without exception"
  std::vector<headers::WWWAuthenticate> wwwauthenticate_;
  std::string contentbase_;
  headers::CONTENTTYPE contenttype_;
  boost::optional<unsigned long> contentlength_;
  std::string session_;
  unsigned int timeout_; // seconds, default is 60
  headers::Transport transport_;
  headers::RtpInfo rtpinfo_;

  boost::optional<sdp::Sdp> sdp_;
  headers::Parameters parameters_;

 private:

  std::string Message(const unsigned int code) const;

};

}

#endif
