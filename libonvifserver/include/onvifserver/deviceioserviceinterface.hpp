// deviceioserviceinterface.hpp
//

#ifndef IDP2PZQ93HUADMK60FAQJ8Q9QK414UZGJK
#define IDP2PZQ93HUADMK60FAQJ8Q9QK414UZGJK

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <memory>
#include <string>

#include "onvifservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace deviceio
{

///// Globals /////

extern const std::string DEVICEIONAMESPACEURI;

extern const std::string GETSERVICECAPABILITIES;

///// Classes ////

class DeviceIOServiceInterface : public OnvifService
{
public:

  DeviceIOServiceInterface(const int onvifauthenticationtimediff);
  virtual ~DeviceIOServiceInterface();

  int Init();
  void Destroy();

  virtual onvif::DeviceIOCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::deviceio::Capabilities ServiceCapabilities() const = 0;

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

  virtual int GetRelayOutputs() const = 0;

protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse GetServiceCapabilities() = 0;

  static const std::string XADDR_;

private:


};

}

}

}

#endif
