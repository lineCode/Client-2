// testcertificatestatus.cpp
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

TEST(TestCertificateStatus, Compare)
{
  Compare(CertificateStatus(std::string("certificateid"), true));
}

}

}
