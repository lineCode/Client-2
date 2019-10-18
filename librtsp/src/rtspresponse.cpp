// rtspresponse.cpp
//

///// Includes /////

#include "rtsp/rtspresponse.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <deque>
#include <utility/utility.hpp>
#include <vector>

#include "rtsp/rtsp.hpp"

///// Namespaces /////

namespace rtsp
{

///// Globals /////

const unsigned int DEFAULT_TIMEOUT = 60;

///// Methods /////

RtspResponse::RtspResponse() :
  contenttype_(headers::CONTENTTYPE_INVALID),
  timeout_(DEFAULT_TIMEOUT)
{

}

RtspResponse::RtspResponse(const std::string& text) :
  contenttype_(headers::CONTENTTYPE_INVALID),
  timeout_(DEFAULT_TIMEOUT)
{
  // Split the header by line
  std::deque<std::string> headers;
  static const boost::regex newline("\r\n|\r[^\n]|[^\r]\n");
  boost::algorithm::split_regex(headers, text, newline);
  if (headers.empty())
  {

    return;
  }

  // Status line
  static const boost::regex statusregex("RTSP\\/1.0[\\s]*([\\d]+)[\\s]*([\\w]+)");
  boost::smatch match;
  if (!boost::regex_search(headers.front(), match, statusregex))
  {

    return;
  }

  code_ = std::stoul(match[1]);
  message_ = match[2];
  headers.pop_front();

  for (const auto& header : headers)
  {
    // Headers
    static const boost::regex headerregex("[\\s]*([\\w-]+)[\\s]*:[\\s]*(.*)[\\s]*");
    if (!boost::regex_search(header, match, headerregex))
    {

      continue;
    }

    std::string field = match[1];
    std::string value = match[2];
    if (boost::iequals(field, std::string("CSeq")))
    {
      try
      {
        cseq_ = std::stoul(value);
        
      }
      catch (...)
      {
      
      }
    }
    else if (field == std::string("Content-Base"))
    {
      contentbase_ = value;

    }
    else if (field == std::string("Content-Type"))
    {
      contenttype_ = headers::ContentTypeFromString(value);

    }
    else if (field == std::string("Content-Length"))
    {
      try
      {
        contentlength_ = std::stoul(value);

      }
      catch (...)
      {
      
      }
    }
    else if (field == std::string("Public"))
    {
      try
      {
        std::vector<std::string> options;
        options.reserve(8);
        static const boost::regex separator(",|, |,[^ ]|[^,] ");
        for (const std::string& option : boost::algorithm::split_regex(options, value, separator))
        {
          headers::REQUESTTYPE optiontype = headers::RequestTypeFromString(boost::trim_copy(option));
          if (optiontype != headers::REQUESTTYPE_INVALID)
          {
            options_.insert(optiontype);

          }
        }
      }
      catch (...)
      {
      
      }
    }
    else if (field == std::string("Date"))
    {
      std::istringstream is(value);
      is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%a, %d %b %Y %H:%M:%S")));
      is >> date_;
      if (is.fail()) // Try again with another date facet
      {
        std::istringstream is(value);
        is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%d %b %Y %H:%M:%S")));
        is >> date_;
        if (is.fail()) // Try again with another date facet
        {
          std::istringstream is(value);
          is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%a, %b %d %Y %H:%M:%S")));
          is >> date_;
        }
      }
    }
    else if (field == std::string("Session"))
    {
      static const boost::regex sessionregex("[\\s]*([\\w\\.\\-\\+\\$]+)[\\s]*(?:;[\\s]*timeout[\\s]*=[\\s]*([\\d]+))?");
      boost::smatch match;
      if (!boost::regex_search(value, match, sessionregex))
      {

        continue;
      }

      session_ = match[1];
      if (match[2].matched)
      {
        timeout_ = std::stoul(match[2]);

      }
    }
    else if (field == std::string("Transport"))
    {
      transport_ = headers::Transport(header);

    }
    else if (field == std::string("RTP-Info"))
    {
      rtpinfo_ = headers::RtpInfo(header);

    }
    else if (field == std::string("WWW-Authenticate"))
    {
      wwwauthenticate_.push_back(headers::WWWAuthenticate(header));

    }
  }
}

RtspResponse::RtspResponse(unsigned int code,
                           uint64_t cseq,
                           const boost::container::flat_set<headers::REQUESTTYPE>& options,
                           const boost::posix_time::ptime& date,
                           const std::string& realm,
                           const std::string& nonce,
                           bool stale,
                           const std::string& contentbase,
                           headers::CONTENTTYPE contenttype,
                           const boost::optional<unsigned long>& contentlength,
                           const std::string& session,
                           unsigned int timeout,
                           const headers::RtpInfo& rtpinfo) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  options_(options),
  date_(date),
  contentbase_(contentbase),
  contenttype_(contenttype),
  contentlength_(contentlength),
  session_(session),
  timeout_(timeout),
  rtpinfo_(rtpinfo)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(headers::CONTENTTYPE_INVALID),
  timeout_(DEFAULT_TIMEOUT)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const boost::container::flat_set<headers::REQUESTTYPE>& options) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  options_(options),
  contenttype_(headers::CONTENTTYPE_INVALID),
  timeout_(DEFAULT_TIMEOUT)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const headers::CONTENTTYPE contenttype, const sdp::Sdp& sdp) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(contenttype),
  timeout_(DEFAULT_TIMEOUT),
  sdp_(sdp)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const headers::Transport& transport, const std::string& session, const unsigned int timeout) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(headers::CONTENTTYPE_INVALID),
  session_(session),
  transport_(transport),
  timeout_(timeout)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const headers::CONTENTTYPE contenttype, const headers::Parameters& parameters) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(contenttype),
  timeout_(DEFAULT_TIMEOUT),
  parameters_(parameters)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const std::string& session, const headers::RtpInfo& rtpinfo) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(headers::CONTENTTYPE_INVALID),
  session_(session),
  timeout_(DEFAULT_TIMEOUT),
  rtpinfo_(rtpinfo)
{

}

RtspResponse::RtspResponse(unsigned int code, uint64_t cseq, const std::string& session) :
  code_(code),
  message_(Message(code)),
  cseq_(cseq),
  contenttype_(headers::CONTENTTYPE_INVALID),
  session_(session),
  timeout_(DEFAULT_TIMEOUT)
{

}

RtspResponse::~RtspResponse()
{

}

headers::WWWAuthenticate RtspResponse::GetStrongestAuthentication() const
{
  headers::WWWAuthenticate result(headers::AUTHENTICATIONTYPE_INVALID, std::string(), std::string(), boost::none);
  for (const auto& wwwauthenticate : wwwauthenticate_)
  {
    if (!wwwauthenticate.IsValid())
    {

      continue;
    }

    if (wwwauthenticate.authenticationtype_ > result.authenticationtype_)
    {
      result = wwwauthenticate;

    }
  }
  return result;
}

void RtspResponse::ParseContent(const std::string& content)
{
  switch (contenttype_)
  {
    case headers::CONTENTTYPE_APPLICATIONSDP:
    {
      sdp_ = sdp::Sdp(content);
      break;
    }
    case headers::CONTENTTYPE_TEXTPARAMETERS:
    {
      parameters_ = headers::Parameters(content);
      break;
    }
  }
}

bool RtspResponse::IsValid() const
{
  if (!code_.is_initialized() || message_.empty() || !cseq_.is_initialized())
  {
  
    return false;
  }

  for (const auto& wwwauthenticate : wwwauthenticate_)
  {
    if (!wwwauthenticate.IsValid())
    {

      return false;
    }
  }

  if ((contenttype_ == headers::CONTENTTYPE_APPLICATIONSDP) && !sdp_.is_initialized())
  {

    return false;
  }

  return true;
}

std::string RtspResponse::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  std::string result = std::string("RTSP/1.0 ") + std::to_string(*code_) + std::string(" ") + message_ + std::string("\r\n");
  result += std::string("CSeq: ") + std::to_string(*cseq_) + std::string("\r\n");

  if (!options_.empty())
  {
    std::vector<std::string> options;
    std::for_each(options_.begin(), options_.end(), [&options](headers::REQUESTTYPE requesttype){ options.push_back(rtsp::headers::ToString(requesttype)); });
    result += std::string("Public: ") + boost::algorithm::join(options, std::string(", ")) + std::string("\r\n");
  }

  if (!date_.is_not_a_date_time())
  {
    std::stringstream ss;
    static std::locale loc(ss.getloc(), new boost::posix_time::time_facet("Date: %a, %d %b %Y %H:%M:%S GMT\r\n"));
    ss.imbue(loc);
    ss << date_;
    result += ss.str();
  }

  for (const auto& wwwauthenticate : wwwauthenticate_)
  {
    result += wwwauthenticate.ToString();

  }

  if (!session_.empty())
  {
    result += std::string("Session: ") + session_ + std::string("; timeout=") + std::to_string(timeout_) + std::string("\r\n");

  }

  if (transport_.transports_.size())
  {
    result += transport_.ToString() + std::string("\r\n");

  }

  if (!rtpinfo_.rtpinfo_.empty())
  {
    result += rtpinfo_.ToString() + std::string("\r\n");

  }

  if (contentbase_.size())
  {
    result += std::string("Content-Base: ") + contentbase_ + std::string("\r\n");

  }

  std::string content;
  switch (contenttype_)
  {
    case headers::CONTENTTYPE_APPLICATIONSDP:
    {
      content = boost::algorithm::join(sdp_->ToString(), std::string("\r\n"));
      break;
    }
    case headers::CONTENTTYPE_TEXTPARAMETERS:
    {
      content = parameters_.ToString() + std::string("\r\n");
      break;
    }
  }

  if (content.length())
  {
    result += std::string("Content-Type: ") + rtsp::headers::ToString(contenttype_) + std::string("\r\n");
    result += std::string("Content-Length: ") + std::to_string(content.length()) + std::string("\r\n");
  }

  result += std::string("\r\n") + content;
  
  return result;
}

bool RtspResponse::operator==(const RtspResponse& rhs) const
{
  return ((code_ == rhs.code_) && (message_ == rhs.message_) && (cseq_ == rhs.cseq_) && (options_ == rhs.options_) && (date_ == rhs.date_) && std::is_permutation(wwwauthenticate_.begin(), wwwauthenticate_.end(), rhs.wwwauthenticate_.begin(), rhs.wwwauthenticate_.end()) && (contentbase_ == rhs.contentbase_) && (contenttype_ == rhs.contenttype_) && (contentlength_ == rhs.contentlength_) && (session_ == rhs.session_) && (timeout_ == rhs.timeout_) && (rtpinfo_ == rhs.rtpinfo_) && (sdp_ == rhs.sdp_) && (parameters_ == rhs.parameters_));
}

std::string RtspResponse::Message(const unsigned int code) const
{
  switch (code)
  {
    case 100:
    {
      
      return std::string("Continue");
    }
    case 200:
    {

      return std::string("OK");
    }
    case 201:
    {

      return std::string("Created");
    }
    case 250:
    {

      return std::string("Low on Storage Space");
    }
    case 300:
    {

      return std::string("Multiple Choices");
    }
    case 301:
    {

      return std::string("Moved Permanently");
    }
    case 302:
    {

      return std::string("Moved Temporarily");
    }
    case 303:
    {

      return std::string("See Other");
    }
    case 304:
    {

      return std::string("Not Modified");
    }
    case 305:
    {

      return std::string("Use Proxy");
    }
    case 400:
    {

      return std::string("Bad Request");
    }
    case 401:
    {

      return std::string("Unauthorized");
    }
    case 402:
    {

      return std::string("Payment Required");
    }
    case 403:
    {

      return std::string("Forbidden");
    }
    case 404:
    {

      return std::string("Not Found");
    }
    case 405:
    {

      return std::string("Method Not Allowed");
    }
    case 406:
    {

      return std::string("Not Acceptable");
    }
    case 407:
    {

      return std::string("Proxy Authentication Required");
    }
    case 408:
    {

      return std::string("Request Time-out");
    }
    case 410:
    {

      return std::string("Gone");
    }
    case 411:
    {

      return std::string("Length Required");
    }
    case 412:
    {

      return std::string("Precondition Failed");
    }
    case 413:
    {

      return std::string("Request Entity Too Large");
    }
    case 414:
    {

      return std::string("Request-URI Too Large");
    }
    case 415:
    {

      return std::string("Unsupported Media Type");
    }
    case 451:
    {

      return std::string("Parameter Not Understood");
    }
    case 452:
    {

      return std::string("Conference Not Found");
    }
    case 453:
    {

      return std::string("Not Enough Bandwidth");
    }
    case 454:
    {

      return std::string("Session Not Found");
    }
    case 455:
    {

      return std::string("Method Not Valid in This State");
    }
    case 456:
    {

      return std::string("Header Field Not Valid for Resource");
    }
    case 457:
    {

      return std::string("Invalid Range");
    }
    case 458:
    {

      return std::string("Parameter Is Read-Only");
    }
    case 459:
    {

      return std::string("Aggregate operation not allowed");
    }
    case 460:
    {

      return std::string("Only aggregate operation allowed");
    }
    case 461:
    {

      return std::string("Unsupported transport");
    }
    case 462:
    {

      return std::string("Destination unreachable");
    }
    case 463:
    {

      return std::string("Key management Failure");
    }
    case 500:
    {

      return std::string("Internal Server Error");
    }
    case 501:
    {

      return std::string("Not Implemented");
    }
    case 502:
    {

      return std::string("Bad Gateway");
    }
    case 503:
    {

      return std::string("Service Unavailable");
    }
    case 504:
    {

      return std::string("Gateway Time-out");
    }
    case 505:
    {

      return std::string("RTSP Version not supported");
    }
    case 551:
    {

      return std::string("Option not supported");
    }
    default:
    {
  
      return std::string("Error"); // Lets just assume it's an error message, because we honestly have no idea and it can't be good
    }
  }
}

}
