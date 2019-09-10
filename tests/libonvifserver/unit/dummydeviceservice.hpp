// dummydeviceservice.hpp
//

#ifndef IDS4PBXMFBOIX156735WOWGW1H5NSONA60
#define IDS4PBXMFBOIX156735WOWGW1H5NSONA60

///// Includes /////

#include <onvifserver/deviceserviceinterface.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Classes /////

class DummyDeviceService : public device::DeviceServiceInterface
{
 public:

  DummyDeviceService();
  virtual ~DummyDeviceService();

  virtual const std::string& GetXAddr() const { return xaddr_; }

  void SetUsername(const std::string& username) { username_ = username; }
  const std::string& GetUsername() const { return username_; }

  void SetUserLevel(onvif::USERLEVEL userlevel) { userlevel_ = userlevel; }
  onvif::USERLEVEL GetUserLevel() const { return userlevel_; }

  void SetPassword(const std::string& password) { password_ = password; }
  const std::string& GetPassword() const { return password_; }

  virtual bool OnvifDigestAuthentication(const std::string& namespaceuri, const std::string& action, evhttp_cmd_type requesttype, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& nc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const override;


 protected:

  virtual bool IsOperationRestricted(onvif::USERLEVEL userlevel, const std::string& xmlns, const std::string& action) const override;
  virtual bool OnvifAuthenticate(const std::string& namespaceuri, const std::string& action, const std::string& username, const std::string& password, const std::string& nonce, const std::string& time) const override;

  virtual ServerResponse AddIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) override;
  virtual ServerResponse AddScopes(const std::vector<std::string>& scopes) override;
  virtual ServerResponse CreateCertificate(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter) override;
  virtual ServerResponse CreateDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration) override;
  virtual ServerResponse CreateStorageConfiguration(const onvif::device::StorageConfigurationData& storageconfiguration) override;
  virtual ServerResponse CreateUsers(const std::vector<onvif::User>& user) override;
  virtual ServerResponse DeleteCertificates(const std::vector<std::string>& certificateid) override;
  virtual ServerResponse DeleteDot1XConfiguration(const std::vector<std::string>& dot1xconfigurationtoken) override;
  virtual ServerResponse DeleteStorageConfiguration(const std::string& token) override;
  virtual ServerResponse DeleteUsers(const std::vector<std::string>& username) override;
  virtual ServerResponse GetAccessPolicy() override;
  virtual ServerResponse GetCACertificates() override;
  virtual ServerResponse GetCapabilities(const std::string& localaddress, const uint16_t port, const boost::optional<onvif::CAPABILITYCATEGORY>& category) override;
  virtual ServerResponse GetCertificateInformation() override;
  virtual ServerResponse GetCertificates() override;
  virtual ServerResponse GetCertificatesStatus() override;
  virtual ServerResponse GetClientCertificateMode() override;
  virtual ServerResponse GetDeviceInformation() override;
  virtual ServerResponse GetDiscoveryMode() override;
  virtual ServerResponse GetDNS() override;
  virtual ServerResponse GetDot11Capabilities() override;
  virtual ServerResponse GetDot11Status(const std::string& interfacetoken) override;
  virtual ServerResponse GetDot1XConfiguration(const std::string& dot1xconfigurationtoken) override;
  virtual ServerResponse GetDot1XConfigurations() override;
  virtual ServerResponse GetDPAddresses() override;
  virtual ServerResponse GetDynamicDNS() override;
  virtual ServerResponse GetEndpointReference() override;
  virtual ServerResponse GetHostname() override;
  virtual ServerResponse GetIPAddressFilter() override;
  virtual ServerResponse GetNetworkDefaultGateway() override;
  virtual ServerResponse GetNetworkInterfaces() override;
  virtual ServerResponse GetNetworkProtocols() override;
  virtual ServerResponse GetNTP() override;
  virtual ServerResponse GetPkcs10Request(const std::string& certificateid, boost::optional<std::string>& subject, const boost::optional<onvif::BinaryData>& attributes) override;
  virtual ServerResponse GetRelayOutputs() override;
  virtual ServerResponse GetRemoteDiscoveryMode() override;
  virtual ServerResponse GetRemoteUser() override;
  virtual ServerResponse GetScopes() override;
  virtual ServerResponse GetServiceCapabilities() override;
  virtual ServerResponse GetServices(const std::string& localaddress, const uint16_t port, bool includecapability) override;
  virtual ServerResponse GetStorageConfiguration(const std::string& token) override;
  virtual ServerResponse GetStorageConfigurations() override;
  virtual ServerResponse GetSystemBackup() override;
  virtual ServerResponse GetSystemDateAndTime() override;
  virtual ServerResponse GetSystemLog(onvif::SYSTEMLOGTYPE logtype) override;
  virtual ServerResponse GetSystemSupportInformation() override;
  virtual ServerResponse GetSystemUris() override;
  virtual ServerResponse GetUsers() override;
  virtual ServerResponse GetWsdlUrl(const std::string& localaddress, const uint16_t port) override;
  virtual ServerResponse GetZeroConfiguration();
  virtual ServerResponse LoadCACertificates(const std::vector<onvif::Certificate>& cacertificate) override;
  virtual ServerResponse LoadCertificates(const std::vector<onvif::Certificate>& nvtcertificate) override;
  virtual ServerResponse LoadCertificateWithPrivateKey(const std::vector<onvif::CertificateWithPrivateKey>& certificatewithprivatekey) override;
  virtual ServerResponse RemoveIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) override;
  virtual ServerResponse RemoveScopes(const std::vector<std::string>& scopeitem) override;
  virtual ServerResponse RestoreSystem(const std::vector<onvif::BackupFile>& backupfiles) override;
  virtual ServerResponse ScanAvailableDot11Networks(const std::string& interfacetoken) override;
  virtual ServerResponse SendAuxiliaryCommand(const std::string& auxiliarycommand) override;
  virtual ServerResponse SetAccessPolicy(const onvif::BinaryData& policyfile) override;
  virtual ServerResponse SetCertificatesStatus(const std::vector<onvif::CertificateStatus>& certificatestatus) override;
  virtual ServerResponse SetClientCertificateMode(bool enabled) override;
  virtual ServerResponse SetDiscoveryMode(onvif::DISCOVERYMODE discoverymode) override;
  virtual ServerResponse SetDNS(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<onvif::IPAddress>& dnsmanual) override;
  virtual ServerResponse SetDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration) override;
  virtual ServerResponse SetDPAddresses(const std::vector<onvif::NetworkHost>& dpaddress) override;
  virtual ServerResponse SetDynamicDNS(onvif::DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<onvif::Duration>& ttl) override;
  virtual ServerResponse SetHostname(const std::string& name) override;
  virtual ServerResponse SetHostnameFromDHCP(bool fromdhcp) override;
  virtual ServerResponse SetIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter) override;
  virtual ServerResponse SetNetworkDefaultGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address) override;
  virtual ServerResponse SetNetworkInterfaces(const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface) override;
  virtual ServerResponse SetNetworkProtocols(const std::vector<NetworkProtocol>& networkprotocols) override;
  virtual ServerResponse SetNTP(bool fromdhcp, const std::vector<onvif::NetworkHost>& ntpmanual) override;
  virtual ServerResponse SetRelayOutputSettings(const std::string& relayoutputtoken, const onvif::RelayOutputSettings& properties) override;
  virtual ServerResponse SetRelayOutputState(const std::string& relayoutputtoken, onvif::RELAYLOGICALSTATE logicalstate) override;
  virtual ServerResponse SetRemoteUser(const boost::optional<onvif::RemoteUser>& remoteuser) override;
  virtual ServerResponse SetRemoteDiscoveryMode(onvif::DISCOVERYMODE RemoteDiscoveryMode) override;
  virtual ServerResponse SetScopes(const std::vector<std::string>& scopes) override;
  virtual ServerResponse SetSystemDateAndTime(onvif::DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<onvif::TimeZone>& timezone, const boost::optional<onvif::DateTime>& datetime) override;
  virtual ServerResponse SetSystemFactoryDefault(onvif::FACTORYDEFAULTTYPE factorydefault) override;
  virtual ServerResponse SetUser(const std::vector<onvif::User>& users) override;
  virtual ServerResponse SetZeroConfiguration(const std::string& interfacetoken, bool enabled) override;
  virtual ServerResponse SystemReboot() override;


 private:

  const std::string xaddr_;

  std::string username_;
  onvif::USERLEVEL userlevel_;
  std::string password_;

};

}

}

}

#endif
