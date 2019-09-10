// onvifservice.hpp
//

#ifndef IDP0MTLTIWVS6LVEG9HBJ3Q109O4D13D80
#define IDP0MTLTIWVS6LVEG9HBJ3Q109O4D13D80

///// Includes /////

#include "service.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <pugixml.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

extern const std::string ADDRESSING_NAMESPACE;
extern const std::string BASEFAULT_NAMESPACE;
extern const std::string C14N_NAMESPACE;
extern const std::string DEVICE_NAMESPACE;
extern const std::string DEVICEIO_NAMESPACE;
extern const std::string DS_NAMESPACE;
extern const std::string ERROR_NAMESPACE;
extern const std::string EVENT_NAMESPACE;
extern const std::string RECEIVER_NAMESPACE;
extern const std::string RECORDING_NAMESPACE;
extern const std::string REPLAY_NAMESPACE;
extern const std::string RESOURCEFAULT_NAMESPACE;
extern const std::string RPC_NAMESPACE;
extern const std::string SCHEMA_INSTANCE_NAMESPACE;
extern const std::string SCHEMA_NAMESPACE;
extern const std::string SEARCH_NAMESPACE;
extern const std::string SOAP_NAMESPACE;
extern const std::string TOPICS_NAMESPACE;
extern const std::string TYPES_NAMESPACE;
extern const std::string WSNT_NAMESPACE;
extern const std::string WSSE_NAMESPACE;
extern const std::string WSTOP_NAMESPACE;
extern const std::string XOP_NAMESPACE;
extern const std::string XS_NAMESPACE;

extern const std::string ADDRESSING_NAMESPACE_ATTRIBUTE;
extern const std::string BASEFAULT_NAMESPACE_ATTRIBUTE;
extern const std::string C14N_NAMESPACE_ATTRIBUTE;
extern const std::string DEVICE_NAMESPACE_ATTRIBUTE;
extern const std::string DEVICEIO_NAMESPACE_ATTRIBUTE;
extern const std::string DS_NAMESPACE_ATTRIBUTE;
extern const std::string ERROR_NAMESPACE_ATTRIBUTE;
extern const std::string EVENT_NAMESPACE_ATTRIBUTE;
extern const std::string RECEIVER_NAMESPACE_ATTRIBUTE;
extern const std::string RECORDING_NAMESPACE_ATTRIBUTE;
extern const std::string REPLAY_NAMESPACE_ATTRIBUTE;
extern const std::string RESOURCEFAULT_NAMESPACE_ATTRIBUTE;
extern const std::string RPC_NAMESPACE_ATTRIBUTE;
extern const std::string SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE;
extern const std::string SCHEMA_NAMESPACE_ATTRIBUTE;
extern const std::string SEARCH_NAMESPACE_ATTRIBUTE;
extern const std::string SOAP_NAMESPACE_ATTRIBUTE;
extern const std::string TOPICS_NAMESPACE_ATTRIBUTE;
extern const std::string TYPES_NAMESPACE_ATTRIBUTE;
extern const std::string WSNT_NAMESPACE_ATTRIBUTE;
extern const std::string WSSE_NAMESPACE_ATTRIBUTE;
extern const std::string WSTOP_NAMESPACE_ATTRIBUTE;
extern const std::string XOP_NAMESPACE_ATTRIBUTE;
extern const std::string XS_NAMESPACE_ATTRIBUTE;

///// Prototypes /////

std::string Envelope(const std::string& operation, const std::string& header, const std::string& body, const std::string namespaces, const std::vector<std::string>& additionalnamespaces);

///// Classes /////

class OnvifService : public Service
{
 public:

  OnvifService(const int onvifmajorversion, const int onvifminorversion, const int onvifauthenticationtimediff, const std::vector<std::string>& namespaces);
  virtual ~OnvifService();

  inline int GetOnvifMajorVersion() const { return onvifmajorversion_; }
  inline int GetOnvifMinorVersion() const { return onvifminorversion_; }

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override;
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const override;

  virtual boost::shared_future<ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual const std::string& GetXAddr() const = 0;

 protected:

  virtual ServerResponse GetRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) { return ServerResponse(HTTPSTATUSCODE::NOTFOUND, std::string("Error"), std::string("Error 404 Not Found")); }
  virtual bool IsOperationRestricted(onvif::USERLEVEL userlevel, const std::string& xmlns, const std::string& action) const = 0;
  virtual bool OnvifDigestAuthentication(const std::string& namespaceuri, const std::string& action, evhttp_cmd_type requesttype, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const = 0;
  virtual bool OnvifAuthenticate(const std::string& namespaceuri, const std::string& action, const std::string& username, const std::string& password, const std::string& nonce, const std::string& time) const = 0;
  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) = 0;
  virtual bool OnvifAuthenticate(const std::string& serverpassword, const std::string& clientpassword, const std::string& clientnonce, const std::string& clienttime) const;
  
  std::string GetNamespace(const std::string& name) const;
  std::string GetName(const std::string& name) const;
  std::string GetNamespaceUri(const std::string& ns, const pugi::xml_node node) const; // Search for a specified namespace beginning at node. If ns is empty, it will find the topmost default namespace

  boost::optional<std::string> GetText(const pugi::xpath_node& parent, const char* xpath);

 protected:

  const int onvifmajorversion_;
  const int onvifminorversion_;
  const int onvifauthenticationtimediff_;

  const std::string namespaces_;

};

}

}

#endif
