// testresponse.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtspresponse.hpp>

#include "testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestResponse, Compare)
{
  Compare(RtspResponse(5, 5, { REQUESTTYPE_OPTIONS, REQUESTTYPE_DESCRIBE }, boost::posix_time::second_clock::local_time(), std::string("realm"), std::string("nonce"), true, std::string("ContentBase"), CONTENTTYPE_INVALID, boost::none, std::string("session"), 50, RtpInfo(std::vector<RTPINFODATA>({ RTPINFODATA(std::string("url1"), 5, 5), RTPINFODATA(std::string("url2"), 6, 6) }))));
}

}

}

}
