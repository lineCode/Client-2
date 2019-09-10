// testversion.cpp
//

///// Includes /////

#include <gtest/gtest.h>

#include <utility/version.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Classes /////

TEST(Version, Compare)
{
  ASSERT_TRUE(Version(2, 3, 4) > Version(1, 2, 3));
  ASSERT_TRUE(Version(3, 2, 1) > Version(2, 3, 4));
  ASSERT_FALSE(Version(2, 3, 4) > Version(3, 2, 1));
  ASSERT_FALSE(Version(3, 2, 1) > Version(3, 2, 1));
  
  ASSERT_TRUE(Version(1, 2, 3) < Version(2, 3, 4));
  ASSERT_TRUE(Version(2, 3, 4) < Version(3, 2, 1));
  ASSERT_TRUE(Version(2, 3, 4) < Version(3, 2, 1));
  ASSERT_FALSE(Version(3, 2, 1) < Version(3, 2, 1));
}

TEST(Version, Comparison)
{
  ASSERT_NE(Version(2, 3, 4), Version(1, 2, 3));
  ASSERT_EQ(Version(3, 2, 1), Version(3, 2, 1));
}

}

}
