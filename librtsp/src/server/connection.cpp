// connection.cpp
//

///// Includes /////

#include "rtsp/server/connection.hpp"

#include <boost/bind.hpp>
#include <network/uri.hpp>
#include <utility/utility.hpp>

#include "rtsp/server/httpconnection.hpp"
#include "rtsp/server/server.hpp"
#include "rtsp/server/userconnection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Globals /////

const std::string XSESSIONCOOKIE("x-sessioncookie");

///// Methods /////

Connection::Connection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection) :
  delimeter_("(?:\\$|\r\n\r\n|\r\r|\n\n)"),
  nonce_(utility::GenerateRandomString(32)),
  server_(std::move(server)),
  userconnection_(std::move(userconnection)),
  socket_(io)
{
  
}

Connection::~Connection()
{
  boost::system::error_code err;
  socket_.close(err);
  userconnection_.reset();
}

void Connection::Destroy()
{
  boost::system::error_code err;
  socket_.close(err);
}

void Connection::Connected()
{
  if (userconnection_)
  {
    userconnection_->Connected();

  }

  boost::asio::async_read_until(socket_, buffer_, delimeter_, boost::bind(&Connection::HandleRTSPRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 1>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 2>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 3>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 4>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 5>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 6>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 7>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::array<boost::asio::const_buffer, 8>& buffers)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, boost::asio::buffer(data, size), boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::Write(const std::string& data)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, boost::asio::buffer(data), boost::asio::transfer_all(), err);
  return err;
}

std::string Connection::GetLocalAddress() const
{
  boost::system::error_code err;
  const auto endpoint = socket_.local_endpoint(err);
  if (err)
  {

    return std::string();
  }
  return endpoint.address().to_string();
}

uint16_t Connection::GetLocalPort() const
{
  boost::system::error_code err;
  const auto endpoint = socket_.local_endpoint(err);
  if (err)
  {

    return 0;
  }
  return endpoint.port();
}

void Connection::HandleRTSPRead(const boost::system::error_code& error, std::size_t size)
{
  if (error)
  {
    Destroy();
    return;
  }

  if ((buffer_.in_avail() >= 1) && (std::string(boost::asio::buffer_cast<const char*>(buffer_.data() + (size - 1)), 1) == "$")) // Interleaved RTP('$')
  {
    buffer_.consume(size);
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(3), boost::bind(&Connection::HandleRTSPRtpHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else // Newlines
  {
    RtspRequest request(std::string(boost::asio::buffer_cast<const char*>(buffer_.data()), size));
    buffer_.consume(size);
    if (!request.IsValid())
    {
      Destroy();
      return;
    }

    // Read any content if Content-Length header says to
    if (request.contentlength_.is_initialized() && (*request.contentlength_ > 0))
    {
      // Pull any extra bytes from the stream if we need too, otherwise just parse the response now
      const int readlength = static_cast<int>(*request.contentlength_) - static_cast<int>(buffer_.in_avail());
      if (readlength > 0)
      {
        boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(readlength), boost::bind(&Connection::HandleRTSPContent, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request));

      }
      else
      {
        request.ParseContent(std::string(boost::asio::buffer_cast<const char*>(buffer_.data()), *request.contentlength_));
        buffer_.consume(*request.contentlength_);

        ParseRequest(request);
        boost::asio::async_read_until(socket_, buffer_, delimeter_, boost::bind(&Connection::HandleRTSPRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
      }
    }
    else
    {
      ParseRequest(request);
      boost::asio::async_read_until(socket_, buffer_, delimeter_, boost::bind(&Connection::HandleRTSPRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
  }
}

void Connection::HandleRTSPRtpHeader(const boost::system::error_code& err, std::size_t)
{
  if (err)
  {
    Destroy();
    return;
  }

  uint8_t channel = 0;
  buffer_.sgetn(reinterpret_cast<char*>(&channel), 1);
  uint16_t numbytes = 0;
  buffer_.sgetn(reinterpret_cast<char*>(&numbytes), 2);
  numbytes = ntohs(numbytes);

  if (numbytes < (sizeof(rtp::Packet) + 1)) // If there isn't enough room for an rtp packet and at least 1 byte of data, clearly the client has sent some guff and we can just terminate now
  {
    Destroy();
    return;
  }

  boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(numbytes), boost::bind(&Connection::HandleRTSPRtpData, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, channel, numbytes));
}

void Connection::HandleRTSPRtpData(const boost::system::error_code& err, std::size_t size, uint8_t channel, uint16_t rtpdatasize)
{
  if (err)
  {
    Destroy();
    return;
  }

  auto interleavedchannel = GetInterleavedChannel(channel);
  if (interleavedchannel.session_ && (interleavedchannel.type_ == INTERLEAVEDCHANNEL_RTP))
  {
    if (size < sizeof(rtp::Packet))
    {
      // Ignore it
      return;
    }

    const rtp::Packet* rtppacket = reinterpret_cast<const rtp::Packet*>(buffer_.data().begin());
    const unsigned int rtppacketsize = rtppacket->GetSize();
    if (size < rtppacketsize)
    {
      //Ignore it
      return;
    }

    interleavedchannel.session_->RtpPacket(rtppacket, reinterpret_cast<const char*>(buffer_.data().begin()) + rtppacketsize, rtpdatasize - rtppacketsize);
  }
  else if (interleavedchannel.session_ && (interleavedchannel.type_ == INTERLEAVEDCHANNEL_RTCP))
  {
    rtcp::Packets rtcppackets;
    unsigned int offset = 0;
    while (offset < rtpdatasize)
    {
      rtcp::Packet rtcppacket;
      if (rtcppacket.Init(reinterpret_cast<const char*>(buffer_.data().begin()) + offset, rtpdatasize - offset))
      {

        break;
      }
      rtcppackets.rtcppackets_.push_back(rtcppacket);
      offset += (rtcppacket.length_ + 1) * 4;
    }
    interleavedchannel.session_->RtcpPacket(rtcppackets);
  }

  buffer_.consume(rtpdatasize);
  boost::asio::async_read_until(socket_, buffer_, delimeter_, boost::bind(&Connection::HandleRTSPRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::HandleRTSPContent(const boost::system::error_code& err, std::size_t, RtspRequest request)
{
  if (err)
  {
    Destroy();
    return;
  }

  request.ParseContent(std::string(boost::asio::buffer_cast<const char*>(buffer_.data()), *request.contentlength_));
  buffer_.consume(*request.contentlength_);

  ParseRequest(request);
  boost::asio::async_read_until(socket_, buffer_, delimeter_, boost::bind(&Connection::HandleRTSPRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::ParseRequest(const RtspRequest& request)
{
  if (!request.cseq_.is_initialized()) // Ignore this request if it doesn't contain a cseq, there isn't much reason to shut it down
  {

    return;
  }

  RtspResponse response; // Should always be filled in so we always have a response
  {
    try
    {
      const network::uri uri(request.url_);
      const auto pathview = uri.path();
      const std::string path = std::string(pathview.begin(), pathview.end());

      boost::shared_ptr<Stream> stream = server_->GetStream(path);
      if (stream == nullptr)
      {
        response = RtspResponse(NOT_FOUND, *request.cseq_);

      }
      else
      {
        if (stream->Authenticate(server_->GetRealm(), nonce_, request)) // Does the request pass authentication
        {
          switch (request.requesttype_)
          {
            case headers::REQUESTTYPE_OPTIONS:
            {
              response = stream->Options(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_DESCRIBE:
            {
              response = stream->Describe(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_SETUP:
            {
              response = stream->Setup(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_GETPARAMETER:
            {
              response = stream->GetParameter(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_SETPARAMETER:
            {
              response = stream->SetParameter(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_PLAY:
            {
              response = stream->Play(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_PAUSE:
            {
              response = stream->Pause(shared_from_this(), request);
              break;
            }
            case headers::REQUESTTYPE_TEARDOWN:
            {
              response = stream->Teardown(shared_from_this(), request);
              break;
            }
            default:
            {
              Destroy(); // This shouldn't happen anyway, but if it does, lets assume the connection is being naughty
              break;
            }
          }
        }
        else // Authentication successful or not required
        {
          response = RtspResponse(UNAUTHORIZED, *request.cseq_);
          if (stream->GetBasicAuthentication())
          {
            response.wwwauthenticate_.push_back(headers::WWWAuthenticate(headers::AUTHENTICATIONTYPE_BASIC, server_->GetRealm()));

          }

          if (stream->GetDigestAuthentication())
          {
            response.wwwauthenticate_.push_back(headers::WWWAuthenticate(headers::AUTHENTICATIONTYPE_DIGEST, server_->GetRealm(), nonce_, false));

          }
        }
      }
    }
    catch (...)
    {
      response = RtspResponse(NOT_FOUND, *request.cseq_);

    }
  }

  const std::string data = response.ToString();
  if (Write(data.data(), data.size()))
  {
    Destroy();

  }
}

void Connection::DestroyConnection()
{
  if (!server_)
  {

    return;
  }
  server_->DestroyConnection(shared_from_this());
  server_ = nullptr;
}

std::string Connection::BadHTTPRequest() const
{
  return std::string("HTTP/1.1 400 Bad Request\r\n\r\n");
}

}

}
