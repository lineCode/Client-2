// testtransport.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestTransport, Compare)
{
  Compare(Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP)));
}

}

}
