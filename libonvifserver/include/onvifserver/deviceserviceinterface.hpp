// deviceserviceinterface.hpp
//

#ifndef ID0U5LUSM7ILNQAT91K5DSSEPM6GV5LRST
#define ID0U5LUSM7ILNQAT91K5DSSEPM6GV5LRST

///// Includes /////

#include <memory>
#include <string>

#include "onvifservice.hpp"

#ifdef _WIN32
  #include <Iphlpapi.h>
  #include <windows.h>
#endif

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace device
{

///// Globals /////

extern const std::string DEVICENAMESPACEURI;

extern const std::string ADDIPADDRESSFILTER;
extern const std::string ADDSCOPES;
extern const std::string CREATECERTIFICATE;
extern const std::string CREATEDOT1XCONFIGURATION;
extern const std::string CREATESTORAGECONFIGURATION;
extern const std::string CREATEUSERS;
extern const std::string DELETECERTIFICATES;
extern const std::string DELETEDOT1XCONFIGURATION;
extern const std::string DELETEGEOLOCATION;
extern const std::string DELETESTORAGECONFIGURATION;
extern const std::string DELETEUSERS;
extern const std::string GETACCESSPOLICY;
extern const std::string GETCACERTIFICATES;
extern const std::string GETCAPABILITIES;
extern const std::string GETCERTIFICATEINFORMATION;
extern const std::string GETCERTIFICATES;
extern const std::string GETCERTIFICATESSTATUS;
extern const std::string GETCLIENTCERTIFICATEMODE;
extern const std::string GETDEVICEINFORMATION;
extern const std::string GETDISCOVERYMODE;
extern const std::string GETDNS;
extern const std::string GETDOT11CAPABILITIES;
extern const std::string GETDOT11STATUS;
extern const std::string GETDOT1XCONFIGURATION;
extern const std::string GETDOT1XCONFIGURATIONS;
extern const std::string GETDPADDRESSES;
extern const std::string GETDYNAMICDNS;
extern const std::string GETENDPOINTREFERENCE;
extern const std::string GETGEOLOCATION;
extern const std::string GETHOSTNAME;
extern const std::string GETIPADDRESSFILTER;
extern const std::string GETNETWORKDEFAULTGATEWAY;
extern const std::string GETNETWORKINTERFACES;
extern const std::string GETNETWORKPROTOCOLS;
extern const std::string GETNTP;
extern const std::string GETPKCS10REQUEST;
extern const std::string GETRELAYOUTPUTS;
extern const std::string GETREMOTEDISCOVERYMODE;
extern const std::string GETREMOTEUSER;
extern const std::string GETSCOPES;
extern const std::string GETSERVICECAPABILITIES;
extern const std::string GETSERVICES;
extern const std::string GETSTORAGECONFIGURATION;
extern const std::string GETSTORAGECONFIGURATIONS;
extern const std::string GETSYSTEMBACKUP;
extern const std::string GETSYSTEMDATEANDTIME;
extern const std::string GETSYSTEMLOG;
extern const std::string GETSYSTEMSUPPORTINFORMATION;
extern const std::string GETSYSTEMURIS;
extern const std::string GETUSERS;
extern const std::string GETWSDLURL;
extern const std::string GETZEROCONFIGURATION;
extern const std::string LOADCACERTIFICATES;
extern const std::string LOADCERTIFICATES;
extern const std::string LOADCERTIFICATEWITHPRIVATEKEY;
extern const std::string REMOVEIPADDRESSFILTER;
extern const std::string REMOVESCOPES;
extern const std::string RESTORESYSTEM;
extern const std::string SCANAVAILABLEDOT11NETWORKS;
extern const std::string SENDAUXILIARYCOMMAND;
extern const std::string SETACCESSPOLICY;
extern const std::string SETCERTIFICATESSTATUS;
extern const std::string SETCLIENTCERTIFICATEMODE;
extern const std::string SETDISCOVERYMODE;
extern const std::string SETDNS;
extern const std::string SETDOT1XCONFIGURATION;
extern const std::string SETDPADDRESSES;
extern const std::string SETDYNAMICDNS;
extern const std::string SETGEOLOCATION;
extern const std::string SETHOSTNAME;
extern const std::string SETHOSTNAMEFROMDHCP;
extern const std::string SETIPADDRESSFILTER;
extern const std::string SETNETWORKDEFAULTGATEWAY;
extern const std::string SETNETWORKINTERFACES;
extern const std::string SETNETWORKPROTOCOLS;
extern const std::string SETNTP;
extern const std::string SETRELAYOUTPUTSETTINGS;
extern const std::string SETRELAYOUTPUTSTATE;
extern const std::string SETREMOTEDISCOVERYMODE;
extern const std::string SETREMOTEUSER;
extern const std::string SETSCOPES;
extern const std::string SETSTORAGECONFIGURATION;
extern const std::string SETSYSTEMDATEANDTIME;
extern const std::string SETSYSTEMFACTORYDEFAULT;
extern const std::string SETUSER;
extern const std::string SETZEROCONFIGURATION;
extern const std::string STARTFIRMWAREUPGRADE;
extern const std::string STARTSYSTEMRESTORE;
extern const std::string SYSTEMREBOOT;
extern const std::string UPGRADESYSTEMFIRMWARE;

///// Classes ////

class DeviceServiceInterface : public OnvifService
{
 public:

  DeviceServiceInterface(const int onvifauthenticationtimediff);
  virtual ~DeviceServiceInterface();

  int Init();
  void Destroy();

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

 protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse AddIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) = 0;
  virtual ServerResponse AddScopes(const std::vector<std::string>& scopes) = 0;
  virtual ServerResponse CreateCertificate(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter) = 0;
  virtual ServerResponse CreateDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration) = 0;
  virtual ServerResponse CreateStorageConfiguration(const onvif::device::StorageConfigurationData& storageconfiguration) = 0;
  virtual ServerResponse CreateUsers(const std::vector<onvif::User>& user) = 0;
  virtual ServerResponse DeleteCertificates(const std::vector<std::string>& certificateid) = 0;
  virtual ServerResponse DeleteDot1XConfiguration(const std::vector<std::string>& dot1xconfigurationtoken) = 0;
  virtual ServerResponse DeleteStorageConfiguration(const std::string& token) = 0;
  virtual ServerResponse DeleteUsers(const std::vector<std::string>& username) = 0;
  virtual ServerResponse GetAccessPolicy() = 0;
  virtual ServerResponse GetCACertificates() = 0;
  virtual ServerResponse GetCapabilities(const std::string& localaddress, const uint16_t port, const boost::optional<onvif::CAPABILITYCATEGORY>& category) = 0;
  virtual ServerResponse GetCertificateInformation() = 0;
  virtual ServerResponse GetCertificates() = 0;
  virtual ServerResponse GetCertificatesStatus() = 0;
  virtual ServerResponse GetClientCertificateMode() = 0;
  virtual ServerResponse GetDeviceInformation() = 0;
  virtual ServerResponse GetDiscoveryMode() = 0;
  virtual ServerResponse GetDNS() = 0;
  virtual ServerResponse GetDot11Capabilities() = 0;
  virtual ServerResponse GetDot11Status(const std::string& interfacetoken) = 0;
  virtual ServerResponse GetDot1XConfiguration(const std::string& dot1xconfigurationtoken) = 0;
  virtual ServerResponse GetDot1XConfigurations() = 0;
  virtual ServerResponse GetDPAddresses() = 0;
  virtual ServerResponse GetDynamicDNS() = 0;
  virtual ServerResponse GetEndpointReference() = 0;
  virtual ServerResponse GetHostname() = 0;
  virtual ServerResponse GetIPAddressFilter() = 0;
  virtual ServerResponse GetNetworkDefaultGateway() = 0;
  virtual ServerResponse GetNetworkInterfaces() = 0;
  virtual ServerResponse GetNetworkProtocols() = 0;
  virtual ServerResponse GetNTP() = 0;
  virtual ServerResponse GetPkcs10Request(const std::string& certificateid, boost::optional<std::string>& subject, const boost::optional<onvif::BinaryData>& attributes) = 0;
  virtual ServerResponse GetRelayOutputs() = 0;
  virtual ServerResponse GetRemoteDiscoveryMode() = 0;
  virtual ServerResponse GetRemoteUser() = 0;
  virtual ServerResponse GetScopes() = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;
  virtual ServerResponse GetServices(const std::string& localaddress, const uint16_t port, const bool includecapability) = 0;
  virtual ServerResponse GetStorageConfiguration(const std::string& token) = 0;
  virtual ServerResponse GetStorageConfigurations() = 0;
  virtual ServerResponse GetSystemBackup() = 0;
  virtual ServerResponse GetSystemDateAndTime() = 0;
  virtual ServerResponse GetSystemLog(onvif::SYSTEMLOGTYPE logtype) = 0;
  virtual ServerResponse GetSystemSupportInformation() = 0;
  virtual ServerResponse GetSystemUris() = 0;
  virtual ServerResponse GetUsers() = 0;
  virtual ServerResponse GetWsdlUrl(const std::string& localaddress, const uint16_t port) = 0;
  virtual ServerResponse GetZeroConfiguration() = 0;
  virtual ServerResponse LoadCACertificates(const std::vector<onvif::Certificate>& cacertificate) = 0;
  virtual ServerResponse LoadCertificates(const std::vector<onvif::Certificate>& nvtcertificate) = 0;
  virtual ServerResponse LoadCertificateWithPrivateKey(const std::vector<onvif::CertificateWithPrivateKey>& certificatewithprivatekey) = 0;
  virtual ServerResponse RemoveIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) = 0;
  virtual ServerResponse RemoveScopes(const std::vector<std::string>& scopeitem) = 0;
  virtual ServerResponse RestoreSystem(const std::vector<onvif::BackupFile>& backupfiles) = 0;
  virtual ServerResponse ScanAvailableDot11Networks(const std::string& interfacetoken) = 0;
  virtual ServerResponse SendAuxiliaryCommand(const std::string& auxiliarycommand) = 0;
  virtual ServerResponse SetAccessPolicy(const onvif::BinaryData& policyfile) = 0;
  virtual ServerResponse SetCertificatesStatus(const std::vector<onvif::CertificateStatus>& certificatestatus) = 0;
  virtual ServerResponse SetClientCertificateMode(bool enabled) = 0;
  virtual ServerResponse SetDiscoveryMode(onvif::DISCOVERYMODE discoverymode) = 0;
  virtual ServerResponse SetDNS(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<onvif::IPAddress>& dnsmanual) = 0;
  virtual ServerResponse SetDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration) = 0;
  virtual ServerResponse SetDPAddresses(const std::vector<onvif::NetworkHost>& dpaddress) = 0;
  virtual ServerResponse SetDynamicDNS(onvif::DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<onvif::Duration>& ttl) = 0;
  virtual ServerResponse SetHostname(const std::string& name) = 0;
  virtual ServerResponse SetHostnameFromDHCP(bool fromdhcp) = 0;
  virtual ServerResponse SetIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) = 0;
  virtual ServerResponse SetNetworkDefaultGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address) = 0;
  virtual ServerResponse SetNetworkInterfaces(const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface) = 0;
  virtual ServerResponse SetNetworkProtocols(const std::vector<NetworkProtocol>& networkprotocols) = 0;
  virtual ServerResponse SetNTP(bool fromdhcp, const std::vector<onvif::NetworkHost>& ntpmanual) = 0;
  virtual ServerResponse SetRelayOutputSettings(const std::string& relayoutputtoken, const onvif::RelayOutputSettings& properties) = 0;
  virtual ServerResponse SetRelayOutputState(const std::string& relayoutputtoken, onvif::RELAYLOGICALSTATE logicalstate) = 0;
  virtual ServerResponse SetRemoteDiscoveryMode(onvif::DISCOVERYMODE RemoteDiscoveryMode) = 0;
  virtual ServerResponse SetRemoteUser(const boost::optional<onvif::RemoteUser>& remoteuser) = 0;
  virtual ServerResponse SetScopes(const std::vector<std::string>& scopes) = 0;
  virtual ServerResponse SetSystemDateAndTime(onvif::DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<onvif::TimeZone>& timezone, const boost::optional<onvif::DateTime>& datetime) = 0;
  virtual ServerResponse SetSystemFactoryDefault(onvif::FACTORYDEFAULTTYPE factorydefault) = 0;
  virtual ServerResponse SetUser(const std::vector<onvif::User>& users) = 0;
  virtual ServerResponse SetZeroConfiguration(const std::string& interfacetoken, bool enabled) = 0;
  virtual ServerResponse SystemReboot() = 0;

#ifdef _WIN32
  std::unique_ptr<char[]> GetAdaptersAddresses() const;
  std::unique_ptr<char[]> GetAdaptersInfo() const;
  const IP_ADAPTER_ADDRESSES* GetAdapterAddresses(const std::string& adaptername, const IP_ADAPTER_ADDRESSES* adaptersaddresses) const;
#endif

  static const std::string XADDR_;

 private:

  bool cominitialised_;

};

}

}

}

#endif
