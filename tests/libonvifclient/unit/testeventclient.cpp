// testeventclient.cpp
//

///// Includes /////

#include "testeventclient.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 8000;
const std::string EVENTURI("/onvif/event");

///// Methods /////

TestEventClient::TestEventClient()
{
  
}

TestEventClient::~TestEventClient()
{
  
}

void TestEventClient::SetUp()
{
  Init(PORT, EVENTURI);

  // Setup dummy server
  dummyeventservice_ = std::make_unique<DummyEventService>();
  ASSERT_FALSE(httpserver_.AddServer(EVENTURI, dummyeventservice_.get()));
}

void TestEventClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(EVENTURI));
  dummyeventservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestEventClient, GetEventProperties)
{
  std::vector<std::string> topicnamespacelocation = { std::string("TopicNamespaceLocation") };
  bool fixedtopicset = true;
  ws::TopicSet topicset = ws::TopicSet(ws::Device(ws::Trigger(ws::DigitalInput(MessageDescription(true, { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) },{ ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) }, { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) },{ ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) }, { ItemListDescription({ SimpleItemDescription(std::string("name1"), std::string("type")), SimpleItemDescription(std::string("name2"), std::string("type")) },{ ElementItemDescription(std::string("name1"), std::string("type")), ElementItemDescription(std::string("name2"), std::string("type")) }) })))));
  std::vector<std::string> topicexpressiondialect = { std::string("TopicExpressionDialect") };
  std::vector<std::string> messagecontentfilterdialect = { std::string("MessageContentFilterDialect") };
  std::vector<std::string> producerpropertiesfilterdialect = { std::string("ProducerPropertiesFilterDialect") };
  std::vector<std::string> messagecontentschemalocation = { std::string("MessageContentSchemaLocation") };

  dummyeventservice_->SetGetEventProperties(topicnamespacelocation, fixedtopicset, topicset, topicexpressiondialect, messagecontentfilterdialect, producerpropertiesfilterdialect, messagecontentschemalocation);

  event::GetEventPropertiesResponse geteventpropertiesresponse = client_->GetEventPropertiesFuture().get();
  ASSERT_GE(geteventpropertiesresponse.latency_, 0);
  ASSERT_FALSE(geteventpropertiesresponse.Error());
  ASSERT_EQ(topicnamespacelocation, geteventpropertiesresponse.topicnamespacelocation_);
  ASSERT_EQ(fixedtopicset, *geteventpropertiesresponse.fixedtopicset_);
  ASSERT_EQ(topicset, *geteventpropertiesresponse.topicset_);
  ASSERT_EQ(topicexpressiondialect, geteventpropertiesresponse.topicexpressiondialect_);
  ASSERT_EQ(messagecontentfilterdialect, geteventpropertiesresponse.messagecontentfilterdialect_);
  ASSERT_EQ(producerpropertiesfilterdialect, geteventpropertiesresponse.producerpropertiesfilterdialect_);
  ASSERT_EQ(messagecontentschemalocation, geteventpropertiesresponse.messagecontentschemalocation_);
}

TEST_F(TestEventClient, GetServiceCapabilities)
{
  const event::Capabilities capabilities(true, true, true, 5, 6, true);
  dummyeventservice_->SetGetServiceCapabilities(capabilities);

  event::GetServiceCapabilitiesResponse geteventservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(geteventservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(geteventservicecapabilitiesresponse.Error());
  ASSERT_TRUE(geteventservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *geteventservicecapabilitiesresponse.capabilities_);
}

}

}
