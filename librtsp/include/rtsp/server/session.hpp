// session.hpp
//

#ifndef IDJ25N0SBABBNL7T4RK8JHECJLWN1HHQYS
#define IDJ25N0SBABBNL7T4RK8JHECJLWN1HHQYS

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/enable_shared_from_this.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <string>
#include <vector>

#include "rtsp/headers/transport.hpp"
#include "rtsp/rtcp/packets.hpp"
#include "rtsp/rtp/packet.hpp"
#include "rtsp/rtsprequest.hpp"
#include "rtsp/server/connection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Connection;

///// Structures /////

struct UdpUnicastTransport
{
  UdpUnicastTransport(boost::asio::ip::udp::socket& rtpsocket, boost::optional<boost::asio::ip::udp::socket>& rtcpsocket, const boost::asio::ip::address& destination, const uint16_t rtpport, const boost::optional<uint16_t>& rtcpport) :
    rtpsocket_(std::move(rtpsocket)),
    rtcpsocket_(std::move(rtcpsocket)),
    destination_(destination),
    rtpport_(rtpport),
    rtcpport_(rtcpport),
    rtcpbuffer_(rtcpsocket.is_initialized() ? boost::optional< std::array<char, std::numeric_limits<uint16_t>::max()> >(std::array<char, std::numeric_limits<uint16_t>::max()>()) : boost::none)
  {
  
  }

  boost::asio::ip::udp::socket rtpsocket_;
  boost::optional<boost::asio::ip::udp::socket> rtcpsocket_;
  boost::asio::ip::address destination_;
  uint16_t rtpport_;
  boost::optional<uint16_t> rtcpport_;

  std::array<char, std::numeric_limits<uint16_t>::max()> rtpbuffer_;
  boost::optional< std::array<char, std::numeric_limits<uint16_t>::max()> > rtcpbuffer_;

  boost::asio::ip::udp::endpoint rtpendpoint_;
  boost::asio::ip::udp::endpoint rtcpendpoint_;

};

///// Classes /////

class Session : public boost::enable_shared_from_this<Session>
{
 public:

  Session(boost::asio::io_service& io, const uint32_t ssrc);
  Session(Session&& session);
  virtual ~Session();

  virtual unsigned int Init(const std::string& name, const boost::shared_ptr<Connection>& interleavedconnection, const headers::Transport& transport); // This returns the RtspResponse code, so 200 is success
  virtual void Destroy();

  virtual RtspResponse GetParameter(const RtspRequest& request);
  virtual RtspResponse SetParameter(const RtspRequest& request);
  virtual RtspResponse Play(const RtspRequest& request);
  virtual RtspResponse Pause(const RtspRequest& request);

  virtual void RtpPacket(const rtp::Packet* rtppacket, const char* data, const size_t size); // Called when an RTP packet is received
  virtual void RtcpPacket(const rtcp::Packets& rtcppackets);
  
  // This is organised like this so we can scatter gather without having to allocate an additional buffer on the heap
  virtual void SendRtpFrame(const uint8_t* data, const uint16_t size);
  virtual void SendRtpFrame(const char* headerdata, const uint16_t headersize, const uint8_t* data, const uint16_t size);
  virtual void SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const uint8_t* data, const uint16_t size);
  virtual void SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const uint8_t* data, const uint16_t size);
  virtual void SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const char* header4data, const uint16_t header4size, const uint8_t* data, const uint16_t size);
  virtual void SendRtpFrame(const char* header1data, const uint16_t header1size, const char* header2data, const uint16_t header2size, const char* header3data, const uint16_t header3size, const char* header4data, const uint16_t header4size, const char* header5data, const uint16_t header5size, const uint8_t* data, const uint16_t size);

  inline uint32_t GetSsrc() const { return ssrc_; }

  inline const std::string& GetName() const { return name_; }

  boost::asio::deadline_timer* GetTimeout() { return timeout_.get(); }

  headers::Transport GetTransport() const;

  boost::shared_ptr<Connection>& GetInterleavedConnection() { return interleavedconnection_; }

  void ResetInterleavedConnection();

  Session& operator=(Session&& session);

 protected:

   // These will pass the rtp data to every transport
  void HandleRtp(const boost::system::error_code& err, std::size_t size, boost::shared_ptr<UdpUnicastTransport> udpunicasttransport);
  void HandleRtcp(const boost::system::error_code& err, std::size_t size, boost::shared_ptr<UdpUnicastTransport> udpunicasttransport);

  const uint32_t ssrc_;

  std::string name_;

  boost::shared_ptr<boost::asio::deadline_timer> timeout_;

  std::vector< boost::shared_ptr<UdpUnicastTransport> > udpunicastsockets_;
  boost::shared_ptr<Connection> interleavedconnection_; // Required for interleaved RTP/RTCP
  boost::optional<uint8_t> interleavedrtpchannel_;
  boost::optional<uint8_t> interleavedrtcpchannel_;

 private:

  template<size_t S>
  void SendBuffers(const std::array<boost::asio::const_buffer, S>& buffers)
  {
    for (auto& udpunicastsocket : udpunicastsockets_)
    {
      boost::system::error_code err;
      udpunicastsocket->rtpsocket_.send_to(buffers, boost::asio::ip::udp::endpoint(udpunicastsocket->destination_, udpunicastsocket->rtpport_), 0, err);
    }

    if (interleavedrtpchannel_.is_initialized() && interleavedconnection_)
    {
      // Batch the block together so we are guaranteed to send it in one chunk and it won't be separated by other messages
      const uint16_t s = ntohs(static_cast<uint16_t>(boost::asio::buffer_size(buffers)));
      const std::array<char, 4> interleavedheader =
      {
        '$',
        static_cast<const char>(*interleavedrtpchannel_),
        reinterpret_cast<const char*>(&s)[0],
        reinterpret_cast<const char*>(&s)[1]
      };

      std::array<boost::asio::const_buffer, S + 1> interleavedbuffers;
      interleavedbuffers[0] = boost::asio::const_buffer(interleavedheader.data(), interleavedheader.size());
      std::copy(buffers.cbegin(), buffers.cend(), interleavedbuffers.begin() + 1);

      if (interleavedconnection_->Write(interleavedbuffers))
      {
        interleavedconnection_.reset();
        interleavedrtpchannel_.reset();
      }
    }
  }
};

}

}

#endif
