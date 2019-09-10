// streamtest.hpp
//

#ifndef IDORWYCJO2YL7I7WJ2UERP2YNQNSA51DJA
#define IDORWYCJO2YL7I7WJ2UERP2YNQNSA51DJA

///// Includes /////

#include <rtsp/server/stream.hpp>
#include <rtsp/server/server.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Classes /////

class StreamTest : public Stream
{
 public:

  StreamTest(boost::shared_ptr<Server>& server, const std::string& url, const std::set<headers::REQUESTTYPE>& authenticationrequired, const bool basicauthentication, const bool digestauthentication, const std::string& realm, const std::string& username, const std::string& password, const std::string& fakesession);
  ~StreamTest();

  boost::shared_ptr<Session> CreateSession(const uint32_t ssrc, const std::string& uri) const override;

  virtual bool AuthenticateBasic(const std::string& username, const std::string& password) const override;
  virtual bool AuthenticateDigest(const std::string& username, const headers::REQUESTTYPE requesttype, const std::string& nonce, const std::string& uri, const std::string& response) const override;

  RtspResponse Options(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse Describe(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse Setup(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse GetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse SetParameter(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse Play(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse Pause(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;
  RtspResponse Teardown(const boost::shared_ptr<Connection>& connection, const RtspRequest& request) override;

 private:

  const std::string realm_;
  const std::string username_;
  const std::string password_;
  const std::string fakesession_;

};

}

}

}

#endif
