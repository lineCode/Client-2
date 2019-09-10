// dummyptzservice.hpp
//

#ifndef IDMBPMSP39V4KH0HU3F704USCFJXB437F7
#define IDMBPMSP39V4KH0HU3F704USCFJXB437F7

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

class DummyPTZService : public server::Service
{
 public:

  DummyPTZService();
  virtual ~DummyPTZService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  inline void SetGetCompatibleConfigurations(const std::vector<PTZConfiguration>& getcompatibleconfigurations) { getcompatibleconfigurations_ = getcompatibleconfigurations; }
  inline void SetGetConfiguration(const PTZConfiguration& getconfiguration) { getconfiguration_ = getconfiguration; }
  inline void SetGetConfigurations(const std::vector<PTZConfiguration>& getconfigurations) { getconfigurations_ = getconfigurations; }
  inline void SetGetNodes(const std::vector<PTZNode>& getnodes) { getnodes_ = getnodes; }
  inline void SetGetServiceCapabilities(const ptz::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  std::vector<PTZConfiguration> getcompatibleconfigurations_;
  PTZConfiguration getconfiguration_;
  std::vector<PTZConfiguration> getconfigurations_;
  std::vector<PTZNode> getnodes_;
  ptz::Capabilities getservicecapabilities_;

};

}

}

#endif
