// deviceclient.hpp
//

#ifndef IDVWJPE1HGQUS144D96VVBR42BOSD6DALI
#define IDVWJPE1HGQUS144D96VVBR42BOSD6DALI

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>
#include <thread>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Enumerations /////

enum DEVICEOPERATION
{
  DEVICEOPERATION_ADDIPADDRESSFILTER,
  DEVICEOPERATION_ADDSCOPES,
  DEVICEOPERATION_CREATECERTIFICATE,
  DEVICEOPERATION_CREATESTORAGECONFIGURATION,
  DEVICEOPERATION_CREATEUSERS,
  DEVICEOPERATION_DELETECERTIFICATES,
  DEVICEOPERATION_DELETEDOT1XCONFIGURATION,
  DEVICEOPERATION_DELETESTORAGECONFIGURATION,
  DEVICEOPERATION_DELETEUSERS,
  DEVICEOPERATION_GETACCESSPOLICY,
  DEVICEOPERATION_GETCACERTIFICATES,
  DEVICEOPERATION_GETCAPABILITIES,
  DEVICEOPERATION_GETCERTIFICATEINFORMATION,
  DEVICEOPERATION_GETCERTIFICATES,
  DEVICEOPERATION_GETCERTIFICATESSTATUS,
  DEVICEOPERATION_GETCLIENTCERTIFICATEMODE,
  DEVICEOPERATION_GETDEVICEINFORMATION,
  DEVICEOPERATION_GETDISCOVERYMODE,
  DEVICEOPERATION_GETDNS,
  DEVICEOPERATION_GETDYNAMICDNS,
  DEVICEOPERATION_GETDOT11CAPABILITIES,
  DEVICEOPERATION_GETDOT11STATUS,
  DEVICEOPERATION_GETDOT1XCONFIGURATION,
  DEVICEOPERATION_GETDOT1XCONFIGURATIONS,
  DEVICEOPERATION_GETDPADDRESSES,
  DEVICEOPERATION_GETENDPOINTREFERENCE,
  DEVICEOPERATION_GETHOSTNAME,
  DEVICEOPERATION_GETIPADDRESSFILTER,
  DEVICEOPERATION_GETNETWORKDEFAULTGATEWAY,
  DEVICEOPERATION_GETNETWORKINTERFACES,
  DEVICEOPERATION_GETNETWORKPROTOCOLS,
  DEVICEOPERATION_GETPKCS10REQUEST,
  DEVICEOPERATION_GETRELAYOUTPUTS,
  DEVICEOPERATION_GETNTP,
  DEVICEOPERATION_GETREMOTEDISCOVERYMODE,
  DEVICEOPERATION_GETREMOTEUSER,
  DEVICEOPERATION_GETSCOPES,
  DEVICEOPERATION_GETSERVICECAPABILITIES,
  DEVICEOPERATION_GETSERVICES,
  DEVICEOPERATION_GETSTORAGECONFIGURATION,
  DEVICEOPERATION_GETSTORAGECONFIGURATIONS,
  DEVICEOPERATION_GETSYSTEMBACKUP,
  DEVICEOPERATION_GETSYSTEMDATEANDTIME,
  DEVICEOPERATION_GETSYSTEMLOG,
  DEVICEOPERATION_GETSYSTEMSUPPORTINFORMATION,
  DEVICEOPERATION_GETSYSTEMURIS,
  DEVICEOPERATION_GETUSERS,
  DEVICEOPERATION_GETWSDLURL,
  DEVICEOPERATION_GETZEROCONFIGURATION,
  DEVICEOPERATION_LOADCACERTIFICATES,
  DEVICEOPERATION_LOADCERTIFICATES,
  DEVICEOPERATION_LOADCERTIFICATEWITHPRIVATEKEY,
  DEVICEOPERATION_REMOVEIPADDRESSFILTER,
  DEVICEOPERATION_REMOVESCOPES,
  DEVICEOPERATION_RESTORESYSTEM,
  DEVICEOPERATION_SCANAVAILABLEDOT11NETWORKS,
  DEVICEOPERATION_SENDAUXILIARYCOMMAND,
  DEVICEOPERATION_SETACCESSPOLICY,
  DEVICEOPERATION_SETCERTIFICATESSTATUS,
  DEVICEOPERATION_SETCLIENTCERTIFICATEMODE,
  DEVICEOPERATION_SETDISCOVERYMODE,
  DEVICEOPERATION_SETDNS,
  DEVICEOPERATION_SETDOT1XCONFIGURATION,
  DEVICEOPERATION_SETDPADDRESSES,
  DEVICEOPERATION_SETDYNAMICDNS,
  DEVICEOPERATION_SETHOSTNAME,
  DEVICEOPERATION_SETHOSTNAMEFROMDHCP,
  DEVICEOPERATION_SETIPADDRESSFILTER,
  DEVICEOPERATION_SETNETWORKDEFAULTGATEWAY,
  DEVICEOPERATION_SETNETWORKINTERFACES,
  DEVICEOPERATION_SETNETWORKPROTOCOLS,
  DEVICEOPERATION_SETNTP,
  DEVICEOPERATION_SETRELAYOUTPUTSETTINGS,
  DEVICEOPERATION_SETREMOTEDISCOVERYMODE,
  DEVICEOPERATION_SETSYSTEMDATEANDTIME,
  DEVICEOPERATION_SETSYSTEMFACTORYDEFAULT,
  DEVICEOPERATION_SETUSER,
  DEVICEOPERATION_SETZEROCONFIGURATION,
  DEVICEOPERATION_SYSTEMREBOOT
};

///// Declarations /////

class DeviceClient;
class DeviceSignals;

///// Classes /////

class CreateCertificateResponse : public Response<DeviceClient>
{
 public:

  CreateCertificateResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter);
  CreateCertificateResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter, const boost::optional<Certificate>& nvtcertificate);
  virtual ~CreateCertificateResponse();

  boost::optional<std::string> certificateid_;
  boost::optional<std::string> subject_;
  boost::optional<onvif::ws::DateTime> validnotbefore_;
  boost::optional<onvif::ws::DateTime> validnotafter_;

  boost::optional<Certificate> nvtcertificate_;

};

class CreateStorageConfigurationResponse : public Response<DeviceClient>
{
 public:

  CreateStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StorageConfigurationData& storageconfiguration);
  CreateStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StorageConfigurationData& storageconfiguration, const boost::optional<std::string>& token);
  virtual ~CreateStorageConfigurationResponse();

  StorageConfigurationData storageconfiguration_;

  boost::optional<std::string> token_;

};

class DeleteCertificatesResponse : public Response<DeviceClient>
{
 public:

  DeleteCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& certificateid);
  virtual ~DeleteCertificatesResponse();

  std::vector<std::string> certificateid_;

};

class DeleteDot1XConfigurationResponse : public Response<DeviceClient>
{
 public:

  DeleteDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& dot1xconfigurationtoken);
  virtual ~DeleteDot1XConfigurationResponse();

  std::vector<std::string> dot1xconfigurationtoken_;

};

class DeleteStorageConfigurationResponse : public Response<DeviceClient>
{
 public:

  DeleteStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token);
  virtual ~DeleteStorageConfigurationResponse();

  std::vector<std::string> token_;

};

class AddIPAddressFilterResponse : public Response<DeviceClient>
{
 public:

  AddIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const IPAddressFilter& ipaddressfilter);
  virtual ~AddIPAddressFilterResponse();

  IPAddressFilter ipaddressfilter_;

};

class SetZeroConfigurationResponse : public Response<DeviceClient>
{
 public:

  SetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, bool enabled);
  virtual ~SetZeroConfigurationResponse();

  std::string interfacetoken_;
  bool enabled_;

};

class SetClientCertificateModeResponse : public Response<DeviceClient>
{
public:

  SetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool enabled);
  virtual ~SetClientCertificateModeResponse();

  bool enabled_;

};

class SystemRebootResponse : public Response<DeviceClient>
{
 public:

  SystemRebootResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  SystemRebootResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& rebootmessage);
  virtual ~SystemRebootResponse();

  boost::optional<std::string> rebootmessage_;

};

class AddScopesResponse : public Response<DeviceClient>
{
 public:

  AddScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& scopes);
  virtual ~AddScopesResponse();

  std::vector<std::string> scopes_;

};

class CreateUsersResponse : public Response<DeviceClient>
{
 public:

  CreateUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<User>& users);
  virtual ~CreateUsersResponse();

  std::vector<User> users_;

};

class DeleteUsersResponse : public Response<DeviceClient>
{
 public:

  DeleteUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& usernames);
  virtual ~DeleteUsersResponse();

  std::vector<std::string> usernames_;

};

class GetAccessPolicyResponse : public Response<DeviceClient>
{
 public:

  GetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<BinaryData> policyfile);
  virtual ~GetAccessPolicyResponse();

  boost::optional<BinaryData> policyfile_;

};

class GetCACertificatesResponse : public Response<DeviceClient>
{
 public:

  GetCACertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetCACertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& certificates);
  virtual ~GetCACertificatesResponse();

  std::vector<Certificate> certificates_;

};

class GetCapabilitiesResponse : public Response<DeviceClient>
{
 public:

  GetCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, CAPABILITYCATEGORY capabilitycategory);
  GetCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, CAPABILITYCATEGORY capabilitycategory, const boost::optional<Capabilities>& capabilities);
  virtual ~GetCapabilitiesResponse();

  CAPABILITYCATEGORY capabilitycategory_;

  boost::optional<Capabilities> capabilities_;
};

class SendAuxiliaryCommandResponse : public Response<DeviceClient>
{
 public:

  SendAuxiliaryCommandResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& auxiliarycommand);
  SendAuxiliaryCommandResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& auxiliarycommand, const boost::optional<std::string>& auxiliarycommandresponse);
  virtual ~SendAuxiliaryCommandResponse();

  std::string auxiliarycommand_;

  boost::optional<std::string> auxiliarycommandresponse_;
};

class GetCertificateInformationResponse : public Response<DeviceClient>
{
 public:

  GetCertificateInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid);
  GetCertificateInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<CertificateInformation>& certificateinformation);
  virtual ~GetCertificateInformationResponse();

  std::string certificateid_;

  boost::optional<CertificateInformation> certificateinformation_;

};

class GetCertificatesResponse : public Response<DeviceClient>
{
 public:

  GetCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& certificates);
  virtual ~GetCertificatesResponse();

  std::vector<Certificate> certificates_;

};

class GetCertificatesStatusResponse : public Response<DeviceClient>
{
 public:

  GetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateStatus>& certificatesstatus);
  virtual ~GetCertificatesStatusResponse();

  std::vector<CertificateStatus> certificatesstatus_;

};

class GetClientCertificateModeResponse : public Response<DeviceClient>
{
public:
 
  GetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<bool> enabled);
  virtual ~GetClientCertificateModeResponse();

  boost::optional<bool> enabled_;

};

class GetDeviceInformationResponse : public Response<DeviceClient>
{
 public:

  GetDeviceInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDeviceInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& manufacturer, const boost::optional<std::string>& model, const boost::optional<std::string>& firmwareversion, const boost::optional<std::string>& serialnumber, const boost::optional<std::string>& hardwareid);
  virtual ~GetDeviceInformationResponse();

  boost::optional<std::string> manufacturer_;
  boost::optional<std::string> model_;
  boost::optional<std::string> firmwareversion_;
  boost::optional<std::string> serialnumber_;
  boost::optional<std::string> hardwareid_;

};

class GetDiscoveryModeResponse : public Response<DeviceClient>
{
 public:

  GetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DISCOVERYMODE> discoverymode);
  virtual ~GetDiscoveryModeResponse();

  boost::optional<DISCOVERYMODE> discoverymode_;

};

class GetDNSResponse : public Response<DeviceClient>
{
 public:

  GetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DNSInformation> dnsinformation);
  virtual ~GetDNSResponse();

  boost::optional<DNSInformation> dnsinformation_;

};

class GetDot1XConfigurationResponse : public Response<DeviceClient>
{
 public:

  GetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& dot1xconfigurationtoken);
  GetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& dot1xconfigurationtoken, const boost::optional<Dot1XConfiguration>& dot1xconfiguration);
  virtual ~GetDot1XConfigurationResponse();

  std::string dot1xconfigurationtoken_;
  boost::optional<Dot1XConfiguration> dot1xconfiguration_;

};

class GetDot1XConfigurationsResponse : public Response<DeviceClient>
{
 public:

  GetDot1XConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDot1XConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Dot1XConfiguration>& dot1xconfiguration);
  virtual ~GetDot1XConfigurationsResponse();

  std::vector<Dot1XConfiguration> dot1xconfiguration_;

};

class GetSystemUrisResponse : public Response<DeviceClient>
{
 public:
   
  GetSystemUrisResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetSystemUrisResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<SystemLogUriList>& systemloguris, const boost::optional<std::string>& supportinfouri, const boost::optional<std::string>& systembackupuri);
  virtual ~GetSystemUrisResponse();

  boost::optional<SystemLogUriList> systemloguris_;
  boost::optional<std::string> supportinfouri_;
  boost::optional<std::string> systembackupuri_;

};

class GetDot11CapabilitiesResponse : public Response<DeviceClient>
{
 public:

  GetDot11CapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDot11CapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Dot11Capabilities>& capabilities);
  virtual ~GetDot11CapabilitiesResponse();

  boost::optional<Dot11Capabilities> capabilities_;

};

class GetDot11StatusResponse : public Response<DeviceClient>
{
 public:

  GetDot11StatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken);
  GetDot11StatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const boost::optional<Dot11Status>& status);
  virtual ~GetDot11StatusResponse();

  std::string interfacetoken_;

  boost::optional<Dot11Status> status_;
  
};

class SetAccessPolicyResponse : public Response<DeviceClient>
{
 public:

  SetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& messageconst, const boost::optional<BinaryData>& policyfile);
  virtual ~SetAccessPolicyResponse();

  boost::optional<BinaryData> policyfile_;

};

class GetPkcs10RequestResponse : public Response<DeviceClient>
{
 public:

  GetPkcs10RequestResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<std::string> subject, const boost::optional<BinaryData>& attributes);
  GetPkcs10RequestResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<std::string> subject, const boost::optional<BinaryData>& attributes, const boost::optional<BinaryData>& pkcs10request);
  virtual ~GetPkcs10RequestResponse();

  std::string certificateid_;
  boost::optional<std::string> subject_;
  boost::optional<BinaryData> attributes_;

  boost::optional<BinaryData> pkcs10request_;

};

class GetDPAddressesResponse : public Response<DeviceClient>
{
 public:

  GetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkHost>& dpaddress);
  virtual ~GetDPAddressesResponse();

  std::vector<NetworkHost> dpaddress_;

};

class GetDynamicDNSResponse : public Response<DeviceClient>
{
 public:

  GetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DynamicDNSInformation> dynamicdnsinformation);
  virtual ~GetDynamicDNSResponse();

  boost::optional<DynamicDNSInformation> dynamicdnsinformation_;

};

class GetEndpointReferenceResponse : public Response<DeviceClient>
{
 public:

  GetEndpointReferenceResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetEndpointReferenceResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& guid);
  virtual ~GetEndpointReferenceResponse();

  boost::optional<std::string> guid_;

};

class GetHostnameResponse : public Response<DeviceClient>
{
 public:

  GetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<HostnameInformation> hostnameinformation);
  virtual ~GetHostnameResponse();

  boost::optional<HostnameInformation> hostnameinformation_;

};

class GetIPAddressFilterResponse : public Response<DeviceClient>
{
 public:

  GetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<IPAddressFilter> ipaddressfilter);
  virtual ~GetIPAddressFilterResponse();

  boost::optional<IPAddressFilter> ipaddressfilter_;

};

class GetNetworkDefaultGatewayResponse : public Response<DeviceClient>
{
 public:

  GetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<NetworkGateway> networkgateway);
  virtual ~GetNetworkDefaultGatewayResponse();

  boost::optional<NetworkGateway> networkgateway_;

};

class GetNetworkInterfacesResponse : public Response<DeviceClient>
{
 public:

  GetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkInterface>& networkinterfaces);
  virtual ~GetNetworkInterfacesResponse();

  boost::optional<NetworkInterface> GetNetworkInterface(const std::string& token) const;

  std::vector<NetworkInterface> networkinterfaces_;

};

class GetNetworkProtocolsResponse : public Response<DeviceClient>
{
 public:

  GetNetworkProtocolsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetNetworkProtocolsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkProtocol>& networkprotocols);
  virtual ~GetNetworkProtocolsResponse();

  std::vector<NetworkProtocol> networkprotocols_;

};

class GetNTPResponse : public Response<DeviceClient>
{
 public:

  GetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<NTPInformation> ntpinformation);
  virtual ~GetNTPResponse();

  boost::optional<NTPInformation> ntpinformation_;

};

class GetRelayOutputsResponse : public Response<DeviceClient>
{
public:

  GetRelayOutputsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRelayOutputsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<RelayOutput>& relayoutputs);
  virtual ~GetRelayOutputsResponse();

  std::vector<RelayOutput> relayoutputs_;

};

class GetRemoteDiscoveryModeResponse : public Response<DeviceClient>
{
 public:

  GetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DISCOVERYMODE> remotediscoverymode);
  virtual ~GetRemoteDiscoveryModeResponse();

  boost::optional<DISCOVERYMODE> remotediscoverymode_;

};

class GetRemoteUserResponse : public Response<DeviceClient>
{
 public:

  GetRemoteUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRemoteUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<RemoteUser> remoteuser);
  virtual ~GetRemoteUserResponse();

  boost::optional<RemoteUser> remoteuser_;

};

class GetScopesResponse : public Response<DeviceClient>
{
 public:

  GetScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Scope>& scopes);
  virtual ~GetScopesResponse();

  std::string GetName() const;

  std::vector<Scope> scopes_;

};

class GetServiceCapabilitiesResponse : public Response<DeviceClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<DeviceServiceCapabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<DeviceServiceCapabilities> capabilities_;
};

class GetServicesResponse : public Response<DeviceClient>
{
 public:

  GetServicesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool includecapability);
  GetServicesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool includecapability, const std::vector<Service>& services);
  virtual ~GetServicesResponse();

  boost::optional<Service> GetService(const std::string& xaddr) const;

  bool includecapability_;
  std::vector<Service> services_;

};

class GetStorageConfigurationResponse : public Response<DeviceClient>
{
 public:

  GetStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<StorageConfiguration>& storageconfiguration);
  virtual ~GetStorageConfigurationResponse();

  std::string token_;
  boost::optional<StorageConfiguration> storageconfiguration_;

};

class GetStorageConfigurationsResponse : public Response<DeviceClient>
{
 public:

  GetStorageConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetStorageConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<StorageConfiguration>& storageconfigurations);
  virtual ~GetStorageConfigurationsResponse();

  std::vector<StorageConfiguration> storageconfigurations_;

};

class GetSystemBackupResponse : public Response<DeviceClient>
{
 public:

  GetSystemBackupResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetSystemBackupResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<BackupFile>& backupfiles);
  virtual ~GetSystemBackupResponse();

  std::vector<BackupFile> backupfiles_;

};

class GetSystemDateAndTimeResponse : public Response<DeviceClient>
{
 public:

  GetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<SystemDateTime> systemdatetime);
  virtual ~GetSystemDateAndTimeResponse();

  boost::optional<SystemDateTime> systemdatetime_;
};

class GetSystemLogResponse : public Response<DeviceClient>
{
 public:

  GetSystemLogResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, SYSTEMLOGTYPE logtype);
  GetSystemLogResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, SYSTEMLOGTYPE logtype, const boost::optional<SystemLog>& systemlog);
  virtual ~GetSystemLogResponse();

  SYSTEMLOGTYPE LOGLEVEL_;
  boost::optional<SystemLog> systemlog_;

};

class GetSystemSupportInformationResponse : public Response<DeviceClient>
{
 public:

  GetSystemSupportInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetSystemSupportInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<SupportInformation> supportinformation);
  virtual ~GetSystemSupportInformationResponse();

  boost::optional<SupportInformation> supportinformation_;

};

class GetUsersResponse : public Response<DeviceClient>
{
 public:

  GetUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<User>& users);
  virtual ~GetUsersResponse();

  std::vector<User> users_;

};

class GetWsdlUrlResponse : public Response<DeviceClient>
{
 public:

  GetWsdlUrlResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetWsdlUrlResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& wsdlurl);
  virtual ~GetWsdlUrlResponse();

  boost::optional<std::string> wsdlurl_;

};

class GetZeroConfigurationResponse : public Response<DeviceClient>
{
 public:

  GetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<NetworkZeroConfiguration>& zeroconfiguration);
  virtual ~GetZeroConfigurationResponse();

  boost::optional<NetworkZeroConfiguration> zeroconfiguration_;

};

class LoadCACertificatesResponse : public Response<DeviceClient>
{
 public:

  LoadCACertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& cacertificate);
  virtual ~LoadCACertificatesResponse();

  std::vector<Certificate> cacertificate_;

};

class SetCertificatesStatusResponse : public Response<DeviceClient>
{
public:

  SetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateStatus>& certificatestatus);
  virtual ~SetCertificatesStatusResponse();

  std::vector<CertificateStatus> certificatestatus_;

};

class LoadCertificatesResponse : public Response<DeviceClient>
{
 public:

  LoadCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& nvtcertificate);
  virtual ~LoadCertificatesResponse();

  std::vector<Certificate> nvtcertificate_;

};

class LoadCertificateWithPrivateKeyResponse : public Response<DeviceClient>
{
 public:

  LoadCertificateWithPrivateKeyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey);
  virtual ~LoadCertificateWithPrivateKeyResponse();

  std::vector<CertificateWithPrivateKey> certificatewithprivatekey_;

};

class RemoveIPAddressFilterResponse : public Response<DeviceClient>
{
 public:

  RemoveIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const IPAddressFilter& ipaddressfilter);
  virtual ~RemoveIPAddressFilterResponse();

  IPAddressFilter ipaddressfilter_;

};

class RemoveScopesResponse : public Response<DeviceClient>
{
 public:

  RemoveScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  RemoveScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& scopes);
  virtual ~RemoveScopesResponse();

  std::vector<std::string> scopes_;

};

class RestoreSystemResponse : public Response<DeviceClient>
{
 public:

  RestoreSystemResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  RestoreSystemResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<BackupFile>& backupfiles);
  virtual ~RestoreSystemResponse();

  std::vector<BackupFile> backupfiles_;

};

class ScanAvailableDot11NetworksResponse : public Response<DeviceClient>
{
 public:

  ScanAvailableDot11NetworksResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken);
  ScanAvailableDot11NetworksResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const std::vector<Dot11AvailableNetworks>& networks);
  virtual ~ScanAvailableDot11NetworksResponse();

  std::string interfacetoken_;
  std::vector<Dot11AvailableNetworks> networks_;

};

class SetDiscoveryModeResponse : public Response<DeviceClient>
{
 public:

  SetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DISCOVERYMODE discoverymode);
  virtual ~SetDiscoveryModeResponse();

  DISCOVERYMODE discoverymode_;

};

class SetDNSResponse : public Response<DeviceClient>
{
 public:

  SetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual);
  virtual ~SetDNSResponse();

  bool fromdhcp_;
  std::vector<std::string> searchdomain_;
  std::vector<IPAddress> dnsmanual_;

};

class SetDot1XConfigurationResponse : public Response<DeviceClient>
{
 public:

  SetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const Dot1XConfiguration& dot1xconfiguration);
  virtual ~SetDot1XConfigurationResponse();

  Dot1XConfiguration dot1xconfiguration_;

};

class SetDPAddressesResponse : public Response<DeviceClient>
{
 public:

  SetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkHost>& dpaddress);
  virtual ~SetDPAddressesResponse();

  std::vector<NetworkHost> dpaddress_;

};

class SetDynamicDNSResponse : public Response<DeviceClient>
{
 public:

  SetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl);
  virtual ~SetDynamicDNSResponse();

  DYNAMICDNSTYPE type_;
  boost::optional<std::string> name_;
  boost::optional<Duration> ttl_;

};

class SetHostnameFromDHCPResponse : public Response<DeviceClient>
{
 public:

  SetHostnameFromDHCPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp);
  SetHostnameFromDHCPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const boost::optional<bool>& rebootneeded);
  virtual ~SetHostnameFromDHCPResponse();

  bool fromdhcp_;
  boost::optional<bool> rebootneeded_;

};

class SetHostnameResponse : public Response<DeviceClient>
{
 public:

  SetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name);
  virtual ~SetHostnameResponse();

  std::string name_;

};

class SetIPAddressFilterResponse : public Response<DeviceClient>
{
 public:

  SetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const IPAddressFilter& ipaddressfilter);
  virtual ~SetIPAddressFilterResponse();

  IPAddressFilter ipaddressfilter_;

};

class SetNetworkDefaultGatewayResponse : public Response<DeviceClient>
{
 public:

  SetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  SetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address);
  virtual ~SetNetworkDefaultGatewayResponse();

  std::vector<std::string> ipv4address_;
  std::vector<std::string> ipv6address_;

};

class SetNetworkInterfacesResponse : public Response<DeviceClient>
{
 public:

  SetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface);
  SetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface, const boost::optional<bool>& rebootneeded);
  virtual ~SetNetworkInterfacesResponse();

  std::string interfacetoken_;
  onvif::NetworkInterfaceSetConfiguration networkinterface_;
  boost::optional<bool> rebootneeded_;

};

class SetNetworkProtocolsResponse : public Response<DeviceClient>
{
 public:

  SetNetworkProtocolsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkProtocol>& networkprotocols);
  virtual ~SetNetworkProtocolsResponse();

  std::vector<NetworkProtocol> networkprotocols_;

};

class SetNTPResponse : public Response<DeviceClient>
{
 public:

  SetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const std::vector<NetworkHost>& ntpmanual);
  virtual ~SetNTPResponse();

  bool fromdhcp_;
  std::vector<NetworkHost> ntpmanual_;

};

class SetRelayOutputSettingsResponse : public Response<DeviceClient>
{
 public:

  SetRelayOutputSettingsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken, const RelayOutputSettings& properties);
  virtual ~SetRelayOutputSettingsResponse();

  std::string relayoutputtoken_;
  RelayOutputSettings properties_;

};

class SetRemoteDiscoveryModeResponse : public Response<DeviceClient>
{
 public:

  SetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DISCOVERYMODE remotediscoverymode);
  virtual ~SetRemoteDiscoveryModeResponse();

  DISCOVERYMODE remotediscoverymode_;

};

class SetSystemDateAndTimeResponse : public Response<DeviceClient>
{
 public:

  SetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  SetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime);
  virtual ~SetSystemDateAndTimeResponse();

  DATETIMETYPE datetimetype_;
  bool daylightsavings_;
  boost::optional<TimeZone> timezone_;
  boost::optional<DateTime> utcdatetime_;

};

class SetSystemFactoryDefaultResponse : public Response<DeviceClient>
{
 public:

  SetSystemFactoryDefaultResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  SetSystemFactoryDefaultResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, FACTORYDEFAULTTYPE factorydefault);
  virtual ~SetSystemFactoryDefaultResponse();

  FACTORYDEFAULTTYPE factorydefault_;

};

class SetUserResponse : public Response<DeviceClient>
{
 public:

  SetUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  SetUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<User>& users);
  virtual ~SetUserResponse();

  std::vector<User> users_;

};

class DeviceClient : public Client<DEVICEOPERATION>, public boost::enable_shared_from_this<DeviceClient>
{
 public:
  
  using Client::Update;

  DeviceClient();
  virtual ~DeviceClient();

  virtual void Destroy() override;

  // Requests
  void AddIPAddressFilter(const IPAddressFilter& ipaddressfilter);
  void AddScopes(const std::vector<std::string>& scopes);
  void CreateCertificate(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter);
  void CreateStorageConfiguration(const StorageConfigurationData& storageconfiguration);
  void CreateUsers(const std::vector<User>& users);
  void DeleteCertificates(const std::vector<std::string>& certificateid);
  void DeleteDot1XConfiguration(const std::vector<std::string>& dot1xconfigurationtoken);
  void DeleteStorageConfiguration(const std::vector<std::string>& token);
  void DeleteUsers(const std::vector<std::string>& usernames);
  void GetAccessPolicy();
  void GetCACertificates();
  void GetCapabilities(CAPABILITYCATEGORY capabilitycategory);
  void GetCertificateInformation(const std::string& certificateid);
  void GetCertificates();
  void GetCertificatesStatus();
  void GetClientCertificateMode();
  void GetDeviceInformation();
  void GetDiscoveryMode();
  void GetDNS();
  void GetDot11Capabilities();
  void GetDot11Status(const std::string& interfacetoken);
  void GetDot1XConfiguration(const std::string& dot1xconfigurationtoken);
  void GetDot1XConfigurations();
  void GetDPAddresses();
  void GetDynamicDNS();
  void GetEndpointReference();
  void GetHostname();
  void GetIPAddressFilter();
  void GetNetworkDefaultGateway();
  void GetNetworkInterfaces();
  void GetNetworkProtocols();
  void GetPkcs10Request(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes);
  void GetRelayOutputs();
  void GetNTP();
  void GetRemoteDiscoveryMode();
  void GetRemoteUser();
  void GetScopes();
  void GetServiceCapabilities();
  void GetServices(bool includecapability);
  void GetStorageConfiguration(const std::string& token);
  void GetStorageConfigurations();
  void GetSystemBackup();
  void GetSystemDateAndTime();
  void GetSystemLog(SYSTEMLOGTYPE logtype);
  void GetSystemSupportInformation();
  void GetSystemUris();
  void GetUsers();
  void GetWsdlUrl();
  void GetZeroConfiguration();
  void LoadCACertificates(const std::vector<Certificate>& cacertificate);
  void LoadCertificates(const std::vector<Certificate>& nvtcertificate);
  void LoadCertificateWithPrivateKey(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey);
  void RemoveIPAddressFilter(const IPAddressFilter& ipaddressfilter);
  void RemoveScopes(const std::vector<std::string>& scopes);
  void RestoreSystem(const std::vector<BackupFile>& backupfiles);
  void ScanAvailableDot11Networks(const std::string& interfacetoken);
  void SendAuxiliaryCommand(const std::string& auxiliarycommand);
  void SetAccessPolicy(const BinaryData& policyfile);
  void SetCertificatesStatus(const std::vector<CertificateStatus>& certificatestatus);
  void SetClientCertificateMode(bool enabled);
  void SetDiscoveryMode(DISCOVERYMODE discoverymode);
  void SetDNS(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual);
  void SetDot1XConfiguration(const Dot1XConfiguration& dot1xconfiguration);
  void SetDPAddresses(const std::vector<NetworkHost>& dpaddress);
  void SetDynamicDNS(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl);
  void SetHostname(const std::string& name);
  void SetHostnameFromDHCP(bool fromdhcp);
  void SetIPAddressFilter(const IPAddressFilter& ipaddressfilter);
  void SetNetworkDefaultGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address);
  void SetNetworkInterfaces(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface);
  void SetNetworkProtocols(const std::vector<NetworkProtocol>& networkprotocols);
  void SetNTP(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual);
  void SetRelayOutputSettings(const std::string& relayoutputtoken, const RelayOutputSettings& properties);
  void SetRemoteDiscoveryMode(DISCOVERYMODE remotediscoverymode);
  void SetSystemDateAndTime(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime);
  void SetSystemFactoryDefault(FACTORYDEFAULTTYPE factorydefault);
  void SetUser(const std::vector<User>& users);
  void SetZeroConfiguration(const std::string& interfacetoken, bool enabled);
  void SystemReboot();

  // Callback requests
  Connection AddIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const AddIPAddressFilterResponse&)> callback);
  Connection AddScopesCallback(const std::vector<std::string>& scopes, boost::function<void(const AddScopesResponse&)> callback);
  Connection CreateCertificateCallback(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter, boost::function<void(const CreateCertificateResponse&)> callback);
  Connection CreateStorageConfigurationCallback(const StorageConfigurationData& storageconfiguration, boost::function<void(const CreateStorageConfigurationResponse&)> callback);
  Connection CreateUsersCallback(const std::vector<User>& users, boost::function<void(const CreateUsersResponse&)> callback);
  Connection DeleteCertificatesCallback(const std::vector<std::string>& certificateid, boost::function<void(const DeleteCertificatesResponse&)> callback);
  Connection DeleteDot1XConfigurationCallback(const std::vector<std::string>& dot1xconfigurationtoken, boost::function<void(const DeleteDot1XConfigurationResponse&)> callback);
  Connection DeleteStorageConfigurationCallback(const std::vector<std::string>& token, boost::function<void(const DeleteStorageConfigurationResponse&)> callback);
  Connection DeleteUsersCallback(const std::vector<std::string>& usernames, boost::function<void(const DeleteUsersResponse&)> callback);
  Connection GetAccessPolicyCallback(boost::function<void(const GetAccessPolicyResponse&)> callback);
  Connection GetCACertificatesCallback(boost::function<void(const GetCACertificatesResponse&)> callback);
  Connection GetCapabilitiesCallback(CAPABILITYCATEGORY capabilitycategory, boost::function<void(const GetCapabilitiesResponse&)> callback);
  Connection GetCertificateInformationCallback(const std::string& certificateid, boost::function<void(const GetCertificateInformationResponse&)> callback);
  Connection GetCertificatesCallback(boost::function<void(const GetCertificatesResponse&)> callback);
  Connection GetCertificatesStatusCallback(boost::function<void(const GetCertificatesStatusResponse&)> callback);
  Connection GetClientCertificateModeCallback(boost::function<void(const GetClientCertificateModeResponse&)> callback);
  Connection GetDeviceInformationCallback(boost::function<void(const GetDeviceInformationResponse&)> callback);
  Connection GetDiscoveryModeCallback(boost::function<void(const GetDiscoveryModeResponse&)> callback);
  Connection GetDNSCallback(boost::function<void(const GetDNSResponse&)> callback);
  Connection GetDot11CapabilitiesCallback(boost::function<void(const GetDot11CapabilitiesResponse&)> callback);
  Connection GetDot11StatusCallback(const std::string& interfacetoken, boost::function<void(const GetDot11StatusResponse&)> callback);
  Connection GetDot1XConfigurationCallback(const std::string& dot1xconfigurationtoken, boost::function<void(const GetDot1XConfigurationResponse&)> callback);
  Connection GetDot1XConfigurationsCallback(boost::function<void(const GetDot1XConfigurationsResponse&)> callback);
  Connection GetDPAddressesCallback(boost::function<void(const GetDPAddressesResponse&)> callback);
  Connection GetDynamicDNSCallback(boost::function<void(const GetDynamicDNSResponse&)> callback);
  Connection GetEndpointReferenceCallback(boost::function<void(const GetEndpointReferenceResponse&)> callback);
  Connection GetHostnameCallback(boost::function<void(const GetHostnameResponse&)> callback);
  Connection GetIPAddressFilterCallback(boost::function<void(const GetIPAddressFilterResponse&)> callback);
  Connection GetNetworkDefaultGatewayCallback(boost::function<void(const GetNetworkDefaultGatewayResponse&)> callback);
  Connection GetNetworkInterfacesCallback(boost::function<void(const GetNetworkInterfacesResponse&)> callback);
  Connection GetNetworkProtocolsCallback(boost::function<void(const GetNetworkProtocolsResponse&)> callback);
  Connection GetPkcs10RequestCallback(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes, boost::function<void(const GetPkcs10RequestResponse&)> callback);
  Connection GetRelayOutputsCallback(boost::function<void(const GetRelayOutputsResponse&)> callback);
  Connection GetNTPCallback(boost::function<void(const GetNTPResponse&)> callback);
  Connection GetRemoteDiscoveryModeCallback(boost::function<void(const GetRemoteDiscoveryModeResponse&)> callback);
  Connection GetRemoteUserCallback(boost::function<void(const GetRemoteUserResponse&)> callback);
  Connection GetScopesCallback(boost::function<void(const GetScopesResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection GetServicesCallback(bool includecapability, boost::function<void(const GetServicesResponse&)> callback);
  Connection GetStorageConfigurationCallback(const std::string& token, boost::function<void(const GetStorageConfigurationResponse&)> callback);
  Connection GetStorageConfigurationsCallback(boost::function<void(const GetStorageConfigurationsResponse&)> callback);
  Connection GetSystemBackupCallback(boost::function<void(const GetSystemBackupResponse&)> callback);
  Connection GetSystemDateAndTimeCallback(boost::function<void(const GetSystemDateAndTimeResponse&)> callback);
  Connection GetSystemLogCallback(SYSTEMLOGTYPE logtype, boost::function<void(const GetSystemLogResponse&)> callback);
  Connection GetSystemSupportInformationCallback(boost::function<void(const GetSystemSupportInformationResponse&)> callback);
  Connection GetSystemUrisCallback(boost::function<void(const GetSystemUrisResponse&)> callback);
  Connection GetUsersCallback(boost::function<void(const GetUsersResponse&)> callback);
  Connection GetWsdlUrlCallback(boost::function<void(const GetWsdlUrlResponse&)> callback);
  Connection GetZeroConfigurationCallback(boost::function<void(const GetZeroConfigurationResponse&)> callback);
  Connection LoadCACertificatesCallback(const std::vector<Certificate>& cacertificate, boost::function<void(const LoadCACertificatesResponse&)> callback);
  Connection LoadCertificatesCallback(const std::vector<Certificate>& nvtcertificate, boost::function<void(const LoadCertificatesResponse&)> callback);
  Connection LoadCertificateWithPrivateKeyCallback(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey, boost::function<void(const LoadCertificateWithPrivateKeyResponse&)> callback);
  Connection RemoveIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const RemoveIPAddressFilterResponse&)> callback);
  Connection RemoveScopesCallback(const std::vector<std::string>& scopes, boost::function<void(const RemoveScopesResponse&)> callback);
  Connection RestoreSystemCallback(const std::vector<BackupFile>& backupfiles, boost::function<void(const RestoreSystemResponse&)> callback);
  Connection ScanAvailableDot11NetworksCallback(const std::string& interfacetoken, boost::function<void(const ScanAvailableDot11NetworksResponse&)> callback);
  Connection SendAuxiliaryCommandCallback(const std::string& auxiliarycommand, boost::function<void(const SendAuxiliaryCommandResponse&)> callback);
  Connection SetAccessPolicyCallback(const BinaryData& setaccesspolicy, boost::function<void(const SetAccessPolicyResponse&)> callback);
  Connection SetCertificatesStatusCallback(const std::vector<CertificateStatus>& certificatestatus, boost::function<void(const SetCertificatesStatusResponse&)> callback);
  Connection SetClientCertificateModeCallback(bool enabled, boost::function<void(const SetClientCertificateModeResponse&)> callback);
  Connection SetDiscoveryModeCallback(DISCOVERYMODE discoverymode, boost::function<void(const SetDiscoveryModeResponse&)> callback);
  Connection SetDNSCallback(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual, boost::function<void(const SetDNSResponse&)> callback);
  Connection SetDot1XConfigurationCallback(const Dot1XConfiguration& dot1xconfiguration, boost::function<void(const SetDot1XConfigurationResponse&)> callback);
  Connection SetDPAddressesCallback(const std::vector<NetworkHost>& dpaddress, boost::function<void(const SetDPAddressesResponse&)> callback);
  Connection SetDynamicDNSCallback(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl, boost::function<void(const SetDynamicDNSResponse&)> callback);
  Connection SetHostnameCallback(const std::string& name, boost::function<void(const SetHostnameResponse&)> callback);
  Connection SetHostnameFromDHCPCallback(bool fromdhcp, boost::function<void(const SetHostnameFromDHCPResponse&)> callback);
  Connection SetIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const SetIPAddressFilterResponse&)> callback);
  Connection SetNetworkDefaultGatewayCallback(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address, boost::function<void(const SetNetworkDefaultGatewayResponse&)> callback);
  Connection SetNetworkInterfacesCallback(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface, boost::function<void(const SetNetworkInterfacesResponse&)> callback);
  Connection SetNetworkProtocolsCallback(const std::vector<NetworkProtocol>& networkprotocols, boost::function<void(const SetNetworkProtocolsResponse&)> callback);
  Connection SetNTPCallback(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual, boost::function<void(const SetNTPResponse&)> callback);
  Connection SetRelayOutputSettingsCallback(const std::string& relayoutputtoken, const RelayOutputSettings& properties, boost::function<void(const SetRelayOutputSettingsResponse&)> callback);
  Connection SetRemoteDiscoveryModeCallback(DISCOVERYMODE remotediscoverymode, boost::function<void(const SetRemoteDiscoveryModeResponse&)> callback);
  Connection SetSystemDateAndTimeCallback(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime, boost::function<void(const SetSystemDateAndTimeResponse&)> callback);
  Connection SetSystemFactoryDefaultCallback(FACTORYDEFAULTTYPE factorydefault, boost::function<void(const SetSystemFactoryDefaultResponse&)> callback);
  Connection SetUserCallback(const std::vector<User>& user, boost::function<void(const SetUserResponse&)> callback);
  Connection SetZeroConfigurationCallback(const std::string& interfacetoken, bool enabled, boost::function<void(const SetZeroConfigurationResponse&)> callback);
  Connection SystemRebootCallback(boost::function<void(const SystemRebootResponse&)> callback);

  // Future requests
  boost::unique_future<AddIPAddressFilterResponse> AddIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter);
  boost::unique_future<AddScopesResponse> AddScopesFuture(const std::vector<std::string>& scopes);
  boost::unique_future<CreateCertificateResponse> CreateCertificateFuture(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter);
  boost::unique_future<CreateStorageConfigurationResponse> CreateStorageConfigurationFuture(const StorageConfigurationData& storageconfiguration);
  boost::unique_future<CreateUsersResponse> CreateUsersFuture(const std::vector<User>& users);
  boost::unique_future<DeleteCertificatesResponse> DeleteCertificatesFuture(const std::vector<std::string>& certificateid);
  boost::unique_future<DeleteDot1XConfigurationResponse> DeleteDot1XConfigurationFuture(const std::vector<std::string>& dot1xconfigurationtoken);
  boost::unique_future<DeleteStorageConfigurationResponse> DeleteStorageConfigurationFuture(const std::vector<std::string>& token);
  boost::unique_future<DeleteUsersResponse> DeleteUsersFuture(const std::vector<std::string>& usernames);
  boost::unique_future<GetAccessPolicyResponse> GetAccessPolicyFuture();
  boost::unique_future<GetCACertificatesResponse> GetCACertificatesFuture();
  boost::unique_future<GetCapabilitiesResponse> GetCapabilitiesFuture(CAPABILITYCATEGORY capabilitycategory);
  boost::unique_future<GetCertificateInformationResponse> GetCertificateInformationFuture(const std::string& certificateid);
  boost::unique_future<GetCertificatesResponse> GetCertificatesFuture();
  boost::unique_future<GetCertificatesStatusResponse> GetCertificatesStatusFuture();
  boost::unique_future<GetClientCertificateModeResponse> GetClientCertificateModeFuture();
  boost::unique_future<GetDeviceInformationResponse> GetDeviceInformationFuture();
  boost::unique_future<GetDiscoveryModeResponse> GetDiscoveryModeFuture();
  boost::unique_future<GetDNSResponse> GetDNSFuture();
  boost::unique_future<GetDot11CapabilitiesResponse> GetDot11CapabilitiesFuture();
  boost::unique_future<GetDot11StatusResponse> GetDot11StatusFuture(const std::string& interfacetoken);
  boost::unique_future<GetDot1XConfigurationResponse> GetDot1XConfigurationFuture(const std::string& dot1xconfigurationtoken);
  boost::unique_future<GetDot1XConfigurationsResponse> GetDot1XConfigurationsFuture();
  boost::unique_future<GetDPAddressesResponse> GetDPAddressesFuture();
  boost::unique_future<GetDynamicDNSResponse> GetDynamicDNSFuture();
  boost::unique_future<GetEndpointReferenceResponse> GetEndpointReferenceFuture();
  boost::unique_future<GetHostnameResponse> GetHostnameFuture();
  boost::unique_future<GetIPAddressFilterResponse> GetIPAddressFilterFuture();
  boost::unique_future<GetNetworkDefaultGatewayResponse> GetNetworkDefaultGatewayFuture();
  boost::unique_future<GetNetworkInterfacesResponse> GetNetworkInterfacesFuture();
  boost::unique_future<GetNetworkProtocolsResponse> GetNetworkProtocolsFuture();
  boost::unique_future<GetPkcs10RequestResponse> GetPkcs10RequestFuture(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes);
  boost::unique_future<GetRelayOutputsResponse> GetRelayOutputsFuture();
  boost::unique_future<GetNTPResponse> GetNTPFuture();
  boost::unique_future<GetRemoteDiscoveryModeResponse> GetRemoteDiscoveryModeFuture();
  boost::unique_future<GetRemoteUserResponse> GetRemoteUserFuture();
  boost::unique_future<GetScopesResponse> GetScopesFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<GetServicesResponse> GetServicesFuture(bool includecapability);
  boost::unique_future<GetStorageConfigurationResponse> GetStorageConfigurationFuture(const std::string& token);
  boost::unique_future<GetStorageConfigurationsResponse> GetStorageConfigurationsFuture();
  boost::unique_future<GetSystemBackupResponse> GetSystemBackupFuture();
  boost::unique_future<GetSystemDateAndTimeResponse> GetSystemDateAndTimeFuture();
  boost::unique_future<GetSystemLogResponse> GetSystemLogFuture(SYSTEMLOGTYPE logtype);
  boost::unique_future<GetSystemSupportInformationResponse> GetSystemSupportInformationFuture();
  boost::unique_future<GetSystemUrisResponse> GetSystemUrisFuture();
  boost::unique_future<GetUsersResponse> GetUsersFuture();
  boost::unique_future<GetWsdlUrlResponse> GetWsdlUrlFuture();
  boost::unique_future<GetZeroConfigurationResponse> GetZeroConfigurationFuture();
  boost::unique_future<LoadCACertificatesResponse> LoadCACertificatesFuture(const std::vector<Certificate>& cacertificate);
  boost::unique_future<LoadCertificatesResponse> LoadCertificatesFuture(const std::vector<Certificate>& nvtcertificate);
  boost::unique_future<LoadCertificateWithPrivateKeyResponse> LoadCertificateWithPrivateKeyFuture(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey);
  boost::unique_future<RemoveIPAddressFilterResponse> RemoveIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter);
  boost::unique_future<RemoveScopesResponse> RemoveScopesFuture(const std::vector<std::string>& scopes);
  boost::unique_future<RestoreSystemResponse> RestoreSystemFuture(const std::vector<BackupFile>& backupfiles);
  boost::unique_future<ScanAvailableDot11NetworksResponse> ScanAvailableDot11NetworksFuture(const std::string& interfacetoken);
  boost::unique_future<SendAuxiliaryCommandResponse> SendAuxiliaryCommandFuture(const std::string& auxiliarycommand);
  boost::unique_future<SetAccessPolicyResponse> SetAccessPolicyFuture(const BinaryData& policyfile);
  boost::unique_future<SetCertificatesStatusResponse> SetCertificatesStatusFuture(const std::vector<CertificateStatus>& certificatestatus);
  boost::unique_future<SetClientCertificateModeResponse> SetClientCertificateModeFuture(bool enabled);
  boost::unique_future<SetDiscoveryModeResponse> SetDiscoveryModeFuture(DISCOVERYMODE discoverymode);
  boost::unique_future<SetDNSResponse> SetDNSFuture(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual);
  boost::unique_future<SetDot1XConfigurationResponse> SetDot1XConfigurationFuture(const Dot1XConfiguration& dot1xconfiguration);
  boost::unique_future<SetDPAddressesResponse> SetDPAddressesFuture(const std::vector<NetworkHost>& dpaddress);
  boost::unique_future<SetDynamicDNSResponse> SetDynamicDNSFuture(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl);
  boost::unique_future<SetHostnameResponse> SetHostnameFuture(const std::string& name);
  boost::unique_future<SetHostnameFromDHCPResponse> SetHostnameFromDHCPFuture(bool fromdhcp);
  boost::unique_future<SetIPAddressFilterResponse> SetIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter);
  boost::unique_future<SetNetworkDefaultGatewayResponse> SetNetworkDefaultGatewayFuture(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address);
  boost::unique_future<SetNetworkInterfacesResponse> SetNetworkInterfacesFuture(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface);
  boost::unique_future<SetNetworkProtocolsResponse> SetNetworkProtocolsFuture(const std::vector<NetworkProtocol>& networkprotocols);
  boost::unique_future<SetNTPResponse> SetNTPFuture(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual);
  boost::unique_future<SetRelayOutputSettingsResponse> SetRelayOutputSettingsFuture(const std::string& relayoutputtoken, const RelayOutputSettings& properties);
  boost::unique_future<SetRemoteDiscoveryModeResponse> SetRemoteDiscoveryModeFuture(DISCOVERYMODE remotediscoverymode);
  boost::unique_future<SetSystemDateAndTimeResponse> SetSystemDateAndTimeFuture(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime);
  boost::unique_future<SetSystemFactoryDefaultResponse> SetSystemFactoryDefaultFuture(FACTORYDEFAULTTYPE factorydefault);
  boost::unique_future<SetUserResponse> SetUserFuture(const std::vector<User>& users);
  boost::unique_future<SetZeroConfigurationResponse> SetZeroConfigurationFuture(const std::string& interfacetoken, bool enabled);
  boost::unique_future<SystemRebootResponse> SystemRebootFuture();
  
  // Boost Signals2
  boost::signals2::signal<void(const AddIPAddressFilterResponse&)>& AddIPAddressFilterSignal();
  boost::signals2::signal<void(const AddScopesResponse&)>& AddScopesSignal();
  boost::signals2::signal<void(const CreateCertificateResponse&)>& CreateCertificateSignal();
  boost::signals2::signal<void(const CreateUsersResponse&)>& GetCreateUsersSignal();
  boost::signals2::signal<void(const DeleteCertificatesResponse&)>& GetDeleteCertificatesSignal();
  boost::signals2::signal<void(const DeleteDot1XConfigurationResponse&)>& GetDeleteDot1XConfigurationSignal();
  boost::signals2::signal<void(const DeleteStorageConfigurationResponse&)>& GetDeleteStorageConfigurationSignal();
  boost::signals2::signal<void(const DeleteUsersResponse&)>& GetDeleteUsersSignal();
  boost::signals2::signal<void(const GetAccessPolicyResponse&)>& GetAccessPolicySignal();
  boost::signals2::signal<void(const GetCACertificatesResponse&)>& GetCACertificatesSignal();
  boost::signals2::signal<void(const GetCapabilitiesResponse&)>& GetCapabilitiesSignal();
  boost::signals2::signal<void(const GetCertificateInformationResponse&)>& GetCertificateInformationSignal();
  boost::signals2::signal<void(const GetCertificatesResponse&)>& GetCertificatesSignal();
  boost::signals2::signal<void(const GetCertificatesStatusResponse&)>& GetCertificatesStatusSignal();
  boost::signals2::signal<void(const GetClientCertificateModeResponse&)>& GetClientCertificateModeSignal();
  boost::signals2::signal<void(const GetDeviceInformationResponse&)>& GetDeviceInformationSignal();
  boost::signals2::signal<void(const GetDiscoveryModeResponse&)>& GetDiscoveryModeSignal();
  boost::signals2::signal<void(const GetDNSResponse&)>& GetDNSSignal();
  boost::signals2::signal<void(const GetDot11CapabilitiesResponse&)>& GetDot11CapabilitiesSignal();
  boost::signals2::signal<void(const GetDot11StatusResponse&)>& GetDot11StatusSignal();
  boost::signals2::signal<void(const GetDot1XConfigurationResponse&)>& GetDot1XConfigurationSignal();
  boost::signals2::signal<void(const GetDot1XConfigurationsResponse&)>& GetDot1XConfigurationsSignal();
  boost::signals2::signal<void(const GetDPAddressesResponse&)>& GetDPAddressesSignal();
  boost::signals2::signal<void(const GetDynamicDNSResponse&)>& GetDynamicDNSSignal();
  boost::signals2::signal<void(const GetEndpointReferenceResponse&)>& GetEndpointReferenceSignal();
  boost::signals2::signal<void(const GetHostnameResponse&)>& GetHsotnameSignal();
  boost::signals2::signal<void(const GetIPAddressFilterResponse&)>& GetIPAddressFilterSignal();
  boost::signals2::signal<void(const GetNetworkDefaultGatewayResponse&)>& GetNetworkDefaultGatewaySignal();
  boost::signals2::signal<void(const GetNetworkInterfacesResponse&)>& GetNetworkInterfacesSignal();
  boost::signals2::signal<void(const GetNetworkProtocolsResponse&)>& GetNetworkProtocolsSignal();
  boost::signals2::signal<void(const GetPkcs10RequestResponse&)>& GetPkcs10RequestSignal();
  boost::signals2::signal<void(const GetRelayOutputsResponse&)>& GetRelayOutputsSignal();
  boost::signals2::signal<void(const GetNTPResponse&)>& GetNTPSignal();
  boost::signals2::signal<void(const GetRemoteDiscoveryModeResponse&)>& GetRemoteDiscoveryModeSignal();
  boost::signals2::signal<void(const GetRemoteUserResponse&)>& GetRemoteUserSignal();
  boost::signals2::signal<void(const GetScopesResponse&)>& GetScopesSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const GetServicesResponse&)>& GetServicesSignal();
  boost::signals2::signal<void(const GetStorageConfigurationResponse&)>& GetStorageConfigurationSignal();
  boost::signals2::signal<void(const GetStorageConfigurationsResponse&)>& GetStorageConfigurationsSignal();
  boost::signals2::signal<void(const GetSystemBackupResponse&)>& GetSystemBackupSignal();
  boost::signals2::signal<void(const GetSystemDateAndTimeResponse&)>& GetSystemDateAndTimeSignal();
  boost::signals2::signal<void(const GetSystemLogResponse&)>& GetSystemLogSignal();
  boost::signals2::signal<void(const GetSystemSupportInformationResponse&)>& GetSystemSupportInformationSignal();
  boost::signals2::signal<void(const GetSystemUrisResponse&)>& GetSystemUrisSignal();
  boost::signals2::signal<void(const GetUsersResponse&)>& GetUsersSignal();
  boost::signals2::signal<void(const GetWsdlUrlResponse&)>& GetWsdlUrlSignal();
  boost::signals2::signal<void(const GetZeroConfigurationResponse&)>& GetZeroConfigurationSignal();
  boost::signals2::signal<void(const LoadCACertificatesResponse&)>& LoadCACertificatesSignal();
  boost::signals2::signal<void(const LoadCertificatesResponse&)>& LoadCertificatesSignal();
  boost::signals2::signal<void(const LoadCertificateWithPrivateKeyResponse&)>& LoadCertificateWithPrivateKeySignal();
  boost::signals2::signal<void(const RemoveIPAddressFilterResponse&)>& RemoveIPAddressFilterSignal();
  boost::signals2::signal<void(const RemoveScopesResponse&)>& RemoveScopesSignal();
  boost::signals2::signal<void(const RestoreSystemResponse&)>& RestoreSystemSignal();
  boost::signals2::signal<void(const ScanAvailableDot11NetworksResponse&)>& ScanAvailableDot11NetworksSignal();
  boost::signals2::signal<void(const SendAuxiliaryCommandResponse&)>& SendAuxiliaryCommandSignal();
  boost::signals2::signal<void(const SetAccessPolicyResponse&)>& SetAccessPolicySignal();
  boost::signals2::signal<void(const SetCertificatesStatusResponse&)>& SetCertificatesStatusSignal();
  boost::signals2::signal<void(const SetClientCertificateModeResponse&)>& SetClientCertificateModeSignal();
  boost::signals2::signal<void(const SetDiscoveryModeResponse&)>& SetDiscoveryModeSignal();
  boost::signals2::signal<void(const SetDNSResponse&)>& SetDNSSignal();
  boost::signals2::signal<void(const SetDot1XConfigurationResponse&)>& SetDot1XConfigurationSignal();
  boost::signals2::signal<void(const SetDPAddressesResponse&)>& SetDPAddressesSignal();
  boost::signals2::signal<void(const SetDynamicDNSResponse&)>& SetDynamicDNSSignal();
  boost::signals2::signal<void(const SetHostnameResponse&)>& SetHostnameSignal();
  boost::signals2::signal<void(const SetHostnameFromDHCPResponse&)>& SetHostnameFromDHCPSignal();
  boost::signals2::signal<void(const SetIPAddressFilterResponse&)>& SetIPAddressFilterSignal();
  boost::signals2::signal<void(const SetNetworkDefaultGatewayResponse&)>& SetNetworkDefaultGatewaySignal();
  boost::signals2::signal<void(const SetNetworkInterfacesResponse&)>& SetNetworkInterfacesSignal();
  boost::signals2::signal<void(const SetNetworkProtocolsResponse&)>& SetNetworkProtocolsSignal();
  boost::signals2::signal<void(const SetNTPResponse&)>& SetNTPSignal();
  boost::signals2::signal<void(const SetRelayOutputSettingsResponse&)>& SetRelayOutputSettingsSignal();
  boost::signals2::signal<void(const SetRemoteDiscoveryModeResponse&)>& SetRemoteDiscoveryModeSignal();
  boost::signals2::signal<void(const SetSystemDateAndTimeResponse&)>& SetSystemDateAndTimeSignal();
  boost::signals2::signal<void(const SetSystemFactoryDefaultResponse&)>& SetSystemFactoryDefaultSignal();
  boost::signals2::signal<void(const SetUserResponse&)>& SetUserSignal();
  boost::signals2::signal<void(const SetZeroConfigurationResponse&)>& SetZeroConfigurationSignal();
  boost::signals2::signal<void(const SystemRebootResponse&)>& SystemRebootSignal();
  
 private:

  virtual void Update(DEVICEOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(DEVICEOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;
  
  // Get the body of the request string
  std::string AddIPAddressFilterBody(const IPAddressFilter& ipaddressfilter);
  std::string AddScopesBody(const std::vector<std::string>& scopes);
  std::string CreateCertificateBody(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter);
  std::string CreateStorageConfigurationBody(const StorageConfigurationData& storageconfiguration);
  std::string CreateUsersBody(const std::vector<User>& users);
  std::string DeleteCertificatesBody(const std::vector<std::string>& certificateid);
  std::string DeleteDot1XConfigurationBody(const std::vector<std::string>& dot1xconfigurationtoken);
  std::string DeleteStorageConfigurationBody(const std::vector<std::string>& token);
  std::string DeleteUsersBody(const std::vector<std::string>& usernames);
  std::string GetAccessPolicyBody();
  std::string GetCACertificatesBody();
  std::string GetCapabilitiesBody(CAPABILITYCATEGORY capability);
  std::string GetCertificateInformationBody(const std::string& certificateid);
  std::string GetCertificatesBody();
  std::string GetCertificatesStatusBody();
  std::string GetClientCertificateModeBody();
  std::string GetDiscoveryModeBody();
  std::string GetDeviceInformationBody();
  std::string GetDNSBody();
  std::string GetDot11CapabilitiesBody();
  std::string GetDot11StatusBody(const std::string& interfacetoken);
  std::string GetDot1XConfigurationBody(const std::string& dot1xconfigurationtoken);
  std::string GetDot1XConfigurationsBody();
  std::string GetDPAddressesBody();
  std::string GetDynamicDNSBody();
  std::string GetEndpointReferenceBody();
  std::string GetHostnameBody();
  std::string GetIPAddressFilterBody();
  std::string GetNetworkDefaultGatewayBody();
  std::string GetNetworkInterfacesBody();
  std::string GetNetworkProtocolsBody();
  std::string GetPkcs10RequestBody(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes);
  std::string GetRelayOutputsBody();
  std::string GetNTPBody();
  std::string GetRemoteDiscoveryModeBody();
  std::string GetRemoteUserBody();
  std::string GetScopesBody();
  std::string GetServiceCapabilitiesBody();
  std::string GetServicesBody(bool includecapability);
  std::string GetStorageConfigurationBody(const std::string& token);
  std::string GetStorageConfigurationsBody();
  std::string GetSystemBackupBody();
  std::string GetSystemDateAndTimeBody();
  std::string GetSystemLogBody(SYSTEMLOGTYPE logtype);
  std::string GetSystemSupportInformationBody();
  std::string GetSystemUrisBody();
  std::string GetUsersBody();
  std::string GetWsdlUrlBody();
  std::string GetZeroConfigurationBody();
  std::string LoadCACertificatesBody(const std::vector<Certificate>& cacertificate);
  std::string LoadCertificatesBody(const std::vector<Certificate>& nvtcertificate);
  std::string LoadCertificateWithPrivateKeyBody(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey);
  std::string RemoveIPAddressFilterBody(const IPAddressFilter& ipaddressfilter);
  std::string RemoveScopesBody(const std::vector<std::string>& scopes);
  std::pair<std::string, std::map<std::string, std::vector<char> > > RestoreSystemBody(std::vector<BackupFile> scopes);
  std::string ScanAvailableDot11NetworksBody(const std::string& interfacetoken);
  std::string SendAuxiliaryCommandBody(const std::string& auxiliarycommand);
  std::string SetAccessPolicyBody(const BinaryData& policyfile);
  std::string SetCertificatesStatusBody(const std::vector<CertificateStatus>& certificatestatus);
  std::string SetClientCertificateModeBody(bool enabled);
  std::string SetDiscoveryModeBody(DISCOVERYMODE discoverymode);
  std::string SetDNSBody(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual);
  std::string SetDot1XConfigurationBody(const Dot1XConfiguration& dot1xconfiguration);
  std::string SetDPAddressesBody(const std::vector<NetworkHost>& ntpmanual);
  std::string SetDynamicDNSBody(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl);
  std::string SetHostnameBody(const std::string& name);
  std::string SetHostnameFromDHCPBody(bool fromdhcp);
  std::string SetIPAddressFilterBody(const IPAddressFilter& ipaddressfilter);
  std::string SetNetworkDefaultGatewayBody(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address);
  std::string SetNetworkInterfacesBody(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface);
  std::string SetNetworkProtocolsBody(const std::vector<NetworkProtocol>& networkprotocols);
  std::string SetNTPBody(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual);
  std::string SetRelayOutputSettingsBody(const std::string& relayoutputtoken, const RelayOutputSettings& properties);
  std::string SetRemoteDiscoveryModeBody(DISCOVERYMODE remotediscoverymode);
  std::string SetSystemDateAndTimeBody(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime);
  std::string SetSystemFactoryDefaultBody(FACTORYDEFAULTTYPE factorydefault);
  std::string SetUserBody(const std::vector<User>& users);
  std::string SetZeroConfigurationBody(const std::string& interfacetoken, bool enabled);
  std::string SystemRebootBody();

  DeviceSignals* signals_; // PIMPL style, required because Visual Studio refuses to compile, and this improve compilation performance

};

}

}

#endif
