// transportdata.cpp
//

///// Includes /////

#include "rtsp/headers/transportdata.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

TransportData::TransportData() :
  lowertransport_(PROTOCOLTYPE_INVALID),
  routingtype_(ROUTINGTYPE_INVALID),
  mode_(MODE_INVALID)
{

}

TransportData::TransportData(const std::string& text) :
  lowertransport_(PROTOCOLTYPE_INVALID),
  routingtype_(ROUTINGTYPE_INVALID),
  mode_(MODE_INVALID)
{
  std::vector<std::string> parameters;
  boost::algorithm::split_regex(parameters, text, boost::regex("[\\s]*;[\\s]*"));
  for (const auto& parameter : parameters)
  {
    // Transport stuff
    static const boost::regex transportregex("[\\s]*([\\w]+)[\\s]*\\/[\\s]*([\\w]+)[\\s]*(?:\\/[\\s]*(TCP|UDP)[\\s]*)?");
    boost::smatch match;
    if (boost::regex_match(parameter, match, transportregex))
    {
      transportprotocol_ = match[1];
      profile_ = match[2];
      if (match[3].matched)
      {
        lowertransport_ = ProtocolTypeFromString(match[3]);

      }
    }

    // Routing type
    static const boost::regex routingtyperegex("[\\s]*(unicast|multicast)[\\s]*");
    if (boost::regex_match(parameter, match, routingtyperegex))
    {
      routingtype_ = RoutingTypeFromString(match[1]);

    }

    // interleaved
    static const boost::regex interleavedregex("[\\s]*interleaved[\\s]*=[\\s]*([\\d]+(?:[\\s]*-[\\s]*[\\d]+[\\s]*))");
    if (boost::regex_match(parameter, match, interleavedregex))
    {
      interleaved_ = Interleaved(match[1]);

    }

    // destination
    static const boost::regex destinationregex("[\\s]*destination[\\s]*=[\\s]*(.*)[\\s]*");
    if (boost::regex_match(parameter, match, destinationregex))
    {
      try
      {
        destination_ = boost::asio::ip::address::from_string(match[1]);

      }
      catch (...)
      {

      }
    }

    // port
    static const boost::regex portregex("[\\s]*port[\\s]*=[\\s]*([\\d]+)(?:[\\s]*-[\\s]*([\\d]+)[\\s]*)");
    if (boost::regex_match(parameter, match, portregex))
    {
      try
      {
        port_.first = boost::lexical_cast<uint16_t>(match[1]);
        if (match[2].matched)
        {
          port_.second = boost::lexical_cast<uint16_t>(match[2]);

        }
      }
      catch (...)
      {

      }
    }

    // client_port
    static const boost::regex clientportregex("[\\s]*client_port[\\s]*=[\\s]*([\\d]+)(?:[\\s]*-[\\s]*([\\d]+)[\\s]*)");
    if (boost::regex_match(parameter, match, clientportregex))
    {
      try
      {
        clientport_.first = boost::lexical_cast<uint16_t>(match[1]);
        if (match[2].matched)
        {
          clientport_.second = boost::lexical_cast<uint16_t>(match[2]);

        }
      }
      catch (...)
      {

      }
    }

    // server_port
    static const boost::regex serverportregex("[\\s]*server_port[\\s]*=[\\s]*([\\d]+)(?:[\\s]*-[\\s]*([\\d]+)[\\s]*)");
    if (boost::regex_match(parameter, match, serverportregex))
    {
      try
      {
        serverport_.first = boost::lexical_cast<uint16_t>(match[1]);
        if (match[2].matched)
        {
          serverport_.second = boost::lexical_cast<uint16_t>(match[2]);

        }
      }
      catch (...)
      {

      }
    }

    // ssrc
    static const boost::regex ssrcregex("[\\s]*ssrc[\\s]*=[\\s]*([0-9a-fA-F]+)[\\s]*");
    if (boost::regex_match(parameter, match, ssrcregex))
    {
      ssrc_ = SSrc(match[1]);

    }

    // mode
    static const boost::regex moderegex("[\\s]*mode[\\s]*=[\\s]*\\\"[\\s]*([\\w]+)[\\s]*\\\"[\\s]*");
    if (boost::regex_match(parameter, match, moderegex))
    {
      mode_ = ModeTypeFromString(match[1]);

    }
  }
}

TransportData::TransportData(const std::string& transportprotocol, const std::string& profile, PROTOCOLTYPE lowertransport, ROUTINGTYPE routingtype, const Interleaved& interleaved, const boost::asio::ip::address& destination, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& port, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& clientport, const std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >& serverport, const SSrc& ssrc, MODE mode) :
  transportprotocol_(transportprotocol),
  profile_(profile),
  lowertransport_(lowertransport),
  routingtype_(routingtype),
  interleaved_(interleaved),
  destination_(destination),
  port_(port),
  clientport_(clientport),
  serverport_(serverport),
  ssrc_(ssrc),
  mode_(mode)
{

}

PROTOCOLTYPE TransportData::GetProtocolType() const
{
  if (lowertransport_ != PROTOCOLTYPE_INVALID)
  {

    return lowertransport_;
  }

  return PROTOCOLTYPE_UDP;
}

ROUTINGTYPE TransportData::GetRoutingType() const
{
  if (routingtype_ != ROUTINGTYPE_INVALID)
  {

    return routingtype_;
  }

  if (lowertransport_ == PROTOCOLTYPE_TCP)
  {

    return ROUTINGTYPE_UNICAST;
  }

  return ROUTINGTYPE_MULTICAST;
}

MODE TransportData::GetMode() const
{
  if (mode_ != MODE_INVALID)
  {

    return mode_;
  }

  return MODE_PLAY;
}

bool TransportData::IsValid() const
{
  if (transportprotocol_.empty())
  {

    return false;
  }

  if (profile_.empty())
  {

    return false;
  }

  if ((lowertransport_ == PROTOCOLTYPE_TCP) && (routingtype_ == ROUTINGTYPE_MULTICAST))
  {

    return false;
  }

  if ((lowertransport_ == PROTOCOLTYPE_UDP) && interleaved_.rtp_.is_initialized())
  {

    return false;
  }

  if (interleaved_.rtp_.is_initialized() && clientport_.first.is_initialized())
  {

    return false;
  }

  return true;
}

std::string TransportData::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  std::string result;

  // Transport stuff
  result += transportprotocol_ + std::string("/") + profile_;
  if (lowertransport_ != PROTOCOLTYPE_INVALID)
  {
    result += (std::string("/") + rtsp::headers::ToString(GetProtocolType()));

  }

  // Routing type
  if (routingtype_ != ROUTINGTYPE_INVALID)
  {
    result += (std::string(";") + rtsp::headers::ToString(GetRoutingType()));

  }

  // interleaved
  if (interleaved_.rtp_.is_initialized())
  {
    result += std::string(";interleaved=") + interleaved_.ToString();

  }

  // destination
  if (!destination_.is_unspecified())
  {
    result += std::string(";destination=") + destination_.to_string();

  }

  // port
  if (port_.first.is_initialized())
  {
    result += std::string(";port=") + std::to_string(*port_.first);
    if (port_.second.is_initialized())
    {
      result += std::string("-") + std::to_string(*port_.second);

    }
  }

  // client_port
  if (clientport_.first.is_initialized())
  {
    result += std::string(";client_port=") + std::to_string(*clientport_.first);
    if (clientport_.second.is_initialized())
    {
      result += std::string("-") + std::to_string(*clientport_.second);

    }
  }

  // server_port
  if (serverport_.first.is_initialized())
  {
    result += std::string(";server_port=") + std::to_string(*serverport_.first);
    if (serverport_.second.is_initialized())
    {
      result += std::string("-") + std::to_string(*serverport_.second);

    }
  }

  // ssrc
  if (ssrc_.ssrc_.is_initialized())
  {
    result += std::string(";ssrc=") + ssrc_.ToString();

  }

  // ssrc
  if (mode_ != MODE_INVALID)
  {
    result += std::string(";mode=\"") + rtsp::headers::ToString(GetMode()) + std::string("\"");

  }

  return result;
}

bool TransportData::operator==(const TransportData& rhs) const
{
  return ((transportprotocol_ == rhs.transportprotocol_) && (profile_ == rhs.profile_) && (lowertransport_ == rhs.lowertransport_) && (routingtype_ == rhs.routingtype_) && (interleaved_ == rhs.interleaved_) && (destination_ == rhs.destination_) && (port_ == rhs.port_) && (clientport_ == rhs.clientport_) && (serverport_ == rhs.serverport_) && (ssrc_ == rhs.ssrc_) && (mode_ == rhs.mode_));
}

}

}
