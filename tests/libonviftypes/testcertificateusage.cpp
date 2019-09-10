// testcertificateusage.cpp
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

TEST(TestCertificateUsage, Compare)
{
  Compare(CertificateUsage(std::string("usage"), true));
}

}

}
