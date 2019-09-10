// testtransportdata.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/transportdata.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestTransportData, Compare)
{
  Compare(TransportData(std::string("RTP"),
          std::string("AVP"),
          PROTOCOLTYPE_TCP,
          ROUTINGTYPE_UNICAST,
          Interleaved(),
          boost::asio::ip::address::from_string("192.168.0.1"),
          std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(3, 4),
          std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(5, 6),
          std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(7, 8),
          SSrc(5555),
          MODE_PLAY));
}

}

}

}
