// testtlsconfiguration.cpp
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

TEST(TestTLSConfiguration, Compare)
{
  Compare(TLSConfiguration(std::string("CertificateID")));
}

}

}
