// receiverserviceinterface.hpp
//

#ifndef IDQZG3TV3ODLAIS0ONRYO662BFTCL9W6FT
#define IDQZG3TV3ODLAIS0ONRYO662BFTCL9W6FT

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

namespace receiver
{

///// Globals /////

extern const std::string RECEIVERNAMESPACEURI;

extern const std::string CONFIGURERECEIVER;
extern const std::string CREATERECEIVER;
extern const std::string DELETERECEIVER;
extern const std::string GETRECEIVER;
extern const std::string GETRECEIVERS;
extern const std::string GETRECEIVERSTATE;
extern const std::string GETSERVICECAPABILITIES;
extern const std::string SETRECEIVERMODE;

///// Classes ////

class ReceiverServiceInterface : public OnvifService
{
 public:

  ReceiverServiceInterface(const int onvifauthenticationtimediff);
  virtual ~ReceiverServiceInterface();

  int Init();
  void Destroy();

  virtual onvif::ReceiverCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::receiver::Capabilities ServiceCapabilities() const = 0;

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

 protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse ConfigureReceiver(const uint64_t receivertoken, const onvif::receiver::ReceiverConfiguration& configuration) = 0;
  virtual ServerResponse CreateReceiver(const onvif::receiver::ReceiverConfiguration& configuration) = 0;
  virtual ServerResponse DeleteReceiver(const uint64_t receivertoken) = 0;
  virtual ServerResponse GetReceiver(const uint64_t receivertoken) = 0;
  virtual ServerResponse GetReceivers() = 0;
  virtual ServerResponse GetReceiverState(const uint64_t receivertoken) = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;
  virtual ServerResponse SetReceiverMode(const uint64_t receivertoken, const onvif::receiver::RECEIVERMODE mode) = 0;

  static const std::string XADDR_;

 private:



};

}

}

}

#endif
