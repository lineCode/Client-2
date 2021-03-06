// testsdp.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/sdp.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestSdp, Compare)
{
  Compare(sdp::Sdp(0,
                   sdp::Origin(std::string("username"), 5000, 6000, sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, "hostname"),
                   std::string("sessionname"),
                   std::string("email"),
                   sdp::Bandwidth(sdp::BANDWIDTHTYPE_AS, 5000),
                   sdp::Timing(std::vector< std::pair<uint64_t, uint64_t> >({
                                                                              { 0, 0 },
                                                                              { 0, 0 },
                                                                              { 1, 2 },
                                                                              { 0, 4 }
                                                                            })),
                   std::string("control"),
                   {
                     sdp::Range(0.0, 5.0),
                     sdp::Range(0.0, boost::none)
                   },
                   {
                     sdp::MediaDescription(sdp::Media(sdp::MEDIATYPE_APPLICATION, 10000, 1, sdp::MEDIAPROTOCOL_RTPAVP, { 20, 30, 40 }),
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
                                           boost::none,
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
                                                     })),
                     sdp::MediaDescription(sdp::Media(sdp::MEDIATYPE_AUDIO, 20000, 1, sdp::MEDIAPROTOCOL_RTPAVP, { 100, 101, 102 }),
                                           sdp::Connection({
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("1.2.3.4"), boost::none, 1),
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("2.3.4.5"), boost::none, 1),
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, boost::asio::ip::address::from_string("224.2.1.1"), 255, 1),
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("FF15::101"), boost::none, 1),
                                                             sdp::CONNECTIONDATA(sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP6, boost::asio::ip::address::from_string("ff02::5"), boost::none, 1)
                                                           }),
                                           sdp::Bandwidth(sdp::BANDWIDTHTYPE_CT, 6000),
                                           30.0,
                                           boost::none,
                                           sdp::RtpMap({
                                                         { 1, sdp::RTPMAPDATA("h", 6, boost::none) },
                                                         { 2, sdp::RTPMAPDATA("i", 5, 2) },
                                                         { 3, sdp::RTPMAPDATA("j", 4, boost::none) },
                                                         { 4, sdp::RTPMAPDATA("k", 3, 1) },
                                                         { 5, sdp::RTPMAPDATA("l", 2, boost::none) }
                                                       }),
                                           std::string("control"),
                                           sdp::Fmtp({
                                                       { 1, { sdp::FMTPPARAMETER("x", "e"), sdp::FMTPPARAMETER("b", "2"), sdp::FMTPPARAMETER("c", "3"), sdp::FMTPPARAMETER("d", "4") } },
                                                       { 2, { sdp::FMTPPARAMETER("y", "d"), sdp::FMTPPARAMETER("f", "2"), sdp::FMTPPARAMETER("g", "3"), sdp::FMTPPARAMETER("h", "4") } },
                                                       { 3, { sdp::FMTPPARAMETER("z", "c"), sdp::FMTPPARAMETER("j", "2"), sdp::FMTPPARAMETER("k", "3"), sdp::FMTPPARAMETER("l", "4") } },
                                                       { 4, { sdp::FMTPPARAMETER("m", "b"), sdp::FMTPPARAMETER("n", "2"), sdp::FMTPPARAMETER("o", "3"), sdp::FMTPPARAMETER("p", "4") } },
                                                       { 5, { sdp::FMTPPARAMETER("b", "a"), sdp::FMTPPARAMETER("r", "2"), sdp::FMTPPARAMETER("s", "3"), sdp::FMTPPARAMETER("t", "4") } }
                                                     }))
                   }));
}

}

}
