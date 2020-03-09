// testrequest.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/rtsprequest.hpp>

#include "testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestRequest, Compare)
{
  const std::string password("password");
  const RtspRequest request(REQUESTTYPE_DESCRIBE, std::string("url"), 5, CONTENTTYPE_APPLICATIONSDP, std::string("transport"), std::string("session"), Range(true, 1000, 5000), true, true, CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), Parameters(), KEEPALIVEMODE_NONE, std::string(), AUTHENTICATIONTYPE_DIGEST, std::string("authenticationuri"), std::string("username"), std::string("realm"), std::string("nonce"));
  request.ToString(password);
  ASSERT_EQ(request, RtspRequest(request.ToString(password)));
}

}

}

}
