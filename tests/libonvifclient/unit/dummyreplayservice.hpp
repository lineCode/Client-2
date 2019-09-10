// dummyreplayservice.hpp
//

#ifndef IDPBDGATCRFTNINWSS2575WPS6W5Z9KZQ4
#define IDPBDGATCRFTNINWSS2575WPS6W5Z9KZQ4

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

class DummyReplayService : public server::Service
{
 public:

  DummyReplayService();
  virtual ~DummyReplayService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetGetReplayUri(const std::string& getreplayuri) { getreplayuri_ = getreplayuri; }
  inline void SetGetServiceCapabilities(const replay::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  std::string getreplayuri_;
  replay::Capabilities getservicecapabilities_;

};

}

}

#endif
