// testmetadataconfiguration.cpp
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

TEST(TestMetadataConfiguration, Compare)
{
  /*TODO Compare(MetadataConfiguration(std::string("token"),
                                METADATACOMPRESSIONTYPE_GZIP,
                                std::string("name"),
                                5,
                                PTZFilter(true, false),
                                EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })),
                                true,
                                MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true),
                                Duration(0, 1, 2, 3, 4, 5, 0),
                                AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })));*/
}

}

}
