// testcertificate.cpp
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

TEST(TestCertificate, Compare)
{
  Compare(Certificate(std::string("token"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))));
}

}

}
