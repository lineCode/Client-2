// stream.cpp
//

///// Includes /////

#include "rtsp/server/stream.hpp"

#include <assert.h>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

Stream::Stream(const boost::shared_ptr<Server>& server, const std::string& path, const std::set<headers::REQUESTTYPE>& authenticationrequired, const bool basicauthentication, const bool digestauthentication) :
  server_(server),
  path_(path),
  ssrcdistribution_(1, std::numeric_limits<uint32_t>::max()),
  authenticationrequired_(authenticationrequired),
  basicauthentication_(basicauthentication),
  digestauthentication_(digestauthentication)
{
  
}

Stream::~Stream()
{
  Destroy();

}

void Stream::Destroy()
{
  authenticationrequired_.clear();
  {
    std::lock_guard<std::mutex> lock(sessionsmutex_);
    std::for_each(sessions_.begin(), sessions_.end(), [](boost::shared_ptr<Session>& session) { return session->Destroy(); });
    sessions_.clear();
  }
}

RtspResponse Stream::Setup(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  const std::string sessionname = GenerateSessionName();
  if (sessionname.empty())
  {

    return RtspResponse(INTERNAL_SERVER_ERROR, *request.cseq_);
  }

  const uint32_t ssrc = GenerateSsrc();
  if (ssrc == 0)
  {

    return RtspResponse(INTERNAL_SERVER_ERROR, *request.cseq_);
  }
  
  boost::shared_ptr<Session> session = CreateSession(ssrc, request.url_);
  if (!session)
  {

    return RtspResponse(NOT_FOUND, *request.cseq_);
  }

  const unsigned int response = session->Init(sessionname, connection, request.transport_);
  if (response != OK)
  {

    return RtspResponse(response, *request.cseq_);
  }

  {
    std::lock_guard<std::mutex> lock(sessionsmutex_);
    sessions_.push_back(session);
  }

  session->GetTimeout()->expires_from_now(boost::posix_time::seconds(static_cast<long>(SESSION_TIMEOUT * 1.1)));
  session->GetTimeout()->async_wait(boost::bind(&Stream::SessionTimeout, shared_from_this(), boost::asio::placeholders::error, session));

  return RtspResponse(OK, *request.cseq_, session->GetTransport(), session->GetName(), SESSION_TIMEOUT);
}

RtspResponse Stream::GetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  if (request.session_.empty())
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }
  
  const boost::shared_ptr<Session> session = GetSession(request.session_);
  if (!session)
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }

  session->GetTimeout()->cancel();
  session->GetTimeout()->expires_from_now(boost::posix_time::seconds(static_cast<long>(SESSION_TIMEOUT * 1.1)));
  session->GetTimeout()->async_wait(boost::bind(&Stream::SessionTimeout, shared_from_this(), boost::asio::placeholders::error, session));

  return session->GetParameter(request);
}

RtspResponse Stream::SetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  if (request.session_.empty())
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }
  
  const boost::shared_ptr<Session> session = GetSession(request.session_);
  if (!session)
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }

  session->GetTimeout()->cancel();
  session->GetTimeout()->expires_from_now(boost::posix_time::seconds(static_cast<long>(SESSION_TIMEOUT * 1.1)));
  session->GetTimeout()->async_wait(boost::bind(&Stream::SessionTimeout, shared_from_this(), boost::asio::placeholders::error, session));

  return session->SetParameter(request);
}

RtspResponse Stream::Play(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  boost::shared_ptr<Session> session = GetSession(request.session_);
  if (!session)
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }
  return session->Play(request);
}

RtspResponse Stream::Pause(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  if (request.session_.empty())
  {

    return RtspResponse(454, *request.cseq_);
  }
  
  const boost::shared_ptr<Session> session = GetSession(request.session_);
  if (!session)
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }

  return session->Pause(request);
}

RtspResponse Stream::Teardown(const boost::shared_ptr<Connection>& connection, const RtspRequest& request)
{
  if (request.session_.empty())
  {

    return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
  }
  
  {
    std::lock_guard<std::mutex> lock(sessionsmutex_);
    std::vector< boost::shared_ptr<Session> >::iterator session = std::find_if(sessions_.begin(), sessions_.end(), [&request](const boost::shared_ptr<Session>& session){ return (session->GetName() == request.session_); });
    if (session == sessions_.end())
    {
  
      return RtspResponse(SESSION_NOT_FOUND, *request.cseq_);
    }
    (*session)->Destroy();
    sessions_.erase(session);
  }
  return RtspResponse(OK, *request.cseq_);
}

bool Stream::Authenticate(const std::string& realm, const std::string& nonce, const RtspRequest& request) const
{
  if (!basicauthentication_ && !digestauthentication_) // If authentication is disabled, never fail
  {
  
    return true;
  }

  if (!GetAthenticationRequired(request.requesttype_)) // Does this request type require authentication
  {
    
    return true;
  }

  if (basicauthentication_ && (request.authenticationtype_ == headers::AUTHENTICATIONTYPE_BASIC))
  {
    if (request.username_.empty())
    {
    
      return false;
    }

    const std::vector<unsigned char> data = utility::Base64Decode(request.username_);
    if (data.empty())
    {
    
      return false;
    }
    const std::string auth(reinterpret_cast<const char*>(data.data()), data.size());
    const std::string::size_type colon = auth.find(":");
    if (colon == std::string::npos)
    {
      
      return false;
    }
    const std::string username = auth.substr(0, colon);
    const std::string password = auth.substr(colon + 1, std::string::npos);
    return AuthenticateBasic(username, password);
  }
  else if (digestauthentication_ && (request.authenticationtype_ == headers::AUTHENTICATIONTYPE_DIGEST))
  {
    if (request.username_.empty())
    {
    
      return false;
    }

    return AuthenticateDigest(request.username_, request.requesttype_, nonce, request.authenticationuri_, request.response_);
  }

  return false;
}

void Stream::ResetInterleavedConnections(const boost::shared_ptr<Connection>& interleavedconnection)
{
  std::lock_guard<std::mutex> lock(sessionsmutex_);
  for (auto& session : sessions_)
  {
    if (session->GetInterleavedConnection() == interleavedconnection)
    {
      session->ResetInterleavedConnection();

    }
  }
}

void Stream::SessionTimeout(const boost::system::error_code& err, boost::weak_ptr<Session> session)
{
  boost::shared_ptr<Session> s = session.lock();
  if (!s) // If the session is already dead, just ignore this
  {
    
    return;
  }

  if (err && (s->GetTimeout())) // If the timer got reset and we still want to live
  {
    s->GetTimeout()->async_wait(boost::bind(&Stream::SessionTimeout, shared_from_this(), boost::asio::placeholders::error, session));
    return;
  }
  
  {
    std::lock_guard<std::mutex> lock(sessionsmutex_);
    const std::vector< boost::shared_ptr<Session> >::const_iterator i = std::find(sessions_.begin(), sessions_.end(), s);
    if (i == sessions_.end())
    {
  
      return;
    }
    sessions_.erase(i);
  }
}

uint32_t Stream::GenerateSsrc()
{
  std::lock_guard<std::mutex> lock(sessionsmutex_);
  const uint32_t ssrc = ssrcdistribution_(ssrcgenerator_);
  if (std::find_if(sessions_.begin(), sessions_.end(), [ssrc](const boost::shared_ptr<Session>& session){ return (session->GetSsrc() == ssrc); }) == sessions_.end())
  {

    return ssrc;
  }
  return 0;
}

std::string Stream::GenerateSessionName() const
{
  std::lock_guard<std::mutex> lock(sessionsmutex_);
  const std::string name = utility::GenerateRandomString(10);
  for (int i = 0; i < 50; ++i)
  {
    if (std::find_if(sessions_.begin(), sessions_.end(), [name](const boost::shared_ptr<Session>& session){ return (session->GetName() == name); }) == sessions_.end())
    {
  
      return name;
    }
  }
  return std::string(); // Something is very wrong if we arrive here, but lets return an empty string for good measure anyway
}

boost::shared_ptr<Session> Stream::GetSession(const std::string& name)
{
  std::lock_guard<std::mutex> lock(sessionsmutex_);
  std::vector< boost::shared_ptr<Session> >::iterator session = std::find_if(sessions_.begin(), sessions_.end(), [name](const boost::shared_ptr<Session>& session){ return (session->GetName() == name); });
  if (session == sessions_.end())
  {
  
    return nullptr;
  }
  return *session;
}

void Stream::SendRtpFrame(const rtp::Packet& rtpheader, const uint8_t* data, const uint16_t size)
{
  std::lock_guard<std::mutex> lock(sessionsmutex_);
  for (auto& session : sessions_)
  {
    session->SendRtpFrame(reinterpret_cast<const char*>(&rtpheader), sizeof(rtp::Packet), data, size);
  
  }
}

}

}
