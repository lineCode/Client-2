// dummyreceiverservice.hpp
//

#ifndef IDKRNQL7R42YFA8KL76Y9OEXQAU64ZN3EN
#define IDKRNQL7R42YFA8KL76Y9OEXQAU64ZN3EN

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

class DummyReceiverService : public server::Service
{
 public:

  DummyReceiverService();
  virtual ~DummyReceiverService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetConfigureReceiver(const std::string& configurereceiverreceivertoken, const receiver::ReceiverConfiguration& configurereceiverconfiguration) { configurereceiverreceivertoken_ = configurereceiverreceivertoken; configurereceiverconfiguration_ = configurereceiverconfiguration; }
  inline void SetCreateReceiver(const receiver::Receiver& createreceiver) { createreceiver_ = createreceiver; }
  inline void SetGetReceiver(const receiver::Receiver& getreceiver) { getreceiver_ = getreceiver; }
  inline void SetGetReceivers(const std::vector<receiver::Receiver>& getreceivers) { getreceivers_ = getreceivers; }
  inline void SetGetServiceCapabilities(const receiver::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }
  inline void SetGetReceiverState(const receiver::ReceiverStateInformation& getreceiverstate) { getreceiverstate_ = getreceiverstate; }

 private:

  std::string configurereceiverreceivertoken_;
  receiver::ReceiverConfiguration configurereceiverconfiguration_;
  receiver::Receiver createreceiver_;
  receiver::Receiver getreceiver_;
  std::vector<receiver::Receiver> getreceivers_;
  receiver::Capabilities getservicecapabilities_;
  receiver::ReceiverStateInformation getreceiverstate_;

};

}

}

#endif
