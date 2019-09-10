// testcpuid.cpp
//

///// Includes /////

#include <boost/predef/architecture.h>
#include <gtest/gtest.h>
#include <utility/cpuid.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Classes /////

TEST(CPUID, CPUID)
{
  CPUID cpuid;
#ifdef BOOST_ARCH_X86
  ASSERT_TRUE((cpuid.GetVendor() == "GenuineIntel") || (cpuid.GetVendor() == "AuthenticAMD"));
#endif
}

}

}
