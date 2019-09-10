// testtopicexpression.cpp
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

TEST(TestTopicExpression, Compare)
{
  Compare(ws::TopicExpression(std::string("dialect"), std::string("expression")));
}

}

}
