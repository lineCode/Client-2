// session.cpp
//

///// Includes /////

#include "rtsp/server/session.hpp"

#include <array>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <numeric>

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

Session::Session(boost::asio::io_service& io, const uint32_t ssrc) :
  ssrc_(ssrc),
  timeout_(boost::make_shared<boost::asio::deadline_timer>(io))
{

}

Session::Session(Session&& session) :
  ssrc_(session.ssrc_),
  name_(session.name_),
  timeout_(std::move(session.timeout_)),
  interleavedconnection_(session.interleavedconnection_),
  interleavedrtpchannel_(session.interleavedrtpchannel_),
  interleavedrtcpchannel_(session.interleavedrtcpchannel_)
{

}

Session::~Session()
{
  Destroy();

}

unsigned int Session::Init(const std::string& name, const boost::shared_ptr<Connection>& interleavedconnection, const headers::Transport& transport)
{
  name_ = name;
  interleavedconnection_ = interleavedconnection;

  for (const headers::TransportData& transportdata : transport.transports_)
  {
    if (transportdata.GetMode() != headers::MODE_PLAY)
    {

      return INTERNAL_SERVER_ERROR;
    }

    if (transportdata.GetProtocolType() == headers::PROTOCOLTYPE_UDP)
    {
      if (transportdata.GetRoutingType() == headers::ROUTINGTYPE_UNICAST)
      {
        if (!transportdata.clientport_.first.is_initialized())
        {
        
          return INTERNAL_SERVER_ERROR;
        }

        if (transportdata.clientport_.second.is_initialized()) // We need to open two adjacent ports, which is really inconvenient
        {
          // Attempt to get neighbouring ports
          std::vector<boost::asio::ip::udp::socket> tmpsockets; // We can't just rotate normally when searching for a port, we have to collect invalid ports to avoid just getting the same one back every time
          boost::optional<boost::asio::ip::udp::socket> socket1(timeout_->get_io_service());
          boost::optional<boost::asio::ip::udp::socket> socket2(timeout_->get_io_service());

          boost::system::error_code err;
          if (socket1->open(boost::asio::ip::udp::v4(), err))
          {

            return INTERNAL_SERVER_ERROR;
          }

          for (int i = 0; i < 50; ++i)
          {
            boost::asio::ip::udp::socket tmpsocket(timeout_->get_io_service());
            if (tmpsocket.open(boost::asio::ip::udp::v4(), err))
            {

              return INTERNAL_SERVER_ERROR;
            }

            if (tmpsocket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0), err))
            {

              return INTERNAL_SERVER_ERROR;
            }

            if (tmpsocket.local_endpoint().port() != std::numeric_limits<unsigned short>::max())
            {
              if (tmpsocket.local_endpoint().port() & 0b1) // Is odd
              {
                // Attempt to bind a port one lower
                if (!socket1->bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), tmpsocket.local_endpoint().port() - 1), err))
                {
                  socket2 = boost::asio::ip::udp::socket(std::move(tmpsocket));
                  break; // Success
                }
                std::cout << err.message() << std::endl;
              }
              else
              {
                // Attempt to bind a bind one higher
                if (!socket1->bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), tmpsocket.local_endpoint().port() + 1), err))
                {
                  socket2 = boost::asio::ip::udp::socket(std::move(tmpsocket));
                  break; // Success
                }
              }
            }

            tmpsockets.emplace_back(std::move(tmpsocket));
          }

          if (!socket2->is_open())
          {

            return INTERNAL_SERVER_ERROR;
          }

          if (socket1->local_endpoint().port() & 0b1) // Is odd
          {
            boost::shared_ptr<UdpUnicastTransport> udpunicasttransport = boost::make_shared<UdpUnicastTransport>(*socket2, socket1, interleavedconnection_->GetSocket().remote_endpoint(err).address(), *transportdata.clientport_.first, transportdata.clientport_.second);
            if (err)
            {

              return INTERNAL_SERVER_ERROR;
            }

            udpunicastsockets_.push_back(udpunicasttransport);
          }
          else // Is even
          {
            boost::shared_ptr<UdpUnicastTransport> udpunicasttransport = boost::make_shared<UdpUnicastTransport>(*socket1, socket2, interleavedconnection_->GetSocket().remote_endpoint(err).address(), *transportdata.clientport_.first, transportdata.clientport_.second);
            if (err)
            {

              return INTERNAL_SERVER_ERROR;
            }

            udpunicastsockets_.push_back(udpunicasttransport);
          }

          udpunicastsockets_.back()->rtpsocket_.async_receive_from(boost::asio::buffer(udpunicastsockets_.back()->rtpbuffer_), udpunicastsockets_.back()->rtpendpoint_, boost::bind(&Session::HandleRtp, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, udpunicastsockets_.back()));
          udpunicastsockets_.back()->rtcpsocket_->async_receive_from(boost::asio::buffer(*udpunicastsockets_.back()->rtcpbuffer_), udpunicastsockets_.back()->rtcpendpoint_, boost::bind(&Session::HandleRtcp, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, udpunicastsockets_.back()));
        }
        else // Just open the rtp port
        {
          boost::system::error_code err;
          boost::asio::ip::udp::socket rtpsocket(timeout_->get_io_service());
          if (rtpsocket.open(boost::asio::ip::udp::v4(), err))
          {

            return INTERNAL_SERVER_ERROR;
          }

          if (rtpsocket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0), err))
          {

            return INTERNAL_SERVER_ERROR;
          }

          boost::optional<boost::asio::ip::udp::socket> rtcp;
          boost::shared_ptr<UdpUnicastTransport> udpunicasttransport = boost::make_shared<UdpUnicastTransport>(rtpsocket, rtcp, interleavedconnection_->GetSocket().remote_endpoint(err).address(), *transportdata.clientport_.first, boost::none);
          if (err)
          {

            return INTERNAL_SERVER_ERROR;
          }

          udpunicastsockets_.push_back(udpunicasttransport);
          udpunicasttransport->rtpsocket_.async_receive_from(boost::asio::buffer(udpunicastsockets_.back()->rtpbuffer_), udpunicastsockets_.back()->rtpendpoint_, boost::bind(&Session::HandleRtp, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, udpunicasttransport));
        }
      }
      else if (transportdata.GetRoutingType() == headers::ROUTINGTYPE_MULTICAST)
      {

        return UNSUPPORTED_TRANSPORT;
      }
      else
      {

        return INTERNAL_SERVER_ERROR;
      }
    }
    else if (transportdata.GetProtocolType() == headers::PROTOCOLTYPE_TCP)
    {
      if (transportdata.GetRoutingType() == headers::ROUTINGTYPE_UNICAST)
      {
        if (transportdata.interleaved_.rtp_.is_initialized() && transportdata.interleaved_.rtcp_.is_initialized())
        {
          if (interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtp_).session_ || interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtcp_).session_)
          {

            return INTERNAL_SERVER_ERROR;
          }
          else
          {
            interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtp_).session_ = shared_from_this();
            interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtp_).type_ = INTERLEAVEDCHANNEL_RTP;
            interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtcp_).session_ = shared_from_this();
            interleavedconnection_->GetInterleavedChannel(*transportdata.interleaved_.rtcp_).type_ = INTERLEAVEDCHANNEL_RTCP;
            interleavedrtpchannel_ = *transportdata.interleaved_.rtp_;
            interleavedrtcpchannel_ = *transportdata.interleaved_.rtcp_;
          }
        }
      }
      else
      {

        return INTERNAL_SERVER_ERROR;
      }
    }
    else
    {

      return INTERNAL_SERVER_ERROR;
    }
  }

  return OK;
}

void Session::Destroy()
{
  boost::system::error_code err;
  timeout_->cancel(err);

  // These can be erased as we loop through destroying them, so we have to be much more careful...
  while (udpunicastsockets_.size())
  {
    boost::shared_ptr<UdpUnicastTransport> udpunicastsocket = udpunicastsockets_.back();
    boost::system::error_code err;
    udpunicastsocket->rtpsocket_.close(err);
    if (udpunicastsocket->rtcpsocket_.is_initialized())
    {
      udpunicastsocket->rtcpsocket_->close(err);

    }

    auto i = std::find(udpunicastsockets_.begin(), udpunicastsockets_.end(), udpunicastsocket);
    if (i != udpunicastsockets_.end())
    {
      udpunicastsockets_.erase(i);

    }
  }
  
  if (interleavedconnection_)
  {
    if (interleavedrtpchannel_.is_initialized())
    {
      interleavedconnection_->GetInterleavedChannel(*interleavedrtpchannel_).Reset();
      interleavedrtpchannel_.reset();
    }

    if (interleavedrtcpchannel_.is_initialized())
    {
      interleavedconnection_->GetInterleavedChannel(*interleavedrtcpchannel_).Reset();
      interleavedrtcpchannel_.reset();
    }
  }
}

RtspResponse Session::GetParameter(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, headers::CONTENTTYPE_TEXTPARAMETERS, headers::Parameters());
}

RtspResponse Session::SetParameter(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, headers::CONTENTTYPE_TEXTPARAMETERS, headers::Parameters());
}

RtspResponse Session::Play(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, name_);
}

RtspResponse Session::Pause(const RtspRequest& request)
{
  return RtspResponse(OK, *request.cseq_, name_);
}

void Session::RtpPacket(const rtp::Packet* rtppacket, const char* data, const size_t size)
{
  (void)rtppacket; // We don't do anything by default

}

void Session::RtcpPacket(const rtcp::Packets& rtcppackets)
{
  (void)rtcppackets; // We don't do anything by default

}

void Session::SendRtpFrame(const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 1> buffers =
  {
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

void Session::SendRtpFrame(const char* headerdata, const uint16_t headersize, const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(headerdata, headersize),
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

void Session::SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 3> buffers =
  {
    boost::asio::const_buffer(header1data, header1size),
    boost::asio::const_buffer(header2data, header2size),
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

void Session::SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 5> buffers =
  {
    boost::asio::const_buffer(header1data, header1size),
    boost::asio::const_buffer(header2data, header2size),
    boost::asio::const_buffer(header3data, header3size),
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

void Session::SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const char* header4data, const uint16_t header4size, const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 6> buffers =
  {
    boost::asio::const_buffer(header1data, header1size),
    boost::asio::const_buffer(header2data, header2size),
    boost::asio::const_buffer(header3data, header3size),
    boost::asio::const_buffer(header4data, header4size),
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

void Session::SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const char* header4data, const uint16_t header4size, const char* header5data, const uint16_t header5size, const uint8_t* data, const uint16_t size)
{
  const std::array<boost::asio::const_buffer, 7> buffers =
  {
    boost::asio::const_buffer(header1data, header1size),
    boost::asio::const_buffer(header2data, header2size),
    boost::asio::const_buffer(header3data, header3size),
    boost::asio::const_buffer(header4data, header4size),
    boost::asio::const_buffer(header5data, header5size),
    boost::asio::const_buffer(data, size)
  };
  SendBuffers(buffers);
}

headers::Transport Session::GetTransport() const
{
  headers::Transport transport;
  for (const auto& udpunicastsocket : udpunicastsockets_)
  {
    boost::system::error_code err;
    transport.transports_.push_back(headers::TransportData(std::string("RTP"),
                                    std::string("AVP"),
                                    headers::PROTOCOLTYPE_UDP,
                                    headers::ROUTINGTYPE_UNICAST,
                                    headers::Interleaved(),
                                    boost::asio::ip::address(),
                                    std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none),
                                    std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(udpunicastsocket->rtpport_, udpunicastsocket->rtcpport_),
                                    std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(udpunicastsocket->rtpsocket_.local_endpoint(err).port(), udpunicastsocket->rtcpsocket_.is_initialized() ? udpunicastsocket->rtcpsocket_->local_endpoint(err).port() : boost::optional<uint16_t>(boost::none)),
                                    headers::SSrc(ssrc_),
                                    headers::MODE_PLAY));
  }

  if (interleavedrtpchannel_.is_initialized())
  {
    transport.transports_.push_back(headers::TransportData(std::string("RTP"), std::string("AVP"), headers::PROTOCOLTYPE_TCP, headers::ROUTINGTYPE_UNICAST, headers::Interleaved(*interleavedrtpchannel_, interleavedrtcpchannel_), boost::asio::ip::address(), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), headers::SSrc(ssrc_), headers::MODE_PLAY));

  }

  return transport;
}

void Session::ResetInterleavedConnection()
{
  interleavedrtpchannel_.reset();
  interleavedrtcpchannel_.reset();
  interleavedconnection_.reset();
}

Session& Session::operator=(Session&& session)
{
  name_ = session.name_;
  interleavedconnection_ = session.interleavedconnection_;
  interleavedrtpchannel_ = session.interleavedrtpchannel_;
  interleavedrtcpchannel_ = session.interleavedrtcpchannel_;
  return *this;
}

void Session::HandleRtp(const boost::system::error_code& err, std::size_t size, boost::shared_ptr<UdpUnicastTransport> udpunicasttransport)
{
  const rtp::Packet* rtppacket = reinterpret_cast<const rtp::Packet*>(udpunicasttransport->rtpbuffer_.data());
  if (err)
  {
    const std::vector< boost::shared_ptr<UdpUnicastTransport> >::const_iterator i = std::find(udpunicastsockets_.begin(), udpunicastsockets_.end(), udpunicasttransport);
    if (i != udpunicastsockets_.end())
    {
      udpunicastsockets_.erase(i);

    }
    return;
  }

  if ((size < sizeof(rtp::Packet)) || (size < rtppacket->GetSize()))
  {
  
    return;
  }
  
  const unsigned int rtppacketsize = rtppacket->GetSize();
  RtpPacket(rtppacket, reinterpret_cast<const char*>(udpunicasttransport->rtpbuffer_.data()) + rtppacketsize, size - rtppacketsize);

  udpunicasttransport->rtpsocket_.async_receive_from(boost::asio::buffer(udpunicasttransport->rtpbuffer_), udpunicasttransport->rtpendpoint_, boost::bind(&Session::HandleRtp, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, udpunicasttransport));
}

void Session::HandleRtcp(const boost::system::error_code& err, std::size_t size, boost::shared_ptr<UdpUnicastTransport> udpunicasttransport)
{
  if (err)
  {
    std::vector< boost::shared_ptr<UdpUnicastTransport> >::iterator i = std::find(udpunicastsockets_.begin(), udpunicastsockets_.end(), udpunicasttransport);
    if (i != udpunicastsockets_.end())
    {
      udpunicastsockets_.erase(i);

    }
    return;
  }

  rtcp::Packets rtcppackets;
  unsigned int offset = 0;
  while (offset < size)
  {
    rtcp::Packet rtcppacket;
    if (rtcppacket.Init(udpunicasttransport->rtpbuffer_.data() + offset, size - offset))
    {

      break;
    }
    rtcppackets.rtcppackets_.push_back(rtcppacket);
    offset += (rtcppacket.length_ + 1) * 4;
  }
  RtcpPacket(rtcppackets);

  udpunicasttransport->rtpsocket_.async_receive_from(boost::asio::buffer(*udpunicasttransport->rtcpbuffer_), udpunicasttransport->rtcpendpoint_, boost::bind(&Session::HandleRtcp, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, udpunicasttransport));
}

}

}
