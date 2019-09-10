// testdynamicdnsinformation.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDynamicDNSInformation, Compare)
{
  Compare(DynamicDNSInformation(DYNAMICDNSTYPE_CLIENTUPDATES, std::string("name"), Duration(0, 1, 2, 3, 4, 5, 0)));
}

}

}
