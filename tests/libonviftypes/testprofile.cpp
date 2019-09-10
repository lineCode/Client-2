// testprofile.cpp
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

TEST(TestProfile, Compare)
{
	/*TODO Compare(Profile(std::string("name"),
                  VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)), 
                  AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")),
                  VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264,
                                            VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5),
                                            Mpeg4Configuration(5, MPEG4PROFILE_ASP),
                                            H264Configuration(5, H264PROFILE_BASELINE),
                                            MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5, 0)),
                  AudioEncoderConfiguration(std::string("name"),
                                            5,
                                            std::string("token"),
                                            AUDIOENCODING_AAC,
                                            5,
                                            5,
                                            MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true),
                                            Duration(0, 1, 2, 3, 4, 5, 0)),
                  VideoAnalyticsConfiguration(std::string("name"),
                                              5, std::string("token"),
                                              AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) }, { ElementItem({Element("name", { { "a", "b" }, { "c", "d" } }, "text", { Element("name2", { { "e", "f" }, { "g", "h" } }, "text2", { Element("name3", { { "i", "j" }, { "k", "l" } }, "text3", {}) }) } )}), ElementItem({Element("name", { { "z", "y" }, { "c", "d" } }, "text", { Element("name5", { { "e", "f" }, { "g", "h" } }, "text5", { Element("name6", { { "i", "j" }, { "k", "l" } }, "text6", {}) }) } )}) }), std::string("name"), std::string("type")) })),
                  PTZConfiguration(std::string("name"),
                                   5,
                                   std::string("token"),
                                   std::string("nodetoken"),
                                   std::string("defaultabsolutepantiltpositionspace"),
                                   std::string("defaultabsolutezoompositionspace"),
                                   std::string("defaultrelativepantilttranslationspace"),
                                   std::string("defaultrelativezoomtranslationspace"),
                                   std::string("defaultcontinuouspantiltvelocityspace"),
                                   std::string("defaultcontinuouszoomvelocityspace"),
                                   DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5),
                                   std::string("PT5S"),
                                   PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))),
                  MetadataConfiguration(std::string("token"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5, 0), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })),
                  AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5),
                  AudioDecoderConfiguration(std::string("name"), 5, std::string("token")),
                  std::string("token"),
                  true));*/
}

}

}
