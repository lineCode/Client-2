// dummyanalyticsservice.hpp
//

#ifndef IDIK0BOU67YJHHEG2S449TOXEGRYQJG5M2
#define IDIK0BOU67YJHHEG2S449TOXEGRYQJG5M2

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

class DummyAnalyticsService : public server::Service
{
 public:

  DummyAnalyticsService();
  virtual ~DummyAnalyticsService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetGetServiceCapabilities(const analytics::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  analytics::Capabilities getservicecapabilities_;

};

}

}

#endif
