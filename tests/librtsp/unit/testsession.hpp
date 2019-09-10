// testsession.hpp
//

#ifndef IDXTS4V3VV9UTX4NU95PNJEK88612W3OK0
#define IDXTS4V3VV9UTX4NU95PNJEK88612W3OK0

///// Includes /////

#include <atomic>
#include <rtsp/server/session.hpp>
#include <thread>

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Connection;

///// Classes /////

class TestSession : public Session
{
 public:

  TestSession(boost::asio::io_service& io, const uint32_t ssrc);
  ~TestSession();

  unsigned int Init(const std::string& name, const boost::shared_ptr<Connection>& connection, const headers::Transport& transport, boost::asio::io_service& io);
  void Destroy();

  virtual RtspResponse GetParameter(const RtspRequest& request);
  virtual RtspResponse SetParameter(const RtspRequest& request);
  virtual RtspResponse Play(const RtspRequest& request);
  virtual RtspResponse Pause(const RtspRequest& request);

 private:

  std::thread play_;
  std::atomic<bool> running_;

  rtp::Packet rtppacket_;

};

}

}

#endif
