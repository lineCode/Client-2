// udpunicastrtpclient.cpp
//

///// Includes /////

#include "rtsp/client/udpunicastrtpclient.hpp"

#include <boost/bind.hpp>

#include "rtsp/rtcp/packet.hpp"
#include "rtsp/rtp/packet.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

UDPUnicastRtpClient::UDPUnicastRtpClient(uint32_t localssrc, double rtptimestampfrequency, RtpCallback rtpcallback, void* rtpcallbackdata, RtcpCallback rtcpcallback, boost::asio::io_service& io_)

UDPUnicastRtpClient::~UDPUnicastRtpClient()

int UDPUnicastRtpClient::Init(sdp::ADDRESSTYPE addresstype)

void UDPUnicastRtpClient::Destroy()

void UDPUnicastRtpClient::SetServerEndpoints(const boost::asio::ip::address& address, uint16_t rtpport, uint16_t rtcpport)

void UDPUnicastRtpClient::SendRtcpPackets(const rtcp::Packets& rtcppackets)

void UDPUnicastRtpClient::HandleRtp(const boost::system::error_code& err, std::size_t size)

void UDPUnicastRtpClient::HandleRtcp(const boost::system::error_code& err, std::size_t size)

}
