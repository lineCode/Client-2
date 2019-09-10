// testmessagecontent.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestMessageContent, Compare)
{
  Compare(ws::MessageContent(std::string("dialect"), std::string("expression")));
}

}

}
