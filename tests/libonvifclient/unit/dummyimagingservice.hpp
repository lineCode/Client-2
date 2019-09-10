// dummyimagingservice.hpp
//

#ifndef ID780U54GWOW9JN2QBKT9F81UDG26USKRU
#define ID780U54GWOW9JN2QBKT9F81UDG26USKRU

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

class DummyImagingService : public server::Service
{
 public:

  DummyImagingService();
  virtual ~DummyImagingService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetGetImagingSettings(const ImagingSettings20& getimagingsettings) { getimagingsettings_ = getimagingsettings; }
  inline void SetGetOptions(const ImagingOptions20& getoptions) { getoptions_ = getoptions; }
  inline void SetGetServiceCapabilities(const imaging::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  ImagingSettings20 getimagingsettings_;
  ImagingOptions20 getoptions_;
  imaging::Capabilities getservicecapabilities_;

};

}

}

#endif
