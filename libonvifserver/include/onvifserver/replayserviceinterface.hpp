// replayserviceinterface.hpp
//

#ifndef IDRWK2IWNYADR88Y1D7H7A5PT83YBGY93W
#define IDRWK2IWNYADR88Y1D7H7A5PT83YBGY93W

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

namespace replay
{

///// Globals /////

extern const std::string REPLAYNAMESPACEURI;

extern const std::string GETREPLAYCONFIGURATION;
extern const std::string GETREPLAYURI;
extern const std::string GETSERVICECAPABILITIES;
extern const std::string SETREPLAYCONFIGURATION;

///// Classes ////

class ReplayServiceInterface : public OnvifService
{
public:

  ReplayServiceInterface(const int onvifauthenticationtimediff);
  virtual ~ReplayServiceInterface();

  int Init();
  void Destroy();

  virtual onvif::ReplayCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::replay::Capabilities ServiceCapabilities() const = 0;

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse GetReplayConfiguration() = 0;
  virtual ServerResponse GetReplayUri(const std::string& localaddress, const onvif::StreamSetup& streamsetup, const uint64_t recordingtoken) = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;
  virtual ServerResponse SetReplayConfiguration(const onvif::replay::ReplayConfiguration& configuration) = 0;

  static const std::string XADDR_;

private:


};

}

}

}

#endif
