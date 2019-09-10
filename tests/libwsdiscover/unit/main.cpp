// main.cpp
//

///// Includes /////

#include "main.hpp"

///// Functions /////

GTEST_API_ int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
