// transportdata.hpp
//

#ifndef IDIPI758KMXISIXUD2TM2BF8HOS8A3M4NT
#define IDIPI758KMXISIXUD2TM2BF8HOS8A3M4NT

///// Includes /////

#include <boost/asio.hpp>

#include <boost/optional.hpp>
#include <stdint.h>
#include <string>
#include <utility>

#include "rtsp/headers/interleaved.hpp"
#include "rtsp/headers/mode.hpp"
#include "rtsp/headers/protocoltype.hpp"
#include "rtsp/headers/routingtype.hpp"
#include "rtsp/headers/ssrc.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

struct TransportData
{
  TransportData();
  TransportData(const std::string& text);
  TransportData(const std::string& transportprotocol, const std::string& profile, PROTOCOLTYPE lowertransport, ROUTINGTYPE routingtype, const Interleaved& interleaved, const boost::asio::ip::address& destination, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& port, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& clientport, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& serverport, const SSrc& ssrc, MODE mode);
  
  PROTOCOLTYPE GetProtocolType() const;
  ROUTINGTYPE GetRoutingType() const;
  MODE GetMode() const;

  bool IsValid() const;
  std::string ToString() const;
  bool operator==(const TransportData& rhs) const;

  std::string transportprotocol_;
  std::string profile_;
  PROTOCOLTYPE lowertransport_;

  ROUTINGTYPE routingtype_;

  Interleaved interleaved_;

  boost::asio::ip::address destination_;

  std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> > port_;
  std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> > clientport_;
  std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> > serverport_;

  SSrc ssrc_;

  MODE mode_;

};

}

}

#endif
