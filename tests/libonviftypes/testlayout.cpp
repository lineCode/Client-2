// layout.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestLayout, Compare)
{
  Compare(Layout({ PaneLayout(std::string("pane1"), Rectangle(5.0f, 6.0f, 7.0f, 8.0f)), PaneLayout(std::string("pane2"), Rectangle(5.0f, 6.0f, 7.0f, 8.0f)) }));
}

}

}
