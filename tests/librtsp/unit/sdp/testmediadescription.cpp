// testmediadescription.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/mediadescription.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestMediaDescription, Compare)
{
  Compare(sdp::MediaDescription(sdp::Media(sdp::MEDIATYPE_APPLICATION, 10000, 1, sdp::MEDIAPROTOCOL_RTPAVP, { 20, 30, 40 }),
                                sdp::Connection({
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("1.2.3.4"), boost::none, 1),
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("2.3.4.5"), boost::none, 1),
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("224.2.1.1"), 255, 1),
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                                                  sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("ff02::5"), boost::none, 1)
                                                }),
                                sdp::Bandwidth(sdp::BANDWIDTHTYPE_AS, 5000),
                                25.0,
                                sdp::Range(0.0, 1.0),
                                sdp::RtpMap({
                                              { 0, sdp::RTPMAPDATA("a", 1, boost::none) },
                                              { 1, sdp::RTPMAPDATA("b", 2, 2) },
                                              { 2, sdp::RTPMAPDATA("c", 3, boost::none) },
                                              { 3, sdp::RTPMAPDATA("d", 4, 3) },
                                              { 4, sdp::RTPMAPDATA("e", 5, boost::none) }
                                            }),
                                std::string("control"),
                                sdp::Fmtp({
                                            { 0, { sdp::FMTPPARAMETER("a", "1"), sdp::FMTPPARAMETER("b", "2"), sdp::FMTPPARAMETER("c", "3"), sdp::FMTPPARAMETER("d", "4") } },
                                            { 1, { sdp::FMTPPARAMETER("e", "1"), sdp::FMTPPARAMETER("f", "2"), sdp::FMTPPARAMETER("g", "3"), sdp::FMTPPARAMETER("h", "4") } },
                                            { 2, { sdp::FMTPPARAMETER("i", "1"), sdp::FMTPPARAMETER("j", "2"), sdp::FMTPPARAMETER("k", "3"), sdp::FMTPPARAMETER("l", "4") } },
                                            { 3, { sdp::FMTPPARAMETER("m", "1"), sdp::FMTPPARAMETER("n", "2"), sdp::FMTPPARAMETER("o", "3"), sdp::FMTPPARAMETER("p", "4") } },
                                            { 4, { sdp::FMTPPARAMETER("q", "1"), sdp::FMTPPARAMETER("r", "2"), sdp::FMTPPARAMETER("s", "3"), sdp::FMTPPARAMETER("t", "4") } }
                                          })));
}

TEST(TestMediaDescription, GetControl)
{
  ASSERT_EQ(std::string("rtsp://test.com/test/control.sdp"), sdp::MediaDescription(sdp::Media(), sdp::Connection(), sdp::Bandwidth(), 0.0, boost::none, sdp::RtpMap(), std::string("control.sdp"), sdp::Fmtp()).GetControl("rtsp://test.com/test/"));
  ASSERT_EQ(std::string("rtsp://test.com/test/control.sdp"), sdp::MediaDescription(sdp::Media(), sdp::Connection(), sdp::Bandwidth(), 0.0, boost::none, sdp::RtpMap(), std::string("rtsp://test.com/test/control.sdp"), sdp::Fmtp()).GetControl("rtsp://test.com/test/"));
  ASSERT_EQ(std::string("rtsp://test.com/test/control.sdp"), sdp::MediaDescription(sdp::Media(), sdp::Connection(), sdp::Bandwidth(), 0.0, boost::none, sdp::RtpMap(), std::string("rtsp://test.com/test/control.sdp"), sdp::Fmtp()).GetControl("rtsp://test.com/test/"));
}

}

}
