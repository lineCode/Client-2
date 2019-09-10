// eventserviceinterface.hpp
//

#ifndef IDTWU68WN8OI1KRSC1MN37KRZB3B2ZLTE8
#define IDTWU68WN8OI1KRSC1MN37KRZB3B2ZLTE8

///// Includes /////

#include <memory>
#include <string>
#include <onviftypes/onviftypes.hpp>

#include "onvifservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace event
{

///// Globals /////

extern const std::string EVENTSNAMESPACEURI;

extern const std::string CREATEPULLPOINTSUBSCRIPTION;
extern const std::string GETEVENTPROPERTIES;
extern const std::string GETSERVICECAPABILITIES;
extern const std::string PULLMESSAGES;
extern const std::string RENEW;
extern const std::string SETSYNCHRONIZATIONPOINT;
extern const std::string UNSUBSCRIBE;

///// Classes ////

class EventServiceInterface : public OnvifService
{
public:

  EventServiceInterface(const int onvifauthenticationtimediff);
  virtual ~EventServiceInterface();

  int Init();
  void Destroy();

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

  virtual onvif::EventCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::event::Capabilities ServiceCapabilities() const = 0;

protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse CreatePullPointSubscription(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const boost::optional<std::string>& messageid, const boost::optional<Element>& topicfilter, const boost::optional<Element>& messagefilter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy) = 0;
  virtual ServerResponse GetEventProperties(const boost::optional<std::string>& messageid) = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;
  virtual boost::shared_future<ServerResponse> PullMessages(const std::string& remoteaddress, const boost::optional<uint64_t>& subscriptionid, const boost::optional<std::string>& messageid, const onvif::Duration& timeout, const int messagelimit) = 0;
  virtual ServerResponse Renew(const std::string& remoteaddress, const boost::optional<uint64_t>& subscriptionid, const boost::optional<std::string>& messageid, const std::string& terminationtime) = 0;
  virtual ServerResponse SetSynchronizationPoint(const std::string& remoteaddress, const boost::optional<uint64_t>& subscriptionid, const boost::optional<std::string>& messageid) = 0;
  virtual onvif::server::ServerResponse Unsubscribe(const std::string& remoteaddress, const boost::optional<uint64_t>& subscriptionid, const boost::optional<std::string>& messageid) = 0;

  static const std::string XADDR_;

private:


};

}

}

}

#endif
