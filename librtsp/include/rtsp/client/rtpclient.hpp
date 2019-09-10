// rtpclient.hpp
//

#ifndef IDZVR1MFWYJWAWB5ICYWFBU6TTG4GKFB4I
#define IDZVR1MFWYJWAWB5ICYWFBU6TTG4GKFB4I

///// Includes /////

#include <boost/asio.hpp>

#include "rtsp/sdp/addresstype.hpp"

#include <boost/chrono.hpp>
#include <boost/optional.hpp>
#include <chrono>
#include <memory>
#include <string>

#include "rtsp/headers/ssrc.hpp"
#include "rtpclient.hpp"
#include "rtsp/rtp/packet.hpp"
#include "rtsp/rtcp/packet.hpp"

///// Namespaces /////

namespace rtsp
{

///// Declerations /////

typedef void(*RtpCallback)(const boost::system::error_code& err, void* object, void* data, const boost::asio::ip::address& endpoint, const rtp::Packet& rtppacket, const uint8_t* payload, unsigned int payloadsize);
typedef void(*RtcpCallback)(const boost::asio::ip::address& endpoint, const rtcp::Packet&);

///// Classes /////

template<class T>
class RtpClient : public boost::enable_shared_from_this< RtpClient<T> >
{
 public:

  RtpClient(uint32_t localssrc, double rtptimestampfrequency, RtpCallback rtpcallback, const boost::shared_ptr<T>& rtpcallbackdataobject, void* rtpcallbackdata, RtcpCallback rtcpcallback) :
    localssrc_(localssrc),
    rtptimestampfrequency_(rtptimestampfrequency),
    rtpcallback_(rtpcallback),
    rtpcallbackdataobject_(rtpcallbackdataobject),
    rtpcallbackdata_(rtpcallbackdata),
    rtcpcallback_(rtcpcallback),
    totalpackets_(0),
    totalpacketslost_(0),
    temppacketslost_(0),
    temppackets_(0),
    cycles_(0),
    sequencenumber_(0),
    rtptimestamp_(0),
    jitter_(0.0)
  {

  }

  virtual ~RtpClient()
  {

  }

  void ParseRtpPacket(const boost::system::error_code& err, const boost::asio::ip::address& endpoint, const char* buffer, std::size_t size)
  {
    if (err)
    {
      rtpcallback_(err, rtpcallbackdataobject_.get(), rtpcallbackdata_, endpoint, rtp::Packet(), nullptr, 0);
      return;
    }

    if (size < sizeof(rtp::Packet))
    {

      return;
    }

    const rtp::Packet* rtppacket = reinterpret_cast<const rtp::Packet*>(buffer);
    const unsigned int rtppacketsize = rtppacket->GetSize();
    if (rtppacketsize > size)
    {

      return;
    }

    rtpcallback_(err, rtpcallbackdataobject_.get(), rtpcallbackdata_, endpoint, *rtppacket, reinterpret_cast<const uint8_t*>(buffer + rtppacketsize), static_cast<unsigned int>(size) - rtppacketsize);

    {
      const auto now = boost::chrono::system_clock::now();
      if (rtptimestamp_ != 0 && (rtppacket->GetTimestamp() != rtptimestamp_)) // Ignore the first packet, and whenever the packets have been generated on the server at the same time
      {
        jitter_ += (std::abs((boost::chrono::duration_cast<boost::chrono::microseconds>((now - localtimestamp_)).count() * rtptimestampfrequency_ / 1000000.0) - static_cast<double>(((static_cast<int64_t>(rtppacket->GetTimestamp()) - static_cast<int64_t>(rtptimestamp_))))) - jitter_) / 16.0;

      }

      if (!ssrc_.ssrc_.is_initialized()) // If ssrc_ has not been initialised, then we set the ssrc of the current packet to it to lock it in. This avoids confusion if there are somehow multiple streams, just one is chosen and the user has to deal with the consequences
      {
        ssrc_.ssrc_ = rtppacket->GetSendersSrc();

      }

      if (*ssrc_.ssrc_ != rtppacket->GetSendersSrc()) // Ignore all packets that do not have the correct destination
      {

        return;
      }

      ++totalpackets_;

      if (rtppacket->GetSequenceNumber() > sequencenumber_)
      {
        if (rtptimestamp_ != 0) // Is this not the first frame?
        {
          const unsigned int packetslost = (rtppacket->GetSequenceNumber() - sequencenumber_) - 1;
          totalpacketslost_ += packetslost;
          temppacketslost_ += packetslost;
        }
        sequencenumber_ = rtppacket->GetSequenceNumber();
        rtptimestamp_ = rtppacket->GetTimestamp();
        localtimestamp_ = now;
      }
      else if (rtppacket->GetSequenceNumber() == sequencenumber_)
      {
        // Duplicate

      }
      else // if(rtppacket.sequencenumber_ < sequencenumber_)
      {
        // Either we have received a frame out of sync, or we have cycled
        // To work out which, we calculate the absolute sequence difference, and see which one is smaller. This should result in a massive difference and make it very clear which has happened, as long as the server is operating well.
        const int32_t cyclesequencediff = (static_cast<int32_t>(std::numeric_limits<uint16_t>::max()) - static_cast<int32_t>(sequencenumber_)) + static_cast<int32_t>(rtppacket->GetSequenceNumber()) + 1;
        const int32_t outofsyncdiff = sequencenumber_ - rtppacket->GetSequenceNumber();

        if (cyclesequencediff < outofsyncdiff)
        {
          // Cycled
          ++cycles_;
          sequencenumber_ = rtppacket->GetSequenceNumber();
          rtptimestamp_ = rtppacket->GetTimestamp();
          localtimestamp_ = now;
          const unsigned int packetslost = cyclesequencediff - 1;
          totalpacketslost_ += packetslost;
          temppacketslost_ += packetslost;
        }
        else
        {
          // Out of sync

        }
      }
    }
  }

  boost::optional<rtcp::Packet> ParseRtcpPacket(const boost::asio::ip::address& endpoint, const rtcp::Packet& rtcppacket)
  {
    if (rtcpcallback_)
    {
      rtcpcallback_(endpoint, rtcppacket);

    }

    {
      boost::optional<rtcp::Packet> response;
      if (ssrc_.ssrc_.is_initialized() && (rtcppacket.payloadtype_ == rtcp::RTCPPAYLOADTYPE_SENDERREPORT))
      {
        uint8_t fractionlost = temppackets_ ? (temppacketslost_ / temppackets_) * 256 : 0;
        const auto now = boost::chrono::system_clock::now();
        const uint32_t delaysincelastsenderreport = static_cast<uint32_t>(boost::chrono::duration_cast<boost::chrono::microseconds>(now - delaysincelastsenderreport_).count() * 0.065536);
        response = rtcp::Packet(false, localssrc_, { rtcp::ReportBlock(*ssrc_.ssrc_, fractionlost, totalpacketslost_, cycles_, sequencenumber_, static_cast<uint32_t>(jitter_), lastsenderreporttimestamp_, delaysincelastsenderreport) });
        temppacketslost_ = 0;
        temppackets_ = 0;
        lastsenderreporttimestamp_ = static_cast<uint32_t>((rtcppacket.ntptimestamp_ & 0x0000FFFFFFFF0000) >> 16);
        delaysincelastsenderreport_ = now;
      }
      if (ssrc_.ssrc_.is_initialized() && (rtcppacket.payloadtype_ == rtcp::RTCPPAYLOADTYPE_SOURCEDESCRIPTION))
      {
        const std::string user = utility::GetUsername();
        std::string domain = utility::GetComputerName();
        if (domain.empty())
        {
          domain = std::string("domain");

        }

        std::string userdomain;
        if (user.empty())
        {
          userdomain = domain;

        }
        else
        {
          userdomain = user + std::string("@") + domain;

        }

        // Can't be too long or RTCP packet will fall apart
        if (userdomain.size() > std::numeric_limits<uint8_t>::max())
        {
          userdomain = userdomain.substr(0, std::numeric_limits<uint8_t>::max());

        }

        response = rtcp::Packet(false, { rtcp::Chunk(localssrc_,{ rtcp::SourceDescriptionItem(1, static_cast<uint8_t>(userdomain.size()), userdomain.c_str()) }) });

      }

      return response;
    }
  }

  inline void SetSsrc(const headers::SSrc& ssrc) { ssrc_ = ssrc; }
  inline const headers::SSrc& GetSsrc() const { return ssrc_; }

 protected:

  const uint32_t localssrc_;
  const double rtptimestampfrequency_;

  RtpCallback rtpcallback_;
  boost::shared_ptr<T> rtpcallbackdataobject_;
  void* rtpcallbackdata_;
  RtcpCallback rtcpcallback_;

  headers::SSrc ssrc_;

  unsigned int totalpackets_;
  unsigned int totalpacketslost_;
  unsigned int temppacketslost_; // Number of packets lost since last receiver report
  unsigned int temppackets_; // Number of packets since last receiver report
  uint16_t cycles_; // The number of times sequencenumber_ has cycled
  uint16_t sequencenumber_; // The highest sequence number received
  uint32_t rtptimestamp_; // The highest rtp timestamp received
  boost::chrono::system_clock::time_point localtimestamp_; // The local timestamp point when the last rtp packet was received
  uint32_t lastsenderreporttimestamp_; // The middle 32 bits of the previous sender reports ntp timestamp
  boost::chrono::system_clock::time_point delaysincelastsenderreport_; // The local timestamp of the previous sender report
  double jitter_;

};

}

#endif
