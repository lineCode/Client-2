// testeapmethodconfiguration.cpp
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

TEST(TestEAPMethodConfiguration, Compare)
{
  Compare(EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password")));
}

}

}
