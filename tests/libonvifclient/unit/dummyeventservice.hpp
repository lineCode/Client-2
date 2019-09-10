// dummyeventservice.hpp
//

#ifndef IDDNBZ10ZMT4O9MJ6ZHD8DVNMV5ENRNP75
#define IDDNBZ10ZMT4O9MJ6ZHD8DVNMV5ENRNP75

///// Includes /////

#include <onvifserver/onvifserver.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class DummyEventService : public server::Service
{
 public:

  DummyEventService();
  virtual ~DummyEventService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  inline void SetGetEventProperties(const std::vector<std::string>& geteventpropertiestopicnamespacelocation, bool geteventpropertiesfixedtopicset, const ws::TopicSet& geteventpropertiestopicset, const std::vector<std::string>& geteventpropertiestopicexpressiondialect, const std::vector<std::string>& geteventpropertiesmessagecontentfilterdialect, const std::vector<std::string>& geteventpropertiesproducerpropertiesfilterdialect, const std::vector<std::string>& geteventpropertiesmessagecontentschemalocation) { geteventpropertiestopicnamespacelocation_ = geteventpropertiestopicnamespacelocation; geteventpropertiesfixedtopicset_ = geteventpropertiesfixedtopicset; geteventpropertiestopicset_ = geteventpropertiestopicset; geteventpropertiestopicexpressiondialect_ = geteventpropertiestopicexpressiondialect; geteventpropertiesmessagecontentfilterdialect_ = geteventpropertiesmessagecontentfilterdialect; geteventpropertiesproducerpropertiesfilterdialect_ = geteventpropertiesproducerpropertiesfilterdialect; geteventpropertiesmessagecontentschemalocation_ = geteventpropertiesmessagecontentschemalocation; }
  inline void SetGetServiceCapabilities(const event::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  std::vector<std::string> geteventpropertiestopicnamespacelocation_; bool geteventpropertiesfixedtopicset_; ws::TopicSet geteventpropertiestopicset_; std::vector<std::string> geteventpropertiestopicexpressiondialect_; std::vector<std::string> geteventpropertiesmessagecontentfilterdialect_; std::vector<std::string> geteventpropertiesproducerpropertiesfilterdialect_; std::vector<std::string> geteventpropertiesmessagecontentschemalocation_;
  event::Capabilities getservicecapabilities_;

};

}

}

#endif
