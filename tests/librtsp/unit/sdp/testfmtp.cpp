// testfmtp.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/fmtp.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestFmtp, Compare)
{
  Compare(sdp::Fmtp({
                      { 0, { sdp::FMTPPARAMETER("a", "1"), sdp::FMTPPARAMETER("b", "2"), sdp::FMTPPARAMETER("c", "3"), sdp::FMTPPARAMETER("d", "4") } },
                      { 1, { sdp::FMTPPARAMETER("e", "1"), sdp::FMTPPARAMETER("f", "2"), sdp::FMTPPARAMETER("g", "3"), sdp::FMTPPARAMETER("h", "4") } },
                      { 2, { sdp::FMTPPARAMETER("i", "1"), sdp::FMTPPARAMETER("j", "2"), sdp::FMTPPARAMETER("k", "3"), sdp::FMTPPARAMETER("l", "4") } },
                      { 3, { sdp::FMTPPARAMETER("m", "1"), sdp::FMTPPARAMETER("n", "2"), sdp::FMTPPARAMETER("o", "3"), sdp::FMTPPARAMETER("p", "4") } },
                      { 4, { sdp::FMTPPARAMETER("q", "1"), sdp::FMTPPARAMETER("r", "2"), sdp::FMTPPARAMETER("s", "3"), sdp::FMTPPARAMETER("t", "4") } }
                    }));
}

}

}
