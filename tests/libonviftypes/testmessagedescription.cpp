// testmessagedescription.cpp
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

TEST(TestMessageDescription, Compare)
{
  Compare(MessageDescription(true,
                             { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) }, { ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) },
                             { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) }, { ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) },
                             { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) }, { ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) }));
}

}

}
