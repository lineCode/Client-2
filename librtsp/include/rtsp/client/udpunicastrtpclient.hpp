// udpunicastrtpclient.hpp
//

#ifndef ID57KFEXUVOM4UKS3YT8XB1UH1PCOHCWBQ
#define ID57KFEXUVOM4UKS3YT8XB1UH1PCOHCWBQ

///// Includes /////

#include <boost/asio.hpp>

#include "rtpclient.hpp"

#include <array>
#include <mutex>
#include <stdint.h>

#include "rtsp/rtcp/packets.hpp"
#include "rtsp/sdp/addresstype.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

template<class T>
class UDPUnicastRtpClient : public RtpClient<T>
{
 public:

  UDPUnicastRtpClient(uint32_t localssrc, double rtptimestampfrequency, RtpCallback rtpcallback, const boost::shared_ptr<T>& rtpcallbackdataobject, void* rtpcallbackdata, RtcpCallback rtcpcallback, boost::asio::io_service& io, const boost::asio::io_service::strand& strand) :
    RtpClient<T>(localssrc, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject, rtpcallbackdata, rtcpcallback),
    rtpsocket_(io),
    rtcpsocket_(io),
    strand_(strand)
  {

  }

  ~UDPUnicastRtpClient()
  {
    Destroy();

  }

  int Init(sdp::ADDRESSTYPE addresstype)
  {
    boost::asio::ip::udp protocol((addresstype == sdp::ADDRESSTYPE_IP4) ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6());

    std::vector<boost::asio::ip::udp::socket> tmpsockets; // We can't just rotate normally when searching for a port, we have to collect invalid ports to avoid just getting the same one back every time
    boost::asio::ip::udp::socket socket1(rtpsocket_.get_io_service());
    boost::asio::ip::udp::socket socket2(rtpsocket_.get_io_service());

    boost::system::error_code err;
    if (socket1.open(protocol, err))
    {

      return 1;
    }

    // Attempt to get neighbouring ports
    for (int i = 0; i < 50; ++i)
    {
      boost::asio::ip::udp::socket tmpsocket(rtpsocket_.get_io_service());
      if (tmpsocket.open(protocol, err))
      {

        return 1;
      }

      if (tmpsocket.bind(boost::asio::ip::udp::endpoint(protocol, 0), err))
      {

        return 1;
      }

      if (tmpsocket.local_endpoint().port() != std::numeric_limits<unsigned short>::max())
      {
        if (tmpsocket.local_endpoint().port() & 0b1) // Is odd
        {
          // Attempt to bind a port one lower
          if (!socket1.bind(boost::asio::ip::udp::endpoint(protocol, tmpsocket.local_endpoint().port() - 1), err))
          {
            socket2 = boost::asio::ip::udp::socket(std::move(tmpsocket));
            break; // Success
          }
        }
        else
        {
          // Attempt to bind a bind one higher
          if (!socket1.bind(boost::asio::ip::udp::endpoint(protocol, tmpsocket.local_endpoint().port() + 1), err))
          {
            socket2 = boost::asio::ip::udp::socket(std::move(tmpsocket));
            break; // Success
          }
        }
      }

      tmpsockets.emplace_back(std::move(tmpsocket));
    }

    if (!socket2.is_open())
    {

      return 1;
    }

    if (socket1.local_endpoint().port() & 0b1) // Is odd
    {
      rtpsocket_ = boost::asio::ip::udp::socket(std::move(socket2));
      rtcpsocket_ = boost::asio::ip::udp::socket(std::move(socket1));
    }
    else // Is even
    {
      rtpsocket_ = boost::asio::ip::udp::socket(std::move(socket1));
      rtcpsocket_ = boost::asio::ip::udp::socket(std::move(socket2));
    }

    rtpsocket_.async_receive_from(boost::asio::buffer(rtpbuffer_), rtpendpoint_, boost::bind(&UDPUnicastRtpClient::HandleRtp, boost::static_pointer_cast<UDPUnicastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    rtcpsocket_.async_receive_from(boost::asio::buffer(rtcpbuffer_), rtcpendpoint_, boost::bind(&UDPUnicastRtpClient::HandleRtcp, boost::static_pointer_cast<UDPUnicastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    return 0;
  }

  void Destroy()
  {
    rtpsocket_.close();
    rtcpsocket_.close();

    serverrtpendpoint_ = boost::asio::ip::udp::endpoint();
    serverrtcpendpoint_ = boost::asio::ip::udp::endpoint();
  }

  void SetServerEndpoints(const boost::asio::ip::address& address, uint16_t rtpport, uint16_t rtcpport)
  {
    std::lock_guard<std::mutex> lock(rtcpsendmutex_);
    serverrtpendpoint_ = boost::asio::ip::udp::endpoint(address, rtpport);
    serverrtcpendpoint_ = boost::asio::ip::udp::endpoint(address, rtcpport);
  }

  void SendRtcpPackets(const rtcp::Packets& rtcppackets)
  {
    std::lock_guard<std::mutex> lock(rtcpsendmutex_);
    if (serverrtcpendpoint_.address().is_unspecified())
    {

      return;
    }

    boost::system::error_code err;
    rtcpsocket_.send_to(boost::asio::buffer(rtcppackets.ToData()), serverrtcpendpoint_, 0, err);
  }

  inline uint16_t GetRtpPort() const { return rtpsocket_.local_endpoint().port(); }
  inline uint16_t GetRtcpPort() const { return rtcpsocket_.local_endpoint().port(); }

  void HandleRtp(const boost::system::error_code& err, std::size_t size)
  {
    this->ParseRtpPacket(err, rtpendpoint_.address(), rtpbuffer_.data(), size);
    if (err)
    {

      return;
    }
    
    rtpsocket_.async_receive_from(boost::asio::buffer(rtpbuffer_), rtpendpoint_, strand_.wrap(boost::bind(&UDPUnicastRtpClient::HandleRtp, boost::static_pointer_cast<UDPUnicastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
  }

  void HandleRtcp(const boost::system::error_code& err, std::size_t size)
  {
    if (err)
    {

      return;
    }

    rtcp::Packets response; // Package the rtcp response packets in the same way they were received(Panasonic fails to stream if done otherwise)
    unsigned int offset = 0;
    while (offset < size)
    {
      rtcp::Packet rtcppacket;
      if (rtcppacket.Init(rtcpbuffer_.data() + offset, size - offset))
      {

        break;
      }

      auto rtcpresponse = this->ParseRtcpPacket(rtcpendpoint_.address(), rtcppacket);
      if (rtcpresponse.is_initialized())
      {
        response.rtcppackets_.push_back(*rtcpresponse);

      }

      offset += (rtcppacket.length_ + 1) * 4;
    }

    if (response.rtcppackets_.size())
    {
      SendRtcpPackets(response);

    }

    rtcpsocket_.async_receive_from(boost::asio::buffer(rtcpbuffer_), rtcpendpoint_, strand_.wrap(boost::bind(&UDPUnicastRtpClient::HandleRtcp, boost::static_pointer_cast<UDPUnicastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
  }

 private:

  boost::asio::ip::udp::socket rtpsocket_;
  boost::asio::ip::udp::socket rtcpsocket_;

  boost::asio::io_service::strand strand_;

  boost::asio::ip::udp::endpoint rtpendpoint_;
  boost::asio::ip::udp::endpoint rtcpendpoint_;
  
  boost::asio::ip::udp::endpoint serverrtpendpoint_;
  boost::asio::ip::udp::endpoint serverrtcpendpoint_;

  std::mutex rtcpsendmutex_;

  std::array<char, std::numeric_limits<uint16_t>::max()> rtpbuffer_;
  std::array<char, std::numeric_limits<uint16_t>::max()> rtcpbuffer_;

};

}

#endif
