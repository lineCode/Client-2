// testdot1xconfiguration.cpp
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

TEST(TestDot1XConfiguration, Compare)
{
  Compare(Dot1XConfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password"))));
}

}

}
