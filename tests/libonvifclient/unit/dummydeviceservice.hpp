// dummydeviceservice.hpp
//

#ifndef IDWBRJ6T1P683I1BNJEH5NGB68CSKB4QKJ
#define IDWBRJ6T1P683I1BNJEH5NGB68CSKB4QKJ

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

class DummyDeviceService : public server::Service
{
 public:

  DummyDeviceService();
  virtual ~DummyDeviceService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  inline void SetCreateCertificate(const Certificate& createcertificate) { createcertificate_ = createcertificate; }
  inline void SetCreateStorageConfiguration(const std::string& createstorageconfiguration) { createstorageconfiguration_ = createstorageconfiguration; }
  inline void SetGetAccessPolicy(const BinaryData& getaccesspolicy) { getaccesspolicy_ = getaccesspolicy; }
  inline void SetGetCACertificates(const std::vector<Certificate>& getcacertificates) { getcacertificates_ = getcacertificates; }
  inline void SetGetCapabilities(const Capabilities& getcapabilities) { getcapabilities_ = getcapabilities; }
  inline void SetGetCertificateInformation(const CertificateInformation& getcertificateinformation) { getcertificateinformation_ = getcertificateinformation; }
  inline void SetGetCertificates(const std::vector<Certificate>& getcertificates) { getcertificates_ = getcertificates; }
  inline void SetGetCertificatesStatus(const std::vector<CertificateStatus>& getcertificatesstatus) { getcertificatesstatus_ = getcertificatesstatus; }
  inline void SetGetClientCertificateMode(bool getclientcertificatemode) { getclientcertificatemode_ = getclientcertificatemode; }
  inline void SetGetDeviceInformation(const std::string& manufacturer, const std::string& model, const std::string& firmwareversion, const std::string& serialnumber, const std::string& hardwareid) { manufacturer_ = manufacturer; model_ = model; firmwareversion_ = firmwareversion; serialnumber_ = serialnumber; hardwareid_ = hardwareid; }
  inline void SetGetDiscoveryMode(DISCOVERYMODE getdiscoverymode) { getdiscoverymode_ = getdiscoverymode; }
  inline void SetGetDNS(const DNSInformation& getdns) { getdns_ = getdns; }
  inline void SetGetDot11Capabilities(const Dot11Capabilities& getdot11capabilities) { getdot11capabilities_ = getdot11capabilities; }
  inline void SetGetDot11Status(const Dot11Status& getdot11status) { getdot11status_ = getdot11status; }
  inline void SetGetDot1XConfiguration(const Dot1XConfiguration& getdot1xconfiguration) { getdot1xconfiguration_ = getdot1xconfiguration; }
  inline void SetGetDot1XConfigurations(const std::vector<Dot1XConfiguration>& getdot1xconfigurations) { getdot1xconfigurations_ = getdot1xconfigurations; }
  inline void SetGetDPAddresses(const std::vector<NetworkHost>& getdpaddresses) { getdpaddresses_ = getdpaddresses; }
  inline void SetGetDynamicDNS(const DynamicDNSInformation& getdynamicdns) { getdynamicdns_ = getdynamicdns; }
  inline void SetGetEndpointReference(const std::string& getendpointreference) { getendpointreference_ = getendpointreference; }
  inline void SetGetHostname(const HostnameInformation& gethostname) { gethostname_ = gethostname; }
  inline void SetGetIPAddressFilter(const IPAddressFilter& getipaddressfilter) { getipaddressfilter_ = getipaddressfilter; }
  inline void SetGetNetworkDefaultGateway(const NetworkGateway& getnetworkdefaultgateway) { getnetworkdefaultgateway_ = getnetworkdefaultgateway; }
  inline void SetSetNetworkInterfaces(bool setnetworkinterfaces) { setnetworkinterfaces_ = setnetworkinterfaces; }
  inline void SetGetNetworkInterfaces(const std::vector<NetworkInterface>& getnetworkinterfaces) { getnetworkinterfaces_ = getnetworkinterfaces; }
  inline void SetGetNetworkProtocols(const std::vector<NetworkProtocol>& getnetworkprotocols) { getnetworkprotocols_ = getnetworkprotocols; }
  inline void SetGetNTP(const NTPInformation& getntp) { getntp_ = getntp; }
  inline void SetGetPkcs10Request(const BinaryData& getpkcs10request) { getpkcs10request_ = getpkcs10request; }
  inline void SetGetRelayOutputs(const std::vector<RelayOutput>& getrelayoutputs) { getrelayoutputs_ = getrelayoutputs;  }
  inline void SetGetRemoteDiscoveryMode(DISCOVERYMODE getremotediscoverymode) { getremotediscoverymode_ = getremotediscoverymode; }
  inline void SetGetRemoteUser(const RemoteUser& getremoteuser) { getremoteuser_ = getremoteuser; }
  inline void SetGetScopes(const std::vector<Scope>& getscopes) { getscopes_ = getscopes; }
  inline void SetGetServiceCapabilities(const device::DeviceServiceCapabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }
  inline void SetGetServices(const std::vector<device::Service>& getservices) { getservices_ = getservices; }
  inline void SetGetStorageConfiguration(const device::StorageConfiguration& getstorageconfiguration) { getstorageconfiguration_ = getstorageconfiguration; }
  inline void SetGetStorageConfigurations(const std::vector<device::StorageConfiguration>& getstorageconfigurations) { getstorageconfigurations_ = getstorageconfigurations; }
  inline void SetGetSystemBackup(const std::vector<BackupFile>& getsystembackup) { getsystembackup_ = getsystembackup; }
  inline void SetGetSystemDateTime(const SystemDateTime& getsystemdatetime) { getsystemdatetime_ = getsystemdatetime; }
  inline void SetGetSystemLog(const SystemLog& getsystemlog) { getsystemlog_ = getsystemlog; }
  inline void SetGetSystemSupportInformation(const SupportInformation& getsystemsupportinformation) { getsystemsupportinformation_ = getsystemsupportinformation; }
  inline void SetGetSystemUris(const SystemLogUriList& systemloguris, const std::string& supportinfouri, const std::string& systembackupuri) { systemloguris_ = systemloguris; supportinfouri_ = supportinfouri; systembackupuri_ = systembackupuri; }
  inline void SetSystemReboot(const std::string& getsystemreboot) { getsystemreboot_ = getsystemreboot; }
  inline void SetGetUsers(const std::vector<User>& getusers) { getusers_ = getusers; }
  inline void SetGetWsdlUrl(const std::string& getwsdlurl) { getwsdlurl_ = getwsdlurl; }
  inline void SetGetZeroConfiguration(const NetworkZeroConfiguration& getzeroconfiguration) { getzeroconfiguration_ = getzeroconfiguration; }
  inline void SetScanAvailableDot11Networks(const std::vector<Dot11AvailableNetworks>& scanavailabledot11networks) { scanavailabledot11networks_ = scanavailabledot11networks; }
  inline void SetSendAuxiliaryCommand(const std::string& auxiliarycommandresponse) { auxiliarycommandresponse_ = auxiliarycommandresponse; }


 private:

  std::string ToXml(const std::string& name, const std::string& text);
  
  Certificate createcertificate_;
  std::string createstorageconfiguration_;
  BinaryData getaccesspolicy_;
  Capabilities getcapabilities_;
  std::vector<Certificate> getcacertificates_;
  CertificateInformation getcertificateinformation_;
  std::vector<Certificate> getcertificates_;
  std::vector<CertificateStatus> getcertificatesstatus_;
  std::string manufacturer_; std::string model_; std::string firmwareversion_; std::string serialnumber_; std::string hardwareid_;
  bool getclientcertificatemode_;
  DISCOVERYMODE getdiscoverymode_;
  DNSInformation getdns_;
  Dot11Capabilities getdot11capabilities_;
  Dot11Status getdot11status_;
  Dot1XConfiguration getdot1xconfiguration_;
  std::vector<Dot1XConfiguration> getdot1xconfigurations_;
  std::vector<NetworkHost> getdpaddresses_;
  DynamicDNSInformation getdynamicdns_;
  std::string getendpointreference_;
  HostnameInformation gethostname_;
  IPAddressFilter getipaddressfilter_;
  NetworkGateway getnetworkdefaultgateway_;
  std::vector<NetworkInterface> getnetworkinterfaces_;
  std::vector<NetworkProtocol> getnetworkprotocols_;
  NTPInformation getntp_;
  BinaryData getpkcs10request_;
  std::vector<RelayOutput> getrelayoutputs_;
  DISCOVERYMODE getremotediscoverymode_;
  RemoteUser getremoteuser_;
  std::vector<Scope> getscopes_;
  device::DeviceServiceCapabilities getservicecapabilities_;
  std::vector<device::Service> getservices_;
  device::StorageConfiguration getstorageconfiguration_;
  std::vector<device::StorageConfiguration> getstorageconfigurations_;
  std::vector<BackupFile> getsystembackup_;
  SystemDateTime getsystemdatetime_;
  SystemLog getsystemlog_;
  SupportInformation getsystemsupportinformation_;
  SystemLogUriList systemloguris_; std::string supportinfouri_; std::string systembackupuri_;
  std::string getsystemreboot_;
  std::vector<User> getusers_;
  std::string getwsdlurl_;
  NetworkZeroConfiguration getzeroconfiguration_;
  std::vector<Dot11AvailableNetworks> scanavailabledot11networks_;
  std::string auxiliarycommandresponse_;
  bool setnetworkinterfaces_;

};

}

}

#endif
