// testconnection.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/connection.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestConnection, Compare)
{
  Compare(sdp::Connection({
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("1.2.3.4"), boost::none, 1),
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("2.3.4.5"), boost::none, 1),
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("224.2.1.1"), 255, 1),
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                            sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("ff02::5"), boost::none, 1)
                          }));
}

}

}
