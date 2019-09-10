// main.cpp
//

///// Includes /////

#include "main.hpp"

#include <utility/ioservicepool.hpp>

utility::IoServicePool ioservicepool(4, 4, []() {}, []() {});

///// Functions /////

GTEST_API_ int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
