// testsession.cpp
//

///// Includes /////

#include "testsession.hpp"

#include <boost/asio.hpp>
#include <chrono>
#include <utility/utility.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

TestSession::TestSession(boost::asio::io_service& io, const uint32_t ssrc) :
  Session(io, ssrc),
  running_(false),
  rtppacket_(false, false, 0, false, 90, 0, 0, 1)
{

}

TestSession::~TestSession()
{
  Destroy();

}

unsigned int TestSession::Init(const std::string& name, const boost::shared_ptr<Connection>& connection, const headers::Transport& transport, boost::asio::io_service& io)
{

  return Session::Init(name, connection, transport);
}

void TestSession::Destroy()
{
  // If the thread is still active, join up and quit it
  if (play_.joinable())
  {
    running_ = false;
    play_.join();
  }

  Session::Destroy();
}

RtspResponse TestSession::GetParameter(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, headers::CONTENTTYPE_TEXTPARAMETERS, headers::Parameters({ headers::Parameter("key1", "value1") }));
}

RtspResponse TestSession::SetParameter(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, headers::CONTENTTYPE_TEXTPARAMETERS, headers::Parameters({ headers::Parameter("key1", std::string()) }));
}

RtspResponse TestSession::Play(const RtspRequest& request)
{
  if (play_.joinable())
  {
  
    return RtspResponse(OK, *request.cseq_, name_);
  }

  play_ = std::thread([&]()
  {
    running_ = true;
    while(running_)
    {
      const std::string data("test");
      rtppacket_.IncrementSequenceNumber();
      SendRtpFrame(reinterpret_cast<const char*>(&rtppacket_), sizeof(rtppacket_), reinterpret_cast<const uint8_t*>(data.data()), static_cast<uint16_t>(data.size()));
      utility::Sleep(std::chrono::milliseconds(50));
    }
  });

  return RtspResponse(OK, *request.cseq_, name_);
}

RtspResponse TestSession::Pause(const RtspRequest& request)
{
  if (play_.joinable())
  {
    running_ = false;
    play_.join();
  }

  return RtspResponse(200, *request.cseq_, name_);
}

}

}
