// rtsprequest.cpp
//

///// Includes /////

#include "rtsp/rtsprequest.hpp"

#include <assert.h>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <deque>
#include <utility/utility.hpp>

#include "rtsp/rtsp.hpp"

///// Namespaces /////

namespace rtsp
{

///// Functions /////

KEEPALIVEMODE FromInt(int keepalivemode)
{
  switch (keepalivemode)
  {
    case KEEPALIVEMODE_NONE:
    {

      return KEEPALIVEMODE_NONE;
    }
    case KEEPALIVEMODE_GETPARAMETER:
    {

      return KEEPALIVEMODE_GETPARAMETER;
    }
    case KEEPALIVEMODE_SETPARAMETER:
    {

      return KEEPALIVEMODE_SETPARAMETER;
    }
    default:
    {

      return KEEPALIVEMODE_NONE;
    }
  }
}

///// Methods /////

RtspRequest::RtspRequest() :
  requesttype_(headers::REQUESTTYPE_INVALID),
  accepttype_(headers::CONTENTTYPE_INVALID),
  requireonvifreplay_(false),
  contenttype_(headers::CONTENTTYPE_INVALID),
  keepalivemode_(KEEPALIVEMODE_NONE),
  authenticationtype_(headers::AUTHENTICATIONTYPE_INVALID)
{

}

RtspRequest::RtspRequest(const std::string& header) :
  requesttype_(headers::REQUESTTYPE_INVALID),
  accepttype_(headers::CONTENTTYPE_INVALID),
  requireonvifreplay_(false),
  contenttype_(headers::CONTENTTYPE_INVALID),
  keepalivemode_(KEEPALIVEMODE_NONE),
  authenticationtype_(headers::AUTHENTICATIONTYPE_INVALID)
{
  // Split the header by line
  std::deque<std::string> headers;
  static const boost::regex headerssplitregex("\r\n|\r[^\n]|[^\r]\n");
  boost::algorithm::split_regex(headers, header, headerssplitregex);
  if (headers.empty())
  {

    return;
  }

  // Parse the status line
  static const boost::regex statusregex("[\\s]*([\\w]+)[\\s]+(.*)[\\s]+RTSP\\/1.0[\\s]*");
  boost::smatch match;
  if (!boost::regex_search(headers.front(), match, statusregex))
  {

    return;
  }

  requesttype_ = headers::RequestTypeFromString(match[1]);
  url_ = match[2];
  headers.pop_front();
  
  // Read headers
  for (const auto& header : headers)
  {
    // Headers
    static const boost::regex headerregex("[\\s]*([\\w-]+)[\\s]*:[\\s]*(.*)[\\s]*");
    boost::smatch match;
    if (!boost::regex_search(header, match, headerregex))
    {

      continue;
    }

    const std::string field = match[1];
    const std::string value = match[2];

    if (field == std::string("CSeq"))
    {
      try
      {
        cseq_ = std::stoul(value);
        
      }
      catch (...)
      {
      
      }
    }
    else if (field == std::string("Accept"))
    {
      try
      {
        accepttype_ = headers::ContentTypeFromString(value);
        
      }
      catch (...)
      {
      
      }
    }
    else if (field == std::string("Transport"))
    {
      try
      {
        transport_ = headers::Transport(header);

      }
      catch (...)
      {

      }
    }
    else if (field == std::string("Session"))
    {
      try
      {
        session_ = value;

      }
      catch (...)
      {

      }
    }
    else if (field == std::string("Require"))
    {
      if (value == "onvif-replay")
      {
        requireonvifreplay_ = true;

      }
    }
    else if (field == std::string("Range"))
    {
      try
      {
        range_ = headers::Range(header);

      }
      catch (...)
      {

      }
    }
    else if (field == std::string("Rate-Control"))
    {
      if (boost::iequals(value, "yes"))
      {
        ratecontrol_ = true;

      }
      else
      {
        ratecontrol_ = false;

      }
    }
    else if (field == std::string("Immediate"))
    {
      if (boost::iequals(value, "yes"))
      {
        immediate_ = true;

      }
      else
      {
        immediate_ = false;

      }
    }
    else if (field == std::string("Content-Type"))
    {
      try
      {
        contenttype_ = headers::ContentTypeFromString(value);

      }
      catch (...)
      {

      }
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
    else if (field == std::string("Authorization"))
    {
      static const boost::regex authenticateregex("[\\s]*([\\w]+)[\\s]*(.*)");
      boost::smatch match;
      if (!boost::regex_search(value, match, authenticateregex))
      {

        continue;
      }

      authenticationtype_ = headers::AuthenticationTypeFromString(match[1]);
      if (authenticationtype_ == headers::AUTHENTICATIONTYPE_BASIC)
      {
        username_ = match[2].str();
        
      }
      else if (authenticationtype_ == headers::AUTHENTICATIONTYPE_DIGEST)
      {
        // Flags
        std::string flags = match[2].str();
        static const boost::regex flagsregex("[\\s]*[\\w]+[\\s]*=[\\s]*\\\"?[\\w:\\/\\.-\\?\\=]+\\\"?,?");
        for (boost::sregex_token_iterator i(flags.begin(), flags.end(), flagsregex); i != boost::sregex_token_iterator(); ++i)
        {
          std::string flag = i->str();
          boost::smatch match;
          static const boost::regex flagregex("[\\s]*([\\w]+)[\\s]*=[\\s]*\\\"?([\\w:\\/\\.-\\?\\=]+)\\\"?,?[\\s]*");
          if (!boost::regex_search(flag, match, flagregex))
          {

            continue;
          }

          const std::string key = match[1];
          const std::string value = match[2];

          if (boost::iequals(key, "realm"))
          {
            realm_ = value;

          }
          else if (boost::iequals(key, "nonce"))
          {
            nonce_ = value;

          }
          else if (boost::iequals(key, "uri"))
          {
            authenticationuri_ = value;

          }
          else if (boost::iequals(key, "username"))
          {
            username_ = value;

          }
          else if (boost::iequals(key, "response"))
          {
            response_ = value;

          }
        }
      }
    }
  }
}

RtspRequest::RtspRequest(headers::REQUESTTYPE requesttype,
                         const std::string& url,
                         uint64_t cseq,
                         headers::CONTENTTYPE accepttype,
                         const headers::Transport& transport,
                         const std::string& session,
                         const boost::optional<headers::Range>& range,
                         const boost::optional<bool>& ratecontrol,
                         const boost::optional<bool>& immediate,
                         headers::CONTENTTYPE contenttype,
                         const boost::optional<std::string::size_type>& contentlength,
                         const std::vector<std::string>& getparameters,
                         const headers::Parameters setparameters,
                         KEEPALIVEMODE keepalivemode,
                         const std::string& keepaliveurl) :
  requesttype_(requesttype),
  url_(url),
  cseq_(cseq),
  accepttype_(accepttype),
  requireonvifreplay_(false),
  transport_(transport),
  session_(session),
  range_(range),
  immediate_(immediate),
  ratecontrol_(ratecontrol),
  getparameters_(getparameters),
  setparameters_(setparameters),
  keepalivemode_(keepalivemode),
  keepaliveurl_(keepaliveurl),
  contenttype_(contenttype),
  contentlength_(contentlength),
  authenticationtype_(headers::AUTHENTICATIONTYPE_INVALID)
{
  
}

RtspRequest::RtspRequest(headers::REQUESTTYPE requesttype,
                         const std::string& url,
                         uint64_t cseq,
                         headers::CONTENTTYPE accepttype,
                         const headers::Transport& transport,
                         const std::string& session,
                         const boost::optional<headers::Range>& range,
                         const boost::optional<bool>& ratecontrol,
                         const boost::optional<bool>& immediate,
                         headers::CONTENTTYPE contenttype,
                         const boost::optional<std::string::size_type>& contentlength,
                         const std::vector<std::string>& getparameters,
                         const headers::Parameters setparameters,
                         KEEPALIVEMODE keepalivemode,
                         const std::string& keepaliveurl,
                         headers::AUTHENTICATIONTYPE authenticationtype,
                         const std::string& authenticationuri,
                         const std::string& username,
                         const std::string& realm,
                         const std::string& nonce) :
  requesttype_(requesttype),
  url_(url),
  cseq_(cseq),
  accepttype_(accepttype),
  requireonvifreplay_(false),
  transport_(transport),
  session_(session),
  range_(range),
  ratecontrol_(ratecontrol),
  immediate_(immediate),
  getparameters_(getparameters),
  setparameters_(setparameters),
  keepalivemode_(keepalivemode),
  keepaliveurl_(keepaliveurl),
  contenttype_(contenttype),
  contentlength_(contentlength),
  authenticationtype_(authenticationtype),
  authenticationuri_(authenticationuri),
  username_(username),
  realm_(realm),
  nonce_(nonce)
{

}

RtspRequest::~RtspRequest()
{

}

void RtspRequest::ParseContent(const std::string& content)
{
  switch (contenttype_)
  {
    case headers::CONTENTTYPE_TEXTPARAMETERS:
    {
      if (requesttype_ == headers::REQUESTTYPE_GETPARAMETER)
      {
        static const boost::regex newline("\r\n|\r[^\n]|[^\r]\n");
        boost::split_regex(getparameters_, content, newline);
      }
      else if (requesttype_ == headers::REQUESTTYPE_GETPARAMETER)
      {
        setparameters_ = headers::Parameters(content);

      }
      break;
    }
  }
}

bool RtspRequest::IsValid() const
{
  if (requesttype_ == headers::REQUESTTYPE_INVALID)
  {
  
    return false;
  }

  if (!cseq_.is_initialized())
  {
  
    return false;
  }

  return true;
}

std::string RtspRequest::ToString(const std::string& password) const
{
  const std::string rtsp("RTSP/1.0");

  if (requesttype_ == headers::REQUESTTYPE_INVALID || !cseq_.is_initialized())
  {
    
    return std::string();
  }

  std::string result;
  
  result += rtsp::headers::ToString(requesttype_) + std::string(" ") + url_ + std::string(" ") + rtsp + std::string("\r\n");
  result += std::string("CSeq: ") + std::to_string(*cseq_) + std::string("\r\n");

  if (accepttype_ != headers::CONTENTTYPE_INVALID)
  {
    result += std::string("Accept: ") + rtsp::headers::ToString(accepttype_) + std::string("\r\n");

  }
  
  if (!transport_.transports_.empty())
  {
    result += transport_.ToString() + std::string("\r\n");

  }

  if (!session_.empty())
  {
    result += std::string("Session: ") + session_ + std::string("\r\n");

  }

  if (requireonvifreplay_)
  {
    result += std::string("Require: onvif-replay\r\n");

  }

  if (range_.is_initialized())
  {
    result += range_->ToString() + std::string("\r\n");

  }

  if (ratecontrol_.is_initialized())
  {
    result += std::string("Rate-Control: ") + (*ratecontrol_ ? "yes" : "no") + "\r\n";

  }

  if (immediate_.is_initialized())
  {
    result += std::string("Immediate: ") + (*immediate_ ? "yes" : "no") + "\r\n";

  }

  switch (authenticationtype_)
  {
    case headers::AUTHENTICATIONTYPE_BASIC:
    {
      result += "Authorization: Basic " + utility::Base64Encode(username_ + ':' + password) + "\r\n";
      break;
    }
    case headers::AUTHENTICATIONTYPE_DIGEST:
    {
      // Build the authentication response string
      // ha1=MD5(username:realm:password)
      // ha2=MD5(method:digestURI)
      // response=MD5(ha1:nonce:ha2)
      response_ = utility::Md5(utility::Md5(username_ + ':' + realm_ + ':' + password) + ':' + nonce_ + ':' + utility::Md5(rtsp::headers::ToString(requesttype_) + ':' + authenticationuri_));
      result += std::string("Authorization: Digest username=\"") + username_ + std::string("\", realm=\"") + realm_ + std::string("\", nonce=\"") + nonce_ + std::string("\", uri=\"") + authenticationuri_ + std::string("\", response=\"") + response_ + std::string("\"\r\n");
      break;
    }
  }

  std::string content;
  if (contenttype_ == headers::CONTENTTYPE_TEXTPARAMETERS)
  {
    if (getparameters_.size())
    {
      content = boost::algorithm::join(getparameters_, std::string("\r\n"));

    }
    else if (setparameters_.parameters_.size())
    {
      content = setparameters_.ToString();

    }
  }

  if (content.length())
  {
    result += std::string("Content-Type: ") + rtsp::headers::ToString(contenttype_) + std::string("\r\n");
    result += std::string("Content-Length: ") + std::to_string(content.length()) + std::string("\r\n");
  }

  result += USERAGENT + std::string("\r\n\r\n") + content;

  return result;
}

bool RtspRequest::operator==(const RtspRequest& rhs) const
{
  return ((requesttype_ == rhs.requesttype_) && (url_ == rhs.url_)  && (cseq_ == rhs.cseq_) && (accepttype_ == rhs.accepttype_) && (transport_ == rhs.transport_) && (session_ == rhs.session_) && (requireonvifreplay_ == rhs.requireonvifreplay_) && (range_ == rhs.range_) && (ratecontrol_ == rhs.ratecontrol_) && (immediate_ == rhs.immediate_) && (getparameters_ == rhs.getparameters_) && (setparameters_ == rhs.setparameters_) && (contenttype_ == rhs.contenttype_) && (contentlength_ == rhs.contentlength_) && (authenticationtype_ == rhs.authenticationtype_) && (authenticationuri_ == rhs.authenticationuri_) && (username_ == rhs.username_) && (realm_ == rhs.realm_) && (nonce_ == rhs.nonce_));
}

}
