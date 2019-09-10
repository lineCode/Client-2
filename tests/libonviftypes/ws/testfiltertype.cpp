// testfilter.cpp
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

TEST(TestFilterType, Compare)
{
  Compare(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) }));
}

}

}
