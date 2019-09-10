// testntpinformation.cpp
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

TEST(TestNTPInformation, Compare)
{
  Compare(NTPInformation(true,
                         {
                           NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
                           NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
                         },
                         {
                           NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("7.7.7.7"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
                           NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("8.8.8.8"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
                         }));
}

}

}
