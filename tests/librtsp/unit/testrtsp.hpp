// testrtsp.hpp
//

///// Includes /////

#include <gtest/gtest.h>

///// Namespaces /////

namespace rtsp
{

///// Functions /////

template <typename T>
void Compare(const T& type)
{
  ASSERT_EQ(type, T(type.ToString()));
}

}
