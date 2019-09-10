// stream.hpp
//

#ifndef IDGV4ADKADST2SKAJSDADSAJ2L1DDMSF3K
#define IDGV4ADKADST2SKAJSDADSAJ2L1DDMSF3K

///// Includes /////

#include <boost/asio.hpp>

#include <mutex>
#include <string>
#include <set>
#include <vector>

#include "connection.hpp"
#include "rtsp/rtsprequest.hpp"
#include "rtsp/rtspresponse.hpp"
#include "server.hpp"
#include "session.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Globals /////

const unsigned int SESSION_TIMEOUT = 60;

///// Declarations /////

class Server;

///// Classes /////

class Stream : public boost::enable_shared_from_this<Stream>
{
public:

  Stream(const boost::shared_ptr<Server>& server, const std::string& path, const std::set<headers::REQUESTTYPE>& authenticationrequired, const bool basicauthentication, const bool digestauthentication);
  virtual ~Stream();

  void Destroy();

  virtual boost::shared_ptr<Session> CreateSession(const uint32_t ssrc, const std::string& uri) const = 0;

  virtual bool AuthenticateBasic(const std::string& username, const std::string& password) const = 0;
  virtual bool AuthenticateDigest(const std::string& username, const headers::REQUESTTYPE requesttype, const std::string& nonce, const std::string& uri, const std::string& response) const = 0;

  virtual RtspResponse Options(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) = 0;
  virtual RtspResponse Describe(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) = 0;
  virtual RtspResponse Setup(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);
  virtual RtspResponse GetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);
  virtual RtspResponse SetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);
  virtual RtspResponse Play(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);
  virtual RtspResponse Pause(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);
  virtual RtspResponse Teardown(const boost::shared_ptr<Connection>& connection, const RtspRequest& request);

  inline const std::string& GetPath() const { return path_; }

  virtual bool Authenticate(const std::string& realm, const std::string& nonce, const RtspRequest& request) const;
  bool GetAthenticationRequired(const headers::REQUESTTYPE requesttype) const { return (authenticationrequired_.find(requesttype) != authenticationrequired_.end()); }

  void SetBasicAuthentication(const bool basicauthentication) { basicauthentication_ = basicauthentication; }
  bool GetBasicAuthentication() const { return basicauthentication_; }

  void SetDigestAuthentication(const bool digestauthentication) { digestauthentication_ = digestauthentication; }
  bool GetDigestAuthentication() const { return digestauthentication_; }

  void ResetInterleavedConnections(const boost::shared_ptr<Connection>& interleavedconnection);

 protected:

  void SessionTimeout(const boost::system::error_code& err, boost::weak_ptr<Session> session);
  std::string GenerateSessionName() const; // Generates a unique session name
  uint32_t GenerateSsrc(); // Generates a unique ssrc
  boost::shared_ptr<Session> GetSession(const std::string& name);
  void SendRtpFrame(const rtp::Packet& rtpheader, const uint8_t* data, const uint16_t size); // This is useful when all sessions will use the same live footage. This synchronously blocks so shouldn't be used with large buffers

  boost::shared_ptr<Server> server_;
  const std::string path_;
  
  std::default_random_engine ssrcgenerator_;
  std::uniform_int_distribution<uint32_t> ssrcdistribution_;

  std::set<headers::REQUESTTYPE> authenticationrequired_;
  bool basicauthentication_;
  bool digestauthentication_;

  mutable std::mutex sessionsmutex_;
  std::vector< boost::shared_ptr<Session> > sessions_;
  
};

}

}

#endif
