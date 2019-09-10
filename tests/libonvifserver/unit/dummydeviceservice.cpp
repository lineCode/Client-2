// dummydeviceservice.cpp
//

///// Includes /////

#include "dummydeviceservice.hpp"

#include <chrono>
#include <onviftypes/onviftypes.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Methods /////

DummyDeviceService::DummyDeviceService() :
  device::DeviceServiceInterface(5),
  xaddr_("/device")
{

}

DummyDeviceService::~DummyDeviceService()
{

}

bool DummyDeviceService::OnvifDigestAuthentication(const std::string& namespaceuri, const std::string& action, evhttp_cmd_type requesttype, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const
{
  if (username_ != clientusername)
  {

    return false;
  }

  if (IsOperationRestricted(userlevel_, namespaceuri, action))
  {

    return false;
  }

  if (DigestResponse(utility::Md5(clientusername + std::string(":") + clientrealm + std::string(":") + password_), requesttype, clienturi, clientnonce, clientnc, clientcnonce, clientqop) != clientresponse)
  {

    return false;
  }

  return true;
}

bool DummyDeviceService::IsOperationRestricted(onvif::USERLEVEL userlevel, const std::string& xmlns, const std::string& action) const
{

  return false;
}

bool DummyDeviceService::OnvifAuthenticate(const std::string& namespaceuri, const std::string& action, const std::string& username, const std::string& password, const std::string& nonce, const std::string& time) const
{
  if (username != username_)
  {

    return false;
  }

  if (IsOperationRestricted(userlevel_, namespaceuri, action))
  {

    return false;
  }

  return OnvifService::OnvifAuthenticate(password_, password, nonce, time);
}

ServerResponse DummyDeviceService::AddIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:AddIPAddressFilterResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::AddScopes(const std::vector<std::string>& scopes)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:AddScopesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::CreateCertificate(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter)
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:CreateCertificateResponse"), std::string(), onvif::Certificate(std::string("certificateid"), onvif::BinaryData(std::vector<unsigned char>(), boost::none)).ToXml(std::string("tt:NvtCertificate")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::CreateDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:CreateDot1XConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::CreateStorageConfiguration(const onvif::device::StorageConfigurationData& storageconfiguration)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:CreateStorageConfigurationResponse"), std::string(), std::string("<tt:Token>Token</tt:Token>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::CreateUsers(const std::vector<onvif::User>& user)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:CreateUsersResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::DeleteCertificates(const std::vector<std::string>& certificateid)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:DeleteCertificatesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::DeleteDot1XConfiguration(const std::vector<std::string>& dot1xconfigurationtoken)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:DeleteDot1XConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::DeleteStorageConfiguration(const std::string& token)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:DeleteStorageConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::DeleteUsers(const std::vector<std::string>& username)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:DeleteUsersResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetAccessPolicy()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetAccessPolicyResponse"), std::string(), onvif::BinaryData(std::vector<unsigned char>(), boost::none).ToXml(std::string("tt:PolicyFile")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetCACertificates()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetCACertificatesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetCapabilities(const std::string& localaddress, const uint16_t port, const boost::optional<onvif::CAPABILITYCATEGORY>& category)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetCapabilitiesResponse"), std::string(), onvif::Capabilities(AnalyticsCapabilities(std::string("xaddr"), true, true), DeviceCapabilities(std::string("xaddr"), NetworkCapabilities(true, true, true, true, true), SystemCapabilities(true, true, true, true, true, true, { onvif::OnvifVersion(5, 6), onvif::OnvifVersion(7, 8) }, true, true, true, true), IOCapabilities(5, 5), SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, true)), EventCapabilities(std::string("xaddr"), true, true, true), ImagingCapabilities(std::string("xaddr")), MediaCapabilities(std::string("xaddr"), RealTimeStreamingCapabilities(true, true, true), ProfileCapabilities(5)), PTZCapabilities(std::string("xaddr")), DeviceIOCapabilities(std::string("xaddr"), 5, 5, 5, 5, 5), DisplayCapabilities(std::string("xaddr"), true), RecordingCapabilities(std::string("xaddr"), true, true, true, true, 5), SearchCapabilities(std::string("xaddr"), true), ReplayCapabilities(std::string("xaddr")), ReceiverCapabilities(std::string("xaddr"), true, true, true, 5, 5), AnalyticsDeviceCapabilities(std::string("xaddr"), true)).ToXml(std::string("tt:Capabilities")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetCertificateInformation()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetCertificateInformationResponse"), std::string(), onvif::CertificateInformation(std::string("certificateid"), std::string("issuerdn"), std::string("subjectdn"), CertificateUsage(std::string("usage1"), true), CertificateUsage(std::string("usage2"), true), 5, std::string("version"), std::string("serialnum"), std::string("signaturealgorithm"), DateTimeRange(ws::DateTime(std::string("2002-05-30T09:00:00")), ws::DateTime(std::string("2002-05-30T09:00:01")))).ToXml(std::string("tt:CertificateInformation")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetCertificates()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetCertificatesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetCertificatesStatus()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetCertificatesStatusResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetClientCertificateMode()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetClientCertificateModeResponse"), std::string(), std::string("<tt:Enabled>false</tt:Enabled>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDeviceInformation()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDeviceInformationResponse"), std::string(), std::string("<tt:Manufacturer>Manufacturer</tt:Manufacturer><tt:Model>Model</tt:Model><tt:FirmwareVersion>FirmwareVersion</tt:FirmwareVersion><tt:SerialNumber>SerialNumber</tt:SerialNumber><tt:HardwareId>HardwareId</tt:HardwareId>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDiscoveryMode()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDiscoveryModeResponse"), std::string(), std::string("<tt:DiscoveryMode>NonDiscoverable</tt:DiscoveryMode>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDNS()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDNSResponse"), std::string(), onvif::DNSInformation(true, std::vector<std::string>(), std::vector<onvif::IPAddress>(), std::vector<onvif::IPAddress>()).ToXml(std::string("tt:DNSInformation")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDot11Capabilities()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDot11CapabilitiesResponse"), std::string(), onvif::Dot11Capabilities(true, true, true, true, true).ToXml(std::string("tt:Capabilities")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDot11Status(const std::string& interfacetoken)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDot11StatusResponse"), std::string(), onvif::Dot11Status(std::string("ssid"), std::string("bssid"), DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD, std::string("activeconfigalias")).ToXml(std::string("tt:Status")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDot1XConfiguration(const std::string& dot1xconfigurationtoken)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDot1XConfigurationResponse"), std::string(), onvif::Dot1XConfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password"))).ToXml(std::string("tt:Dot1XConfigurationToken")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDot1XConfigurations()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDot1XConfigurationsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDPAddresses()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDPAddressesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetDynamicDNS()
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetDynamicDNSResponse"), std::string(), onvif::DynamicDNSInformation(onvif::DYNAMICDNSTYPE_CLIENTUPDATES, boost::none, boost::none).ToXml(std::string("tt:DynamicDNSInformation")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetEndpointReference()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetEndpointReferenceResponse"), std::string(), std::string("<tt:GUID>GUID</tt:GUID>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetHostname()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetHostnameResponse"), std::string(), onvif::HostnameInformation(false, boost::none).ToXml("tt:HostnameInformation"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetIPAddressFilter()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetIPAddressFilterResponse"), std::string(), onvif::IPAddressFilter(onvif::IPADDRESSFILTERTYPE_ALLOW, std::vector<onvif::PrefixedIPv4Address>(), std::vector<onvif::PrefixedIPv6Address>()).ToXml(std::string("tt:IPAddressFilter")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetNetworkDefaultGateway()
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetNetworkDefaultGatewayResponse"), std::string(), onvif::NetworkGateway().ToXml(std::string("tt:NetworkGateway")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetNetworkInterfaces()
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetNetworkInterfacesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetNetworkProtocols()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetNetworkProtocolsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetNTP()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetNTPResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetPkcs10Request(const std::string& certificateid, boost::optional<std::string>& subject, const boost::optional<onvif::BinaryData>& attributes)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetPkcs10RequestResponse"), std::string(), onvif::BinaryData(std::vector<unsigned char>(), boost::none).ToXml(std::string("tt:Pkcs10Request")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetRelayOutputs()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetRelayOutputsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetRemoteDiscoveryMode()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetRemoteDiscoveryModeResponse"), std::string(), std::string("<tt:RemoteDiscoveryMode>NonDiscoverable</tt:RemoteDiscoveryMode>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetRemoteUser()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetRemoteUserResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetScopes()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetScopesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetServiceCapabilities()
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetServiceCapabilitiesResponse"), std::string(), onvif::device::DeviceServiceCapabilities(onvif::device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true), onvif::device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5), onvif::device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true), onvif::device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" })))).ToXml(std::string("Capabilities")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetServices(const std::string& localaddress, const uint16_t port, bool includecapability)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetServicesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetStorageConfiguration(const std::string& token)
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetStorageConfigurationResponse"), std::string(), onvif::device::StorageConfiguration(std::string("token"), onvif::device::StorageConfigurationData(std::string("type"), boost::none, boost::none, boost::none)).ToXml(std::string("Data")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetStorageConfigurations()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetStorageConfigurationsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetSystemBackup()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetSystemBackupResponse"), std::string(), onvif::BackupFile(std::string("name"), onvif::AttachmentData(boost::none, std::string("include"))).ToXml(std::string("tt:BackupFiles")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetSystemDateAndTime()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetSystemDateAndTimeResponse"), std::string(), onvif::SystemDateTime(onvif::DATETIME_NTP, true, boost::none, boost::none, boost::none).ToXml(std::string("tt:SystemDateAndTime")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetSystemLog(onvif::SYSTEMLOGTYPE logtype)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetSystemLogResponse"), std::string(), onvif::SystemLog().ToXml(std::string("tt:SystemLog")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetSystemSupportInformation()
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetSystemSupportInformationResponse"), std::string(), onvif::SupportInformation().ToXml(std::string("tt:SupportInformation")), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetSystemUris()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetSystemUrisResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetUsers()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetUsersResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetWsdlUrl(const std::string& localaddress, const uint16_t port)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetWsdlUrlResponse"), std::string(), std::string("<tt:WsdlUrl>http://127.0.0.1:9000/wsdl/</tt:WsdlUrl>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::GetZeroConfiguration()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:GetZeroConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::LoadCACertificates(const std::vector<onvif::Certificate>& cacertificate)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:LoadCACertificatesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::LoadCertificates(const std::vector<onvif::Certificate>& nvtcertificate)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:LoadCertificatesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::LoadCertificateWithPrivateKey(const std::vector<onvif::CertificateWithPrivateKey>& certificatewithprivatekey)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:LoadCertificateWithPrivateKeyResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::RemoveIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:RemoveIPAddressFilterResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::RemoveScopes(const std::vector<std::string>& scopeitem)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:RemoveScopesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::RestoreSystem(const std::vector<onvif::BackupFile>& backupfiles)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:RestoreSystemResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::ScanAvailableDot11Networks(const std::string& interfacetoken)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:ScanAvailableDot11NetworksResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SendAuxiliaryCommand(const std::string& auxiliarycommand)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SendAuxiliaryCommandResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetAccessPolicy(const onvif::BinaryData& policyfile)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetAccessPolicyResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetCertificatesStatus(const std::vector<onvif::CertificateStatus>& certificatestatus)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetCertificatesStatusResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetClientCertificateMode(bool enabled)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetClientCertificateModeResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetDiscoveryMode(onvif::DISCOVERYMODE discoverymode)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetDiscoveryModeResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetDNS(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<onvif::IPAddress>& dnsmanual)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetDNSResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetDot1XConfiguration(const onvif::Dot1XConfiguration& dot1xconfiguration)
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetDot1XConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}


ServerResponse DummyDeviceService::SetDPAddresses(const std::vector<onvif::NetworkHost>& dpaddress)
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetDPAddressesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetDynamicDNS(onvif::DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<onvif::Duration>& ttl)
{
  
  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetDynamicDNSResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetHostname(const std::string& name)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetHostnameResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetHostnameFromDHCP(bool fromdhcp)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetHostnameFromDHCPResponse"), std::string(), std::string("<tt:RebootNeeded>false</tt:RebootNeeded>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetIPAddressFilter(const onvif::IPAddressFilter& ipaddressfilter)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetIPAddressFilterResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetNetworkDefaultGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetNetworkDefaultGatewayResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetNetworkInterfaces(const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetNetworkInterfacesResponse"), std::string(), std::string("<tt:RebootNeeded>false</tt:RebootNeeded>"), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetNetworkProtocols(const std::vector<NetworkProtocol>& networkprotocols)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetNetworkProtocolsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetNTP(bool fromdhcp, const std::vector<onvif::NetworkHost>& ntpmanual)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetNTPResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetRelayOutputSettings(const std::string& relayoutputtoken, const onvif::RelayOutputSettings& properties)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetRelayOutputSettingsResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetRelayOutputState(const std::string& relayoutputtoken, onvif::RELAYLOGICALSTATE logicalstate)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetRelayOutputStateResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetRemoteUser(const boost::optional<onvif::RemoteUser>& remoteuser)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetRemoteUserResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetRemoteDiscoveryMode(onvif::DISCOVERYMODE RemoteDiscoveryMode)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetRemoteDiscoveryModeResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetScopes(const std::vector<std::string>& scopes)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetScopesResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetSystemDateAndTime(onvif::DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<onvif::TimeZone>& timezone, const boost::optional<onvif::DateTime>& datetime)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetSystemDateAndTimeResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetSystemFactoryDefault(onvif::FACTORYDEFAULTTYPE factorydefault)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetSystemFactoryDefaultResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetUser(const std::vector<onvif::User>& users)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetUserResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SetZeroConfiguration(const std::string& interfacetoken, bool enabled)
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SetZeroConfigurationResponse"), std::string(), std::string(), namespaces_, std::vector<std::string>()));
}

ServerResponse DummyDeviceService::SystemReboot()
{

  return ServerResponse(HTTPSTATUSCODE::OK, Envelope(std::string("device:SystemRebootResponse"), std::string(), std::string("<tt:Message>bye</tt:Message>"), namespaces_, std::vector<std::string>()));
}

}

}

}
