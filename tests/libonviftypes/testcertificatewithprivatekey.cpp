// testcertificatewithprivatekey.cpp
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

TEST(TestCertificateWithPrivateKey, Compare)
{
  Compare(CertificateWithPrivateKey(std::string("certificateid"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), BinaryData({ 'd', 'e', 'f' }, std::string("contenttype"))));
}

}

}
