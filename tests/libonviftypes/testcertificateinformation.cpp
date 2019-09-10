// testcertificateinformation.cpp
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

TEST(TestCertificateInformation, Compare)
{
  Compare(CertificateInformation(std::string("certificateid"),
                                 std::string("issuerdn"),
                                 std::string("subjectdn"),
                                 CertificateUsage(std::string("usage1"), true),
                                 CertificateUsage(std::string("usage2"), true),
                                 5,
                                 std::string("version"),
                                 std::string("serialnum"),
                                 std::string("signaturealgorithm"),
                                 DateTimeRange(ws::DateTime(std::string("2002-05-30T09:00:00")), ws::DateTime(std::string("2002-05-30T09:00:01")))));
}

}

}
