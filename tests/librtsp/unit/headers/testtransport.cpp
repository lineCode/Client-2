// testtransport.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/transport.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestTransport, Compare)
{
  Compare(Transport(std::vector<TransportData>({
                                                 TransportData(std::string("RTP"), std::string("AVP"), PROTOCOLTYPE_TCP, ROUTINGTYPE_UNICAST, Interleaved(3, 4), boost::asio::ip::address::from_string("192.168.0.1"), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(3, 4), SSrc(5555), MODE_PLAY),
                                                 TransportData(std::string("RTP"), std::string("AVP"), PROTOCOLTYPE_UDP, ROUTINGTYPE_MULTICAST, Interleaved(), boost::asio::ip::address::from_string("192.168.0.1"), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(9, 10), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(11, 12), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(13, 14), SSrc(6666), MODE_RECORD)
                                               })));
}

}

}

}
