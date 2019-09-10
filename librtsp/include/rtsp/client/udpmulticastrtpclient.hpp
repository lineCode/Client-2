// udpmulticastrtpclient.hpp
//

#ifndef ID5XVS8TUFVUA0H73CIKNZGQU4MNS1CN52
#define ID5XVS8TUFVUA0H73CIKNZGQU4MNS1CN52

///// Includes /////

#include <boost/asio.hpp>

#include "rtpclient.hpp"

#include <array>
#include <mutex>
#include <stdint.h>
#include <string>

#include "rtsp/rtcp/packets.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

template<class T>
class UDPMulticastRtpClient : public RtpClient<T>
{
 public:

  UDPMulticastRtpClient(uint32_t localssrc, double rtptimestampfrequency, RtpCallback rtpcallback, const boost::shared_ptr<T>& rtpcallbackdataobject, void* rtpcallbackdata, RtcpCallback rtcpcallback, boost::asio::io_service& io, boost::asio::io_service::strand& strand) :
    RtpClient<T>(localssrc, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject, rtpcallbackdata, rtcpcallback),
    rtpsocket_(io),
    rtcpsocket_(io),
    strand_(strand)
  {

  }

  ~UDPMulticastRtpClient()
  {

  }

  int Init(const boost::asio::ip::address& listenaddress, const boost::asio::ip::address& destination, uint16_t rtpport, uint16_t rtcpport)
  {
    multicastrtcpendpoint_ = boost::asio::ip::udp::endpoint(destination, rtcpport);

    if (listenaddress.is_unspecified())
    {

      return 1;
    }

    const boost::asio::ip::udp protocol = destination.is_v4() ? boost::asio::ip::udp::v4() : boost::asio::ip::udp::v6();

    boost::system::error_code err;
    if (rtpsocket_.open(protocol, err))
    {

      return 1;
    }

    if (rtcpsocket_.open(protocol, err))
    {

      return 1;
    }

    rtpsocket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    rtcpsocket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));

    rtpsocket_.set_option(boost::asio::ip::multicast::enable_loopback(false));
    rtcpsocket_.set_option(boost::asio::ip::multicast::enable_loopback(false));

    const boost::asio::ip::address inaddrany = destination.is_v4() ? boost::asio::ip::address(boost::asio::ip::address_v4::any()) : boost::asio::ip::address(boost::asio::ip::address_v6::any());
    if (rtpsocket_.bind(boost::asio::ip::udp::endpoint(inaddrany, rtpport), err))
    {

      return 1;
    }

    if (rtcpsocket_.bind(boost::asio::ip::udp::endpoint(inaddrany, rtcpport), err))
    {

      return 1;
    }

    if (listenaddress.is_v4() && destination.is_v4())
    {
      if (rtpsocket_.set_option(boost::asio::ip::multicast::join_group(destination.to_v4(), listenaddress.to_v4()), err))
      {

        return 1;
      }

      if (rtcpsocket_.set_option(boost::asio::ip::multicast::join_group(destination.to_v4(), listenaddress.to_v4()), err))
      {

        return 1;
      }
    }
    else if (listenaddress.is_v6() && destination.is_v6())
    {
      if (rtpsocket_.set_option(boost::asio::ip::multicast::join_group(destination), err))
      {

        return 1;
      }

      if (rtcpsocket_.set_option(boost::asio::ip::multicast::join_group(destination), err))
      {

        return 1;
      }
    }
    else
    {

      return 1;
    }

    rtpsocket_.async_receive_from(boost::asio::buffer(rtpbuffer_), rtpendpoint_, strand_.wrap(boost::bind(&UDPMulticastRtpClient::HandleRtp, boost::static_pointer_cast<UDPMulticastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
    rtcpsocket_.async_receive_from(boost::asio::buffer(rtcpbuffer_), rtcpendpoint_, strand_.wrap(boost::bind(&UDPMulticastRtpClient::HandleRtcp, boost::static_pointer_cast<UDPMulticastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));

    return 0;
  }

  void Destroy()
  {
    rtpsocket_.close();
    rtcpsocket_.close();

    rtpendpoint_ = boost::asio::ip::udp::endpoint();
    rtcpendpoint_ = boost::asio::ip::udp::endpoint();

    multicastrtcpendpoint_ = boost::asio::ip::udp::endpoint();
  }

  void SendRtcpPackets(const rtcp::Packets& rtcppackets)
  {
    std::lock_guard<std::mutex> lock(rtcpsendmutex_);
    if (multicastrtcpendpoint_.address().is_unspecified())
    {

      return;
    }

    boost::system::error_code err;
    rtcpsocket_.send_to(boost::asio::buffer(rtcppackets.ToData()), multicastrtcpendpoint_, 0, err);
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
    
    rtpsocket_.async_receive_from(boost::asio::buffer(rtpbuffer_), rtpendpoint_, strand_.wrap(boost::bind(&UDPMulticastRtpClient::HandleRtp, boost::static_pointer_cast<UDPMulticastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
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

    rtcpsocket_.async_receive_from(boost::asio::buffer(rtcpbuffer_), rtcpendpoint_, strand_.wrap(boost::bind(&UDPMulticastRtpClient::HandleRtcp, boost::static_pointer_cast<UDPMulticastRtpClient>(this->shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
  }

 private:

  boost::asio::ip::udp::socket rtpsocket_;
  boost::asio::ip::udp::socket rtcpsocket_;

  boost::asio::io_service::strand strand_;

  boost::asio::ip::udp::endpoint multicastrtcpendpoint_;

  boost::asio::ip::udp::endpoint rtpendpoint_;
  boost::asio::ip::udp::endpoint rtcpendpoint_;

  std::mutex rtcpsendmutex_;

  std::array<char, std::numeric_limits<uint16_t>::max()> rtpbuffer_;
  std::array<char, std::numeric_limits<uint16_t>::max()> rtcpbuffer_;

};

}

#endif
