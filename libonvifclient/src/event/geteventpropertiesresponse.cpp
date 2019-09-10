// geteventpropertiesresponse.cpp
//

///// Includes /////

#include "onvifclient/eventclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Methods /////

GetEventPropertiesResponse::GetEventPropertiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetEventPropertiesResponse::GetEventPropertiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& topicnamespacelocation, const boost::optional<bool>& fixedtopicset, const boost::optional<ws::TopicSet>& topicset, const std::vector<std::string>& topicexpressiondialect, const std::vector<std::string>& messagecontentfilterdialect, const std::vector<std::string>& producerpropertiesfilterdialect, const std::vector<std::string>& messagecontentschemalocation) :
  Response(client, localendpoint, latency, message),
  topicnamespacelocation_(topicnamespacelocation),
  fixedtopicset_(fixedtopicset),
  topicset_(topicset),
  topicexpressiondialect_(topicexpressiondialect),
  messagecontentfilterdialect_(messagecontentfilterdialect),
  producerpropertiesfilterdialect_(producerpropertiesfilterdialect),
  messagecontentschemalocation_(messagecontentschemalocation)
{

}

GetEventPropertiesResponse::~GetEventPropertiesResponse()
{

}

}

}
