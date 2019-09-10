// testwwwauthenticate.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/wwwauthenticate.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestAuthenticate, Compare)
{
  Compare(WWWAuthenticate(AUTHENTICATIONTYPE_BASIC, std::string(), std::string(), boost::none));
  Compare(WWWAuthenticate(AUTHENTICATIONTYPE_DIGEST, std::string("realm"), std::string("nonce"), true));
  Compare(WWWAuthenticate(AUTHENTICATIONTYPE_DIGEST, std::string("realm-dash"), std::string("nonce"), true));
}

}

}

}
