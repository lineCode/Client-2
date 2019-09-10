// deviceclient.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

#include <boost/date_time/gregorian/parsers.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

class DeviceSignals
{
 public:

  Signal< DEVICEOPERATION, DeviceClient, AddIPAddressFilterResponse, IPAddressFilter> addipaddressfilter_;
  Signal< DEVICEOPERATION, DeviceClient, AddScopesResponse, std::vector<std::string> > addscopes_;
  Signal< DEVICEOPERATION, DeviceClient, CreateCertificateResponse, boost::optional<std::string>, boost::optional<std::string>, boost::optional<onvif::ws::DateTime>, boost::optional<onvif::ws::DateTime> > createcertificate_;
  Signal< DEVICEOPERATION, DeviceClient, CreateStorageConfigurationResponse, StorageConfigurationData> createstorageconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, CreateUsersResponse, std::vector<User> > createusers_;
  Signal< DEVICEOPERATION, DeviceClient, DeleteCertificatesResponse, std::vector<std::string> > deletecertificates_;
  Signal< DEVICEOPERATION, DeviceClient, DeleteDot1XConfigurationResponse, std::vector<std::string> > deletedot1xconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, DeleteStorageConfigurationResponse, std::vector<std::string> > deletestorageconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, DeleteUsersResponse, std::vector<std::string> > deleteusers_;
  Signal< DEVICEOPERATION, DeviceClient, GetAccessPolicyResponse> getaccesspolicy_;
  Signal< DEVICEOPERATION, DeviceClient, GetCACertificatesResponse> getcacertificates_;
  Signal< DEVICEOPERATION, DeviceClient, GetCapabilitiesResponse, CAPABILITYCATEGORY> getcapabilities_;
  Signal< DEVICEOPERATION, DeviceClient, GetCertificateInformationResponse, std::string> getcertificateinformation_;
  Signal< DEVICEOPERATION, DeviceClient, GetCertificatesResponse> getcertificates_;
  Signal< DEVICEOPERATION, DeviceClient, GetCertificatesStatusResponse> getcertificatesstatus_;
  Signal< DEVICEOPERATION, DeviceClient, GetClientCertificateModeResponse> getclientcertificatemode_;
  Signal< DEVICEOPERATION, DeviceClient, GetDeviceInformationResponse> getdeviceinformation_;
  Signal< DEVICEOPERATION, DeviceClient, GetDiscoveryModeResponse> getdiscoverymode_;
  Signal< DEVICEOPERATION, DeviceClient, GetDNSResponse> getdns_;
  Signal< DEVICEOPERATION, DeviceClient, GetDot11CapabilitiesResponse> getdot11capabilities_;
  Signal< DEVICEOPERATION, DeviceClient, GetDot11StatusResponse, std::string> getdot11status_;
  Signal< DEVICEOPERATION, DeviceClient, GetDot1XConfigurationResponse, std::string> getdot1xconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, GetDot1XConfigurationsResponse> getdot1xconfigurations_;
  Signal< DEVICEOPERATION, DeviceClient, GetDPAddressesResponse> getdpaddresses_;
  Signal< DEVICEOPERATION, DeviceClient, GetDynamicDNSResponse> getdynamicdns_;
  Signal< DEVICEOPERATION, DeviceClient, GetEndpointReferenceResponse> getendpointreference_;
  Signal< DEVICEOPERATION, DeviceClient, GetHostnameResponse> gethostname_;
  Signal< DEVICEOPERATION, DeviceClient, GetIPAddressFilterResponse> getipaddressfilter_;
  Signal< DEVICEOPERATION, DeviceClient, GetNetworkDefaultGatewayResponse> getnetworkdefaultgateway_;
  Signal< DEVICEOPERATION, DeviceClient, GetNetworkInterfacesResponse> getnetworkinterfaces_;
  Signal< DEVICEOPERATION, DeviceClient, GetNetworkProtocolsResponse> getnetworkprotocols_;
  Signal< DEVICEOPERATION, DeviceClient, GetPkcs10RequestResponse, std::string, boost::optional<std::string>, boost::optional<BinaryData> > getpkcs10request_;
  Signal< DEVICEOPERATION, DeviceClient, GetRelayOutputsResponse> getrelayoutputs_;
  Signal< DEVICEOPERATION, DeviceClient, GetNTPResponse> getntp_;
  Signal< DEVICEOPERATION, DeviceClient, GetRemoteDiscoveryModeResponse> getremotediscoverymode_;
  Signal< DEVICEOPERATION, DeviceClient, GetRemoteUserResponse> getremoteuser_;
  Signal< DEVICEOPERATION, DeviceClient, GetScopesResponse> getscopes_;
  Signal< DEVICEOPERATION, DeviceClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal< DEVICEOPERATION, DeviceClient, GetServicesResponse, bool> getservices_;
  Signal< DEVICEOPERATION, DeviceClient, GetStorageConfigurationResponse, std::string> getstorageconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, GetStorageConfigurationsResponse> getstorageconfigurations_;
  Signal< DEVICEOPERATION, DeviceClient, GetSystemBackupResponse> getsystembackup_;
  Signal< DEVICEOPERATION, DeviceClient, GetSystemDateAndTimeResponse> getsystemdateandtime_;
  Signal< DEVICEOPERATION, DeviceClient, GetSystemLogResponse, SYSTEMLOGTYPE> getsystemlog_;
  Signal< DEVICEOPERATION, DeviceClient, GetSystemSupportInformationResponse> getsystemsupportinformation_;
  Signal< DEVICEOPERATION, DeviceClient, GetSystemUrisResponse> getsystemuris_;
  Signal< DEVICEOPERATION, DeviceClient, GetUsersResponse> getusers_;
  Signal< DEVICEOPERATION, DeviceClient, GetWsdlUrlResponse> getwsdlurl_;
  Signal< DEVICEOPERATION, DeviceClient, GetZeroConfigurationResponse> getzeroconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, LoadCACertificatesResponse, std::vector<Certificate> > loadcacertificates_;
  Signal< DEVICEOPERATION, DeviceClient, LoadCertificatesResponse, std::vector<Certificate> > loadcertificates_;
  Signal< DEVICEOPERATION, DeviceClient, LoadCertificateWithPrivateKeyResponse, std::vector<CertificateWithPrivateKey> > loadcertificatewithprivatekey_;
  Signal< DEVICEOPERATION, DeviceClient, RemoveIPAddressFilterResponse, IPAddressFilter> removeipaddressfilter_;
  Signal< DEVICEOPERATION, DeviceClient, RemoveScopesResponse, std::vector<std::string> > removescopes_;
  Signal< DEVICEOPERATION, DeviceClient, RestoreSystemResponse, std::vector<BackupFile> > restoresystem_;
  Signal< DEVICEOPERATION, DeviceClient, ScanAvailableDot11NetworksResponse, std::string> scanavailabledot11networks_;
  Signal< DEVICEOPERATION, DeviceClient, SendAuxiliaryCommandResponse, std::string> sendauxiliarycommand_;
  Signal< DEVICEOPERATION, DeviceClient, SetAccessPolicyResponse, BinaryData> setaccesspolicy_;
  Signal< DEVICEOPERATION, DeviceClient, SetCertificatesStatusResponse, std::vector<CertificateStatus> > setcertificatesstatus_;
  Signal< DEVICEOPERATION, DeviceClient, SetClientCertificateModeResponse, bool> setclientcertificatemode_;
  Signal< DEVICEOPERATION, DeviceClient, SetDiscoveryModeResponse, DISCOVERYMODE> setdiscoverymode_;
  Signal< DEVICEOPERATION, DeviceClient, SetDNSResponse, bool, std::vector<std::string>, std::vector<IPAddress> > setdns_;
  Signal< DEVICEOPERATION, DeviceClient, SetDot1XConfigurationResponse, Dot1XConfiguration> setdot1xconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, SetDPAddressesResponse, std::vector<NetworkHost> > setdpaddresses_;
  Signal< DEVICEOPERATION, DeviceClient, SetDynamicDNSResponse, DYNAMICDNSTYPE, boost::optional<std::string>, boost::optional<Duration> > setdynamicdns_;
  Signal< DEVICEOPERATION, DeviceClient, SetHostnameResponse, std::string> sethostname_;
  Signal< DEVICEOPERATION, DeviceClient, SetHostnameFromDHCPResponse, bool> sethostnamefromdhcp_;
  Signal< DEVICEOPERATION, DeviceClient, SetIPAddressFilterResponse, IPAddressFilter> setipaddressfilter_;
  Signal< DEVICEOPERATION, DeviceClient, SetNetworkDefaultGatewayResponse, std::vector<std::string>, std::vector<std::string> > setnetworkdefaultgateway_;
  Signal< DEVICEOPERATION, DeviceClient, SetNetworkInterfacesResponse, std::string, onvif::NetworkInterfaceSetConfiguration > setnetworkinterfaces_;
  Signal< DEVICEOPERATION, DeviceClient, SetNetworkProtocolsResponse, std::vector<NetworkProtocol> > setnetworkprotocols_;
  Signal< DEVICEOPERATION, DeviceClient, SetNTPResponse, bool, std::vector<NetworkHost> > setntp_;
  Signal< DEVICEOPERATION, DeviceClient, SetRelayOutputSettingsResponse, std::string, RelayOutputSettings> setrelayoutputsettings_;
  Signal< DEVICEOPERATION, DeviceClient, SetRemoteDiscoveryModeResponse, DISCOVERYMODE> setremotediscoverymode_;
  Signal< DEVICEOPERATION, DeviceClient, SetSystemDateAndTimeResponse, DATETIMETYPE, bool, boost::optional<TimeZone>, boost::optional<DateTime> > setsystemdateandtime_;
  Signal< DEVICEOPERATION, DeviceClient, SetSystemFactoryDefaultResponse, FACTORYDEFAULTTYPE> setsystemfactorydefault_;
  Signal< DEVICEOPERATION, DeviceClient, SetUserResponse, std::vector<User> > setuser_;
  Signal< DEVICEOPERATION, DeviceClient, SetZeroConfigurationResponse, std::string, bool> setzeroconfiguration_;
  Signal< DEVICEOPERATION, DeviceClient, SystemRebootResponse> systemreboot_;

};

///// Methods /////

DeviceClient::DeviceClient() :
  signals_(new DeviceSignals(
  {
    Signal< DEVICEOPERATION, DeviceClient, AddIPAddressFilterResponse, IPAddressFilter>(this, DEVICEOPERATION_ADDIPADDRESSFILTER, true, std::string("http://www.onvif.org/ver10/device/wsdl/AddIPAddressFilter"), false),
    Signal< DEVICEOPERATION, DeviceClient, AddScopesResponse, std::vector<std::string> >(this, DEVICEOPERATION_ADDSCOPES, true, std::string("http://www.onvif.org/ver10/device/wsdl/AddScopes"), false),
    Signal< DEVICEOPERATION, DeviceClient, CreateCertificateResponse, boost::optional<std::string>, boost::optional<std::string>, boost::optional<onvif::ws::DateTime>, boost::optional<onvif::ws::DateTime> >(this, DEVICEOPERATION_CREATECERTIFICATE, true, std::string("http://www.onvif.org/ver10/device/wsdl/CreateCertificate"), false),
    Signal< DEVICEOPERATION, DeviceClient, CreateStorageConfigurationResponse, StorageConfigurationData>(this, DEVICEOPERATION_CREATESTORAGECONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/CreateStorageConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, CreateUsersResponse, std::vector<User> >(this, DEVICEOPERATION_CREATEUSERS, true, std::string("http://www.onvif.org/ver10/device/wsdl/CreateUsers"), false),
    Signal< DEVICEOPERATION, DeviceClient, DeleteCertificatesResponse, std::vector<std::string> >(this, DEVICEOPERATION_DELETECERTIFICATES, true, std::string("http://www.onvif.org/ver10/device/wsdl/DeleteCertificates"), false),
    Signal< DEVICEOPERATION, DeviceClient, DeleteDot1XConfigurationResponse, std::vector<std::string> >(this, DEVICEOPERATION_DELETEDOT1XCONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/DeleteDot1XConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, DeleteStorageConfigurationResponse, std::vector<std::string> >(this, DEVICEOPERATION_DELETESTORAGECONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/DeleteStorageConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, DeleteUsersResponse, std::vector<std::string> >(this, DEVICEOPERATION_DELETEUSERS, true, std::string("http://www.onvif.org/ver10/device/wsdl/DeleteUsers"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetAccessPolicyResponse>(this, DEVICEOPERATION_GETACCESSPOLICY, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetAccessPolicy"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetCACertificatesResponse>(this, DEVICEOPERATION_GETCACERTIFICATES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetCACertificates"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetCapabilitiesResponse, CAPABILITYCATEGORY>(this, DEVICEOPERATION_GETCAPABILITIES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetCapabilities"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetCertificateInformationResponse, std::string>(this, DEVICEOPERATION_GETCERTIFICATEINFORMATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetCertificateInformation"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetCertificatesResponse>(this, DEVICEOPERATION_GETCERTIFICATES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetCertificates"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetCertificatesStatusResponse>(this, DEVICEOPERATION_GETCERTIFICATESSTATUS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetCertificatesStatus"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetClientCertificateModeResponse>(this, DEVICEOPERATION_GETCLIENTCERTIFICATEMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetClientCertificateMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDeviceInformationResponse>(this, DEVICEOPERATION_GETDEVICEINFORMATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDiscoveryModeResponse>(this, DEVICEOPERATION_GETDISCOVERYMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDiscoveryMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDNSResponse>(this, DEVICEOPERATION_GETDNS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDNS"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDot11CapabilitiesResponse>(this, DEVICEOPERATION_GETDOT11CAPABILITIES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDot11Capabilities"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDot11StatusResponse, std::string>(this, DEVICEOPERATION_GETDOT11STATUS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDot11Status"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDot1XConfigurationResponse, std::string>(this, DEVICEOPERATION_GETDOT1XCONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDot1XConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDot1XConfigurationsResponse>(this, DEVICEOPERATION_GETDOT1XCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDot1XConfigurations"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDPAddressesResponse>(this, DEVICEOPERATION_GETDPADDRESSES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDPAddresses"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetDynamicDNSResponse>(this, DEVICEOPERATION_GETDYNAMICDNS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetDynamicDNS"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetEndpointReferenceResponse>(this, DEVICEOPERATION_GETENDPOINTREFERENCE, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetEndpointReference"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetHostnameResponse>(this, DEVICEOPERATION_GETHOSTNAME, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetHostname"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetIPAddressFilterResponse>(this, DEVICEOPERATION_GETIPADDRESSFILTER, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetIPAddressFilter"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetNetworkDefaultGatewayResponse>(this, DEVICEOPERATION_GETNETWORKDEFAULTGATEWAY, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetNetworkDefaultGateway"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetNetworkInterfacesResponse>(this, DEVICEOPERATION_GETNETWORKINTERFACES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetNetworkInterfaces"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetNetworkProtocolsResponse>(this, DEVICEOPERATION_GETNETWORKPROTOCOLS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetNetworkProtocols"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetPkcs10RequestResponse, std::string, boost::optional<std::string>, boost::optional<BinaryData> >(this, DEVICEOPERATION_GETPKCS10REQUEST, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetPkcs10Request"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetRelayOutputsResponse>(this, DEVICEOPERATION_GETRELAYOUTPUTS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetRelayOutputs"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetNTPResponse>(this, DEVICEOPERATION_GETNTP, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetNTP"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetRemoteDiscoveryModeResponse>(this, DEVICEOPERATION_GETREMOTEDISCOVERYMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetRemoteDiscoveryMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetRemoteUserResponse>(this, DEVICEOPERATION_GETREMOTEUSER, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetRemoteUser"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetScopesResponse>(this, DEVICEOPERATION_GETSCOPES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetScopes"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetServiceCapabilitiesResponse>(this, DEVICEOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetServiceCapabilities"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetServicesResponse, bool>(this, DEVICEOPERATION_GETSERVICES, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetServices"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetStorageConfigurationResponse, std::string>(this, DEVICEOPERATION_GETSTORAGECONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetStorageConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetStorageConfigurationsResponse>(this, DEVICEOPERATION_GETSTORAGECONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetStorageConfigurations"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetSystemBackupResponse>(this, DEVICEOPERATION_GETSYSTEMBACKUP, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetSystemBackup"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetSystemDateAndTimeResponse>(this, DEVICEOPERATION_GETSYSTEMDATEANDTIME, false, std::string("http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetSystemLogResponse, SYSTEMLOGTYPE>(this, DEVICEOPERATION_GETSYSTEMLOG, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetSystemLog"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetSystemSupportInformationResponse>(this, DEVICEOPERATION_GETSYSTEMSUPPORTINFORMATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetSystemSupportInformation"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetSystemUrisResponse>(this, DEVICEOPERATION_GETSYSTEMURIS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetSystemUris"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetUsersResponse>(this, DEVICEOPERATION_GETUSERS, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetUsers"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetWsdlUrlResponse>(this, DEVICEOPERATION_GETWSDLURL, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetWsdlUrl"), false),
    Signal< DEVICEOPERATION, DeviceClient, GetZeroConfigurationResponse>(this, DEVICEOPERATION_GETZEROCONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/GetZeroConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, LoadCACertificatesResponse, std::vector<Certificate> >(this, DEVICEOPERATION_LOADCACERTIFICATES, true, std::string("http://www.onvif.org/ver10/device/wsdl/LoadCACertificates"), false),
    Signal< DEVICEOPERATION, DeviceClient, LoadCertificatesResponse, std::vector<Certificate> >(this, DEVICEOPERATION_LOADCERTIFICATES, true, std::string("http://www.onvif.org/ver10/device/wsdl/LoadCertificates"), false),
    Signal< DEVICEOPERATION, DeviceClient, LoadCertificateWithPrivateKeyResponse, std::vector<CertificateWithPrivateKey> >(this, DEVICEOPERATION_LOADCERTIFICATEWITHPRIVATEKEY, true, std::string("http://www.onvif.org/ver10/device/wsdl/LoadCertificateWithPrivateKey"), false),
    Signal< DEVICEOPERATION, DeviceClient, RemoveIPAddressFilterResponse, IPAddressFilter>(this, DEVICEOPERATION_REMOVEIPADDRESSFILTER, true, std::string("http://www.onvif.org/ver10/device/wsdl/RemoveIPAddressFilter"), false),
    Signal< DEVICEOPERATION, DeviceClient, RemoveScopesResponse, std::vector<std::string> >(this, DEVICEOPERATION_REMOVESCOPES, true, std::string("http://www.onvif.org/ver10/device/wsdl/RemoveScopes"), false),
    Signal< DEVICEOPERATION, DeviceClient, RestoreSystemResponse, std::vector<BackupFile> >(this, DEVICEOPERATION_RESTORESYSTEM, true, std::string("http://www.onvif.org/ver10/device/wsdl/RestoreSystem"), false),
    Signal< DEVICEOPERATION, DeviceClient, ScanAvailableDot11NetworksResponse, std::string>(this, DEVICEOPERATION_SCANAVAILABLEDOT11NETWORKS, true, std::string("http://www.onvif.org/ver10/device/wsdl/ScanAvailableDot11Networks"), false),
    Signal< DEVICEOPERATION, DeviceClient, SendAuxiliaryCommandResponse, std::string>(this, DEVICEOPERATION_SENDAUXILIARYCOMMAND, true, std::string("http://www.onvif.org/ver10/device/wsdl/SendAuxiliaryCommand"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetAccessPolicyResponse, BinaryData>(this, DEVICEOPERATION_SETACCESSPOLICY, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetAccessPolicy"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetCertificatesStatusResponse, std::vector<CertificateStatus> >(this, DEVICEOPERATION_SETCERTIFICATESSTATUS, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetCertificatesStatus"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetClientCertificateModeResponse, bool>(this, DEVICEOPERATION_SETCLIENTCERTIFICATEMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetClientCertificateMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetDiscoveryModeResponse, DISCOVERYMODE>(this, DEVICEOPERATION_SETDISCOVERYMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetDiscoveryMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetDNSResponse, bool, std::vector<std::string>, std::vector<IPAddress> >(this, DEVICEOPERATION_SETDNS, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetDNS"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetDot1XConfigurationResponse, Dot1XConfiguration>(this, DEVICEOPERATION_SETDOT1XCONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetDot1XConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetDPAddressesResponse, std::vector<NetworkHost> >(this, DEVICEOPERATION_SETDPADDRESSES, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetDPAddresses"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetDynamicDNSResponse, DYNAMICDNSTYPE, boost::optional<std::string>, boost::optional<Duration> >(this, DEVICEOPERATION_SETDYNAMICDNS, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetDynamicDNS"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetHostnameResponse, std::string>(this, DEVICEOPERATION_SETHOSTNAME, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetHostname"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetHostnameFromDHCPResponse, bool>(this, DEVICEOPERATION_SETHOSTNAMEFROMDHCP, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetHostnameFromDHCP"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetIPAddressFilterResponse, IPAddressFilter>(this, DEVICEOPERATION_SETIPADDRESSFILTER, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetIPAddressFilter"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetNetworkDefaultGatewayResponse, std::vector<std::string>, std::vector<std::string> >(this, DEVICEOPERATION_SETNETWORKDEFAULTGATEWAY, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetNetworkDefaultGateway"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetNetworkInterfacesResponse, std::string, onvif::NetworkInterfaceSetConfiguration >(this, DEVICEOPERATION_SETNETWORKINTERFACES, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetNetworkInterfaces"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetNetworkProtocolsResponse, std::vector<NetworkProtocol> >(this, DEVICEOPERATION_SETNETWORKPROTOCOLS, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetNetworkProtocols"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetNTPResponse, bool, std::vector<NetworkHost> >(this, DEVICEOPERATION_SETNTP, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetNTP"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetRelayOutputSettingsResponse, std::string, RelayOutputSettings>(this, DEVICEOPERATION_SETRELAYOUTPUTSETTINGS, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetRelayOutputSettings"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetRemoteDiscoveryModeResponse, DISCOVERYMODE>(this, DEVICEOPERATION_SETREMOTEDISCOVERYMODE, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetRemoteDiscoveryMode"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetSystemDateAndTimeResponse, DATETIMETYPE, bool, boost::optional<TimeZone>, boost::optional<DateTime> >(this, DEVICEOPERATION_SETSYSTEMDATEANDTIME, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetSystemDateAndTime"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetSystemFactoryDefaultResponse, FACTORYDEFAULTTYPE>(this, DEVICEOPERATION_SETSYSTEMFACTORYDEFAULT, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetSystemFactoryDefault"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetUserResponse, std::vector<User> >(this, DEVICEOPERATION_SETUSER, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetUser"), false),
    Signal< DEVICEOPERATION, DeviceClient, SetZeroConfigurationResponse, std::string, bool>(this, DEVICEOPERATION_SETZEROCONFIGURATION, true, std::string("http://www.onvif.org/ver10/device/wsdl/SetZeroConfiguration"), false),
    Signal< DEVICEOPERATION, DeviceClient, SystemRebootResponse>(this, DEVICEOPERATION_SYSTEMREBOOT, true, std::string("http://www.onvif.org/ver10/device/wsdl/SystemReboot"), false)
  }))
{

}

DeviceClient::~DeviceClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void DeviceClient::Destroy()
{
  Client::Destroy();

  signals_->addipaddressfilter_.Destroy();
  signals_->addscopes_.Destroy();
  signals_->createcertificate_.Destroy();
  signals_->createusers_.Destroy();
  signals_->deletecertificates_.Destroy();
  signals_->deletedot1xconfiguration_.Destroy();
  signals_->deletestorageconfiguration_.Destroy();
  signals_->deleteusers_.Destroy();
  signals_->getaccesspolicy_.Destroy();
  signals_->getcacertificates_.Destroy();
  signals_->getcapabilities_.Destroy();
  signals_->getcertificateinformation_.Destroy();
  signals_->getcertificates_.Destroy();
  signals_->getcertificatesstatus_.Destroy();
  signals_->getclientcertificatemode_.Destroy();
  signals_->getdeviceinformation_.Destroy();
  signals_->getdiscoverymode_.Destroy();
  signals_->getdns_.Destroy();
  signals_->getdot11capabilities_.Destroy();
  signals_->getdot11status_.Destroy();
  signals_->getdot1xconfiguration_.Destroy();
  signals_->getdot1xconfigurations_.Destroy();
  signals_->getdpaddresses_.Destroy();
  signals_->getdynamicdns_.Destroy();
  signals_->getendpointreference_.Destroy();
  signals_->gethostname_.Destroy();
  signals_->getipaddressfilter_.Destroy();
  signals_->getnetworkdefaultgateway_.Destroy();
  signals_->getnetworkinterfaces_.Destroy();
  signals_->getnetworkprotocols_.Destroy();
  signals_->getpkcs10request_.Destroy();
  signals_->getrelayoutputs_.Destroy();
  signals_->getntp_.Destroy();
  signals_->getremotediscoverymode_.Destroy();
  signals_->getremoteuser_.Destroy();
  signals_->getscopes_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->getservices_.Destroy();
  signals_->getstorageconfiguration_.Destroy();
  signals_->getstorageconfigurations_.Destroy();
  signals_->getsystembackup_.Destroy();
  signals_->getsystemdateandtime_.Destroy();
  signals_->getsystemlog_.Destroy();
  signals_->getsystemsupportinformation_.Destroy();
  signals_->getsystemuris_.Destroy();
  signals_->getusers_.Destroy();
  signals_->getwsdlurl_.Destroy();
  signals_->getzeroconfiguration_.Destroy();
  signals_->loadcacertificates_.Destroy();
  signals_->loadcertificates_.Destroy();
  signals_->loadcertificatewithprivatekey_.Destroy();
  signals_->removeipaddressfilter_.Destroy();
  signals_->removescopes_.Destroy();
  signals_->restoresystem_.Destroy();
  signals_->scanavailabledot11networks_.Destroy();
  signals_->sendauxiliarycommand_.Destroy();
  signals_->setaccesspolicy_.Destroy();
  signals_->setcertificatesstatus_.Destroy();
  signals_->setclientcertificatemode_.Destroy();
  signals_->setdiscoverymode_.Destroy();
  signals_->setdns_.Destroy();
  signals_->setdot1xconfiguration_.Destroy();
  signals_->setdpaddresses_.Destroy();
  signals_->setdynamicdns_.Destroy();
  signals_->sethostname_.Destroy();
  signals_->sethostnamefromdhcp_.Destroy();
  signals_->setipaddressfilter_.Destroy();
  signals_->setnetworkdefaultgateway_.Destroy();
  signals_->setnetworkinterfaces_.Destroy();
  signals_->setnetworkprotocols_.Destroy();
  signals_->setntp_.Destroy();
  signals_->setrelayoutputsettings_.Destroy();
  signals_->setremotediscoverymode_.Destroy();
  signals_->setsystemdateandtime_.Destroy();
  signals_->setsystemfactorydefault_.Destroy();
  signals_->setuser_.Destroy();
  signals_->setzeroconfiguration_.Destroy();
  signals_->systemreboot_.Destroy();
}

// Requests
void DeviceClient::AddIPAddressFilter(const IPAddressFilter& ipaddressfilter)
{
  signals_->addipaddressfilter_.Create(AddIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

void DeviceClient::AddScopes(const std::vector<std::string>& scopes)
{
  signals_->addscopes_.Create(AddScopesBody(scopes), scopes);
}

void DeviceClient::CreateCertificate(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter)
{
  signals_->createcertificate_.Create(CreateCertificateBody(certificateid, subject, validnotbefore, validnotafter), certificateid, subject, validnotbefore, validnotafter);
}

void DeviceClient::CreateStorageConfiguration(const StorageConfigurationData& storageconfiguration)
{
  signals_->createstorageconfiguration_.Create(CreateStorageConfigurationBody(storageconfiguration), storageconfiguration);
}

void DeviceClient::CreateUsers(const std::vector<User>& users)
{
  signals_->createusers_.Create(CreateUsersBody(users), users);
}

void DeviceClient::DeleteCertificates(const std::vector<std::string>& certificateid)
{
  signals_->deletecertificates_.Create(DeleteCertificatesBody(certificateid), certificateid);
}

void DeviceClient::DeleteDot1XConfiguration(const std::vector<std::string>& dot1xconfigurationtoken)
{
  signals_->deletedot1xconfiguration_.Create(DeleteDot1XConfigurationBody(dot1xconfigurationtoken), dot1xconfigurationtoken);
}

void DeviceClient::DeleteStorageConfiguration(const std::vector<std::string>& token)
{
  signals_->deletestorageconfiguration_.Create(DeleteStorageConfigurationBody(token), token);
}

void DeviceClient::DeleteUsers(const std::vector<std::string>& usernames)
{
  signals_->deleteusers_.Create(DeleteUsersBody(usernames), usernames);
}

void DeviceClient::GetAccessPolicy()
{
  signals_->getaccesspolicy_.Create(GetAccessPolicyBody());
}

void DeviceClient::GetCACertificates()
{
  signals_->getcacertificates_.Create(GetCACertificatesBody());
}

void DeviceClient::GetCapabilities(CAPABILITYCATEGORY capabilitycategory)
{
  signals_->getcapabilities_.Create(GetCapabilitiesBody(capabilitycategory), capabilitycategory);
}

void DeviceClient::GetCertificateInformation(const std::string& certificateid)
{
  signals_->getcertificateinformation_.Create(GetCertificateInformationBody(certificateid), certificateid);
}

void DeviceClient::GetCertificates()
{
  signals_->getcertificates_.Create(GetCertificatesBody());
}

void DeviceClient::GetCertificatesStatus()
{
  signals_->getcertificatesstatus_.Create(GetCertificatesStatusBody());
}

void DeviceClient::GetClientCertificateMode()
{
  signals_->getclientcertificatemode_.Create(GetClientCertificateModeBody());
}

void DeviceClient::GetDeviceInformation()
{
  signals_->getdeviceinformation_.Create(GetDeviceInformationBody());
}

void DeviceClient::GetDiscoveryMode()
{
  signals_->getdiscoverymode_.Create(GetDiscoveryModeBody());
}

void DeviceClient::GetDNS()
{
  signals_->getdns_.Create(GetDNSBody());
}

void DeviceClient::GetDot11Capabilities()
{
  signals_->getdot11capabilities_.Create(GetDot11CapabilitiesBody());
}

void DeviceClient::GetDot11Status(const std::string& interfacetoken)
{
  signals_->getdot11status_.Create(GetDot11StatusBody(interfacetoken), interfacetoken);
}

void DeviceClient::GetDot1XConfiguration(const std::string& dot1xconfigurationtoken)
{
  signals_->getdot1xconfiguration_.Create(GetDot1XConfigurationBody(dot1xconfigurationtoken), dot1xconfigurationtoken);
}

void DeviceClient::GetDot1XConfigurations()
{
  signals_->getdot1xconfigurations_.Create(GetDot1XConfigurationsBody());
}

void DeviceClient::GetDPAddresses()
{
  signals_->getdpaddresses_.Create(GetDPAddressesBody());
}

void DeviceClient::GetDynamicDNS()
{
  signals_->getdynamicdns_.Create(GetDynamicDNSBody());
}

void DeviceClient::GetEndpointReference()
{
  signals_->getendpointreference_.Create(GetEndpointReferenceBody());
}

void DeviceClient::GetHostname()
{
  signals_->gethostname_.Create(GetHostnameBody());
}

void DeviceClient::GetIPAddressFilter()
{
  signals_->getipaddressfilter_.Create(GetIPAddressFilterBody());
}

void DeviceClient::GetNetworkDefaultGateway()
{
  signals_->getnetworkdefaultgateway_.Create(GetNetworkDefaultGatewayBody());
}

void DeviceClient::GetNetworkInterfaces()
{
  signals_->getnetworkinterfaces_.Create(GetNetworkInterfacesBody());
}

void DeviceClient::GetNetworkProtocols()
{
  signals_->getnetworkprotocols_.Create(GetNetworkProtocolsBody());
}

void DeviceClient::GetPkcs10Request(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes)
{
  signals_->getpkcs10request_.Create(GetPkcs10RequestBody(certificateid, subject, attributes), certificateid, subject, attributes);
}

void DeviceClient::GetRelayOutputs()
{
  signals_->getrelayoutputs_.Create(GetRelayOutputsBody());
}

void DeviceClient::GetNTP()
{
  signals_->getntp_.Create(GetNTPBody());
}

void DeviceClient::GetRemoteDiscoveryMode()
{
  signals_->getremotediscoverymode_.Create(GetRemoteDiscoveryModeBody());
}

void DeviceClient::GetRemoteUser()
{
  signals_->getremoteuser_.Create(GetRemoteUserBody());
}

void DeviceClient::GetScopes()
{
  signals_->getscopes_.Create(GetScopesBody());
}

void DeviceClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void DeviceClient::GetServices(bool includecapability)
{
  signals_->getservices_.Create(GetServicesBody(includecapability), includecapability);
}

void DeviceClient::GetStorageConfiguration(const std::string& token)
{
  signals_->getstorageconfiguration_.Create(GetStorageConfigurationBody(token), token);
}

void DeviceClient::GetStorageConfigurations()
{
  signals_->getstorageconfigurations_.Create(GetStorageConfigurationsBody());
}

void DeviceClient::GetSystemBackup()
{
  signals_->getsystembackup_.Create(GetSystemBackupBody());
}

void DeviceClient::GetSystemDateAndTime()
{
  signals_->getsystemdateandtime_.Create(GetSystemDateAndTimeBody());
}

void DeviceClient::GetSystemLog(SYSTEMLOGTYPE logtype)
{
  signals_->getsystemlog_.Create(GetSystemLogBody(logtype), logtype);
}

void DeviceClient::GetSystemSupportInformation()
{
  signals_->getsystemsupportinformation_.Create(GetSystemSupportInformationBody());
}

void DeviceClient::GetSystemUris()
{
  signals_->getsystemuris_.Create(GetSystemUrisBody());
}

void DeviceClient::GetUsers()
{
  signals_->getusers_.Create(GetUsersBody());
}

void DeviceClient::GetWsdlUrl()
{
  signals_->getwsdlurl_.Create(GetWsdlUrlBody());
}

void DeviceClient::GetZeroConfiguration()
{
  signals_->getzeroconfiguration_.Create(GetZeroConfigurationBody());
}

void DeviceClient::LoadCACertificates(const std::vector<Certificate>& cacertificate)
{
  signals_->loadcacertificates_.Create(LoadCACertificatesBody(cacertificate), cacertificate);
}

void DeviceClient::LoadCertificates(const std::vector<Certificate>& nvtcertificate)
{
  signals_->loadcertificates_.Create(LoadCertificatesBody(nvtcertificate), nvtcertificate);
}

void DeviceClient::LoadCertificateWithPrivateKey(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey)
{
  signals_->loadcertificatewithprivatekey_.Create(LoadCertificateWithPrivateKeyBody(certificatewithprivatekey), certificatewithprivatekey);
}

void DeviceClient::RemoveIPAddressFilter(const IPAddressFilter& ipaddressfilter)
{
  signals_->removeipaddressfilter_.Create(RemoveIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

void DeviceClient::RemoveScopes(const std::vector<std::string>& scopes)
{
  signals_->removescopes_.Create(RemoveScopesBody(scopes), scopes);
}

void DeviceClient::RestoreSystem(const std::vector<BackupFile>& backupfiles)
{
  const auto body = RestoreSystemBody(backupfiles);
  signals_->restoresystem_.Create(body.first, body.second, backupfiles);
}

void DeviceClient::ScanAvailableDot11Networks(const std::string& interfacetoken)
{
  signals_->scanavailabledot11networks_.Create(ScanAvailableDot11NetworksBody(interfacetoken), interfacetoken);
}

void DeviceClient::SendAuxiliaryCommand(const std::string& auxiliarycommand)
{
  signals_->sendauxiliarycommand_.Create(SendAuxiliaryCommandBody(auxiliarycommand), auxiliarycommand);
}

void DeviceClient::SetAccessPolicy(const BinaryData& policyfile)
{
  signals_->setaccesspolicy_.Create(SetAccessPolicyBody(policyfile), policyfile);
}

void DeviceClient::SetCertificatesStatus(const std::vector<CertificateStatus>& certificatestatus)
{
  signals_->setcertificatesstatus_.Create(SetCertificatesStatusBody(certificatestatus), certificatestatus);
}

void DeviceClient::SetClientCertificateMode(bool enabled)
{
  signals_->setclientcertificatemode_.Create(SetClientCertificateModeBody(enabled), enabled);
}

void DeviceClient::SetDiscoveryMode(DISCOVERYMODE discoverymode)
{
  signals_->setdiscoverymode_.Create(SetDiscoveryModeBody(discoverymode), discoverymode);
}

void DeviceClient::SetDNS(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual)
{
  signals_->setdns_.Create(SetDNSBody(fromdhcp, searchdomain, dnsmanual), fromdhcp, searchdomain, dnsmanual);
}

void DeviceClient::SetDot1XConfiguration(const Dot1XConfiguration& dot1xconfiguration)
{
  signals_->setdot1xconfiguration_.Create(SetDot1XConfigurationBody(dot1xconfiguration), dot1xconfiguration);
}

void DeviceClient::SetDPAddresses(const std::vector<NetworkHost>& dpaddress)
{
  signals_->setdpaddresses_.Create(SetDPAddressesBody(dpaddress), dpaddress);
}

void DeviceClient::SetDynamicDNS(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl)
{
  signals_->setdynamicdns_.Create(SetDynamicDNSBody(type, name, ttl), type, name, ttl);
}

void DeviceClient::SetHostname(const std::string& name)
{
  signals_->sethostname_.Create(SetHostnameBody(name), name);
}

void DeviceClient::SetHostnameFromDHCP(bool fromdhcp)
{
  signals_->sethostnamefromdhcp_.Create(SetHostnameFromDHCPBody(fromdhcp), fromdhcp);
}

void DeviceClient::SetIPAddressFilter(const IPAddressFilter& ipaddressfilter)
{
  signals_->setipaddressfilter_.Create(SetIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

void DeviceClient::SetNetworkDefaultGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address)
{
  signals_->setnetworkdefaultgateway_.Create(SetNetworkDefaultGatewayBody(ipv4address, ipv6address), ipv4address, ipv6address);
}

void DeviceClient::SetNetworkInterfaces(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface)
{
  signals_->setnetworkinterfaces_.Create(SetNetworkInterfacesBody(token, networkinterface), token, networkinterface);
}

void DeviceClient::SetNetworkProtocols(const std::vector<NetworkProtocol>& networkprotocols)
{
  signals_->setnetworkprotocols_.Create(SetNetworkProtocolsBody(networkprotocols), networkprotocols);
}

void DeviceClient::SetNTP(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual)
{
  signals_->setntp_.Create(SetNTPBody(fromdhcp, ntpmanual), fromdhcp, ntpmanual);
}

void DeviceClient::SetRelayOutputSettings(const std::string& relayoutputtoken, const RelayOutputSettings& properties)
{
  signals_->setrelayoutputsettings_.Create(SetRelayOutputSettingsBody(relayoutputtoken, properties), relayoutputtoken, properties);
}

void DeviceClient::SetRemoteDiscoveryMode(DISCOVERYMODE remotediscoverymode)
{
  signals_->setremotediscoverymode_.Create(SetRemoteDiscoveryModeBody(remotediscoverymode), remotediscoverymode);
}

void DeviceClient::SetSystemDateAndTime(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime)
{
  signals_->setsystemdateandtime_.Create(SetSystemDateAndTimeBody(datetimetype, daylightsavings, timezone, utcdatetime), datetimetype, daylightsavings, timezone, utcdatetime);
}

void DeviceClient::SetSystemFactoryDefault(FACTORYDEFAULTTYPE factorydefault)
{
  signals_->setsystemfactorydefault_.Create(SetSystemFactoryDefaultBody(factorydefault), factorydefault);
}

void DeviceClient::SetUser(const std::vector<User>& users)
{
  signals_->setuser_.Create(SetUserBody(users), users);
}

void DeviceClient::SetZeroConfiguration(const std::string& interfacetoken, bool enabled)
{
  signals_->setzeroconfiguration_.Create(SetZeroConfigurationBody(interfacetoken, enabled), interfacetoken, enabled);
}

void DeviceClient::SystemReboot()
{
  signals_->systemreboot_.Create(SystemRebootBody());
}

// Callbacks
Connection DeviceClient::AddIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const AddIPAddressFilterResponse&)> callback)
{
  return signals_->addipaddressfilter_.CreateCallback(AddIPAddressFilterBody(ipaddressfilter), callback, ipaddressfilter);
}

Connection DeviceClient::AddScopesCallback(const std::vector<std::string>& scopes, boost::function<void(const AddScopesResponse&)> callback)
{
  return signals_->addscopes_.CreateCallback(AddScopesBody(scopes), callback, scopes);
}

Connection DeviceClient::CreateCertificateCallback(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter, boost::function<void(const CreateCertificateResponse&)> callback)
{
  return signals_->createcertificate_.CreateCallback(CreateCertificateBody(certificateid, subject, validnotbefore, validnotafter), callback, certificateid, subject, validnotbefore, validnotafter);
}

Connection DeviceClient::CreateStorageConfigurationCallback(const StorageConfigurationData& storageconfiguration, boost::function<void(const CreateStorageConfigurationResponse&)> callback)
{
  return signals_->createstorageconfiguration_.CreateCallback(CreateStorageConfigurationBody(storageconfiguration), callback, storageconfiguration);
}

Connection DeviceClient::CreateUsersCallback(const std::vector<User>& users, boost::function<void(const CreateUsersResponse&)> callback)
{
  return signals_->createusers_.CreateCallback(CreateUsersBody(users), callback, users);
}

Connection DeviceClient::DeleteCertificatesCallback(const std::vector<std::string>& certificateid, boost::function<void(const DeleteCertificatesResponse&)> callback)
{
  return signals_->deletecertificates_.CreateCallback(DeleteCertificatesBody(certificateid), callback, certificateid);
}

Connection DeviceClient::DeleteDot1XConfigurationCallback(const std::vector<std::string>& dot1xconfigurationtoken, boost::function<void(const DeleteDot1XConfigurationResponse&)> callback)
{
  return signals_->deletedot1xconfiguration_.CreateCallback(DeleteDot1XConfigurationBody(dot1xconfigurationtoken), callback, dot1xconfigurationtoken);
}

Connection DeviceClient::DeleteStorageConfigurationCallback(const std::vector<std::string>& token, boost::function<void(const DeleteStorageConfigurationResponse&)> callback)
{
  return signals_->deletestorageconfiguration_.CreateCallback(DeleteStorageConfigurationBody(token), callback, token);
}

Connection DeviceClient::DeleteUsersCallback(const std::vector<std::string>& usernames, boost::function<void(const DeleteUsersResponse&)> callback)
{
  return signals_->deleteusers_.CreateCallback(DeleteUsersBody(usernames), callback, usernames);
}

Connection DeviceClient::GetAccessPolicyCallback(boost::function<void(const GetAccessPolicyResponse&)> callback)
{
  return signals_->getaccesspolicy_.CreateCallback(GetAccessPolicyBody(), callback);
}

Connection DeviceClient::GetCACertificatesCallback(boost::function<void(const GetCACertificatesResponse&)> callback)
{
  return signals_->getcacertificates_.CreateCallback(GetCACertificatesBody(), callback);
}

Connection DeviceClient::GetCapabilitiesCallback(CAPABILITYCATEGORY capabilitycategory, boost::function<void(const GetCapabilitiesResponse&)> callback)
{
  return signals_->getcapabilities_.CreateCallback(GetCapabilitiesBody(capabilitycategory), callback, capabilitycategory);
}

Connection DeviceClient::GetCertificateInformationCallback(const std::string& certificateid, boost::function<void(const GetCertificateInformationResponse&)> callback)
{
  return signals_->getcertificateinformation_.CreateCallback(GetCertificateInformationBody(certificateid), callback, certificateid);
}

Connection DeviceClient::GetCertificatesStatusCallback(boost::function<void(const GetCertificatesStatusResponse&)> callback)
{
  return signals_->getcertificatesstatus_.CreateCallback(GetCertificatesStatusBody(), callback);
}

Connection DeviceClient::GetClientCertificateModeCallback(boost::function<void(const GetClientCertificateModeResponse&)> callback)
{
  return signals_->getclientcertificatemode_.CreateCallback(GetClientCertificateModeBody(), callback);
}

Connection DeviceClient::GetCertificatesCallback(boost::function<void(const GetCertificatesResponse&)> callback)
{
  return signals_->getcertificates_.CreateCallback(GetCertificatesBody(), callback);
}

Connection DeviceClient::GetDeviceInformationCallback(boost::function<void(const GetDeviceInformationResponse&)> callback)
{
  return signals_->getdeviceinformation_.CreateCallback(GetDeviceInformationBody(), callback);
}

Connection DeviceClient::GetDiscoveryModeCallback(boost::function<void(const GetDiscoveryModeResponse&)> callback)
{
  return signals_->getdiscoverymode_.CreateCallback(GetDiscoveryModeBody(), callback);
}

Connection DeviceClient::GetDNSCallback(boost::function<void(const GetDNSResponse&)> callback)
{
  return signals_->getdns_.CreateCallback(GetDNSBody(), callback);
}

Connection DeviceClient::GetDot11CapabilitiesCallback(boost::function<void(const GetDot11CapabilitiesResponse&)> callback)
{
  return signals_->getdot11capabilities_.CreateCallback(GetDot11CapabilitiesBody(), callback);
}

Connection DeviceClient::GetDot11StatusCallback(const std::string& interfacetoken, boost::function<void(const GetDot11StatusResponse&)> callback)
{
  return signals_->getdot11status_.CreateCallback(GetDot11StatusBody(interfacetoken), callback, interfacetoken);
}

Connection DeviceClient::GetDot1XConfigurationCallback(const std::string& dot1xconfigurationtoken, boost::function<void(const GetDot1XConfigurationResponse&)> callback)
{
  return signals_->getdot1xconfiguration_.CreateCallback(GetDot1XConfigurationBody(dot1xconfigurationtoken), callback, dot1xconfigurationtoken);
}

Connection DeviceClient::GetDot1XConfigurationsCallback(boost::function<void(const GetDot1XConfigurationsResponse&)> callback)
{
  return signals_->getdot1xconfigurations_.CreateCallback(GetDot1XConfigurationsBody(), callback);
}

Connection DeviceClient::GetDPAddressesCallback(boost::function<void(const GetDPAddressesResponse&)> callback)
{
  return signals_->getdpaddresses_.CreateCallback(GetDPAddressesBody(), callback);
}

Connection DeviceClient::GetDynamicDNSCallback(boost::function<void(const GetDynamicDNSResponse&)> callback)
{
  return signals_->getdynamicdns_.CreateCallback(GetDynamicDNSBody(), callback);
}

Connection DeviceClient::GetEndpointReferenceCallback(boost::function<void(const GetEndpointReferenceResponse&)> callback)
{
  return signals_->getendpointreference_.CreateCallback(GetEndpointReferenceBody(), callback);
}

Connection DeviceClient::GetHostnameCallback(boost::function<void(const GetHostnameResponse&)> callback)
{
  return signals_->gethostname_.CreateCallback(GetHostnameBody(), callback);
}

Connection DeviceClient::GetIPAddressFilterCallback(boost::function<void(const GetIPAddressFilterResponse&)> callback)
{
  return signals_->getipaddressfilter_.CreateCallback(GetIPAddressFilterBody(), callback);
}

Connection DeviceClient::GetNetworkDefaultGatewayCallback(boost::function<void(const GetNetworkDefaultGatewayResponse&)> callback)
{
  return signals_->getnetworkdefaultgateway_.CreateCallback(GetNetworkDefaultGatewayBody(), callback);
}

Connection DeviceClient::GetNetworkInterfacesCallback(boost::function<void(const GetNetworkInterfacesResponse&)> callback)
{
  return signals_->getnetworkinterfaces_.CreateCallback(GetNetworkInterfacesBody(), callback);
}

Connection DeviceClient::GetNetworkProtocolsCallback(boost::function<void(const GetNetworkProtocolsResponse&)> callback)
{
  return signals_->getnetworkprotocols_.CreateCallback(GetNetworkProtocolsBody(), callback);
}

Connection DeviceClient::GetPkcs10RequestCallback(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes, boost::function<void(const GetPkcs10RequestResponse&)> callback)
{
  return signals_->getpkcs10request_.CreateCallback(GetPkcs10RequestBody(certificateid, subject, attributes), callback, certificateid, subject, attributes);
}

Connection DeviceClient::GetRelayOutputsCallback(boost::function<void(const GetRelayOutputsResponse&)> callback)
{
  return signals_->getrelayoutputs_.CreateCallback(GetRelayOutputsBody(), callback);
}

Connection DeviceClient::GetNTPCallback(boost::function<void(const GetNTPResponse&)> callback)
{
  return signals_->getntp_.CreateCallback(GetNTPBody(), callback);
}

Connection DeviceClient::GetRemoteDiscoveryModeCallback(boost::function<void(const GetRemoteDiscoveryModeResponse&)> callback)
{
  return signals_->getremotediscoverymode_.CreateCallback(GetRemoteDiscoveryModeBody(), callback);
}

Connection DeviceClient::GetRemoteUserCallback(boost::function<void(const GetRemoteUserResponse&)> callback)
{
  return signals_->getremoteuser_.CreateCallback(GetRemoteUserBody(), callback);
}

Connection DeviceClient::GetScopesCallback(boost::function<void(const GetScopesResponse&)> callback)
{
  return signals_->getscopes_.CreateCallback(GetScopesBody(), callback);
}

Connection DeviceClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection DeviceClient::GetServicesCallback(bool includecapability, boost::function<void(const GetServicesResponse&)> callback)
{
  return signals_->getservices_.CreateCallback(GetServicesBody(includecapability), callback, includecapability);
}

Connection DeviceClient::GetStorageConfigurationCallback(const std::string& token, boost::function<void(const GetStorageConfigurationResponse&)> callback)
{
  return signals_->getstorageconfiguration_.CreateCallback(GetStorageConfigurationBody(token), callback, token);
}

Connection DeviceClient::GetStorageConfigurationsCallback(boost::function<void(const GetStorageConfigurationsResponse&)> callback)
{
  return signals_->getstorageconfigurations_.CreateCallback(GetStorageConfigurationsBody(), callback);
}

Connection DeviceClient::GetSystemBackupCallback(boost::function<void(const GetSystemBackupResponse&)> callback)
{
  return signals_->getsystembackup_.CreateCallback(GetSystemBackupBody(), callback);
}

Connection DeviceClient::GetSystemDateAndTimeCallback(boost::function<void(const GetSystemDateAndTimeResponse&)> callback)
{
  return signals_->getsystemdateandtime_.CreateCallback(GetSystemDateAndTimeBody(), callback);
}

Connection DeviceClient::GetSystemLogCallback(SYSTEMLOGTYPE logtype, boost::function<void(const GetSystemLogResponse&)> callback)
{
  return signals_->getsystemlog_.CreateCallback(GetSystemLogBody(logtype), callback, logtype);
}

Connection DeviceClient::GetSystemSupportInformationCallback(boost::function<void(const GetSystemSupportInformationResponse&)> callback)
{
  return signals_->getsystemsupportinformation_.CreateCallback(GetSystemSupportInformationBody(), callback);
}

Connection DeviceClient::GetSystemUrisCallback(boost::function<void(const GetSystemUrisResponse&)> callback)
{
  return signals_->getsystemuris_.CreateCallback(GetSystemUrisBody(), callback);
}

Connection DeviceClient::GetUsersCallback(boost::function<void(const GetUsersResponse&)> callback)
{
  return signals_->getusers_.CreateCallback(GetUsersBody(), callback);
}

Connection DeviceClient::GetWsdlUrlCallback(boost::function<void(const GetWsdlUrlResponse&)> callback)
{
  return signals_->getwsdlurl_.CreateCallback(GetWsdlUrlBody(), callback);
}

Connection DeviceClient::GetZeroConfigurationCallback(boost::function<void(const GetZeroConfigurationResponse&)> callback)
{
  return signals_->getzeroconfiguration_.CreateCallback(GetZeroConfigurationBody(), callback);
}

Connection DeviceClient::LoadCACertificatesCallback(const std::vector<Certificate>& cacertificate, boost::function<void(const LoadCACertificatesResponse&)> callback)
{
  return signals_->loadcacertificates_.CreateCallback(LoadCACertificatesBody(cacertificate), callback, cacertificate);
}

Connection DeviceClient::LoadCertificatesCallback(const std::vector<Certificate>& nvtcertificate, boost::function<void(const LoadCertificatesResponse&)> callback)
{
  return signals_->loadcertificates_.CreateCallback(LoadCertificatesBody(nvtcertificate), callback, nvtcertificate);
}

Connection DeviceClient::LoadCertificateWithPrivateKeyCallback(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey, boost::function<void(const LoadCertificateWithPrivateKeyResponse&)> callback)
{
  return signals_->loadcertificatewithprivatekey_.CreateCallback(LoadCertificateWithPrivateKeyBody(certificatewithprivatekey), callback, certificatewithprivatekey);
}

Connection DeviceClient::RemoveIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const RemoveIPAddressFilterResponse&)> callback)
{
  return signals_->removeipaddressfilter_.CreateCallback(RemoveIPAddressFilterBody(ipaddressfilter), callback, ipaddressfilter);
}

Connection DeviceClient::RemoveScopesCallback(const std::vector<std::string>& scopes, boost::function<void(const RemoveScopesResponse&)> callback)
{
  return signals_->removescopes_.CreateCallback(RemoveScopesBody(scopes), callback, scopes);
}

Connection DeviceClient::RestoreSystemCallback(const std::vector<BackupFile>& backupfiles, boost::function<void(const RestoreSystemResponse&)> callback)
{
  const auto body = RestoreSystemBody(backupfiles);
  return signals_->restoresystem_.CreateCallback(body.first, body.second, callback, backupfiles);
}

Connection DeviceClient::ScanAvailableDot11NetworksCallback(const std::string& interfacetoken, boost::function<void(const ScanAvailableDot11NetworksResponse&)> callback)
{
  return signals_->scanavailabledot11networks_.CreateCallback(ScanAvailableDot11NetworksBody(interfacetoken), callback, interfacetoken);
}

Connection DeviceClient::SendAuxiliaryCommandCallback(const std::string& auxiliarycommand, boost::function<void(const SendAuxiliaryCommandResponse&)> callback)
{
  return signals_->sendauxiliarycommand_.CreateCallback(SendAuxiliaryCommandBody(auxiliarycommand), callback, auxiliarycommand);
}

Connection DeviceClient::SetAccessPolicyCallback(const BinaryData& policyfile, boost::function<void(const SetAccessPolicyResponse&)> callback)
{
  return signals_->setaccesspolicy_.CreateCallback(SetAccessPolicyBody(policyfile), callback, policyfile);
}

Connection DeviceClient::SetCertificatesStatusCallback(const std::vector<CertificateStatus>& certificatesstatus, boost::function<void(const SetCertificatesStatusResponse&)> callback)
{
  return signals_->setcertificatesstatus_.CreateCallback(SetCertificatesStatusBody(certificatesstatus), callback, certificatesstatus);
}

Connection DeviceClient::SetClientCertificateModeCallback(bool enabled, boost::function<void(const SetClientCertificateModeResponse&)> callback)
{
  return signals_->setclientcertificatemode_.CreateCallback(SetClientCertificateModeBody(enabled), callback, enabled);
}

Connection DeviceClient::SetDiscoveryModeCallback(DISCOVERYMODE discoverymode, boost::function<void(const SetDiscoveryModeResponse&)> callback)
{
  return signals_->setdiscoverymode_.CreateCallback(SetDiscoveryModeBody(discoverymode), callback, discoverymode);
}

Connection DeviceClient::SetDNSCallback(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual, boost::function<void(const SetDNSResponse&)> callback)
{
  return signals_->setdns_.CreateCallback(SetDNSBody(fromdhcp, searchdomain, dnsmanual), callback, fromdhcp, searchdomain, dnsmanual);
}

Connection DeviceClient::SetDot1XConfigurationCallback(const Dot1XConfiguration& dot1xconfiguration, boost::function<void(const SetDot1XConfigurationResponse&)> callback)
{
  return signals_->setdot1xconfiguration_.CreateCallback(SetDot1XConfigurationBody(dot1xconfiguration), callback, dot1xconfiguration);
}

Connection DeviceClient::SetDPAddressesCallback(const std::vector<NetworkHost>& dpaddress, boost::function<void(const SetDPAddressesResponse&)> callback)
{
  return signals_->setdpaddresses_.CreateCallback(SetDPAddressesBody(dpaddress), callback, dpaddress);
}

Connection DeviceClient::SetDynamicDNSCallback(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl, boost::function<void(const SetDynamicDNSResponse&)> callback)
{
  return signals_->setdynamicdns_.CreateCallback(SetDynamicDNSBody(type, name, ttl), callback, type, name, ttl);
}

Connection DeviceClient::SetHostnameCallback(const std::string& name, boost::function<void(const SetHostnameResponse&)> callback)
{
  return signals_->sethostname_.CreateCallback(SetHostnameBody(name), callback, name);
}

Connection DeviceClient::SetHostnameFromDHCPCallback(bool fromdhcp, boost::function<void(const SetHostnameFromDHCPResponse&)> callback)
{
  return signals_->sethostnamefromdhcp_.CreateCallback(SetHostnameFromDHCPBody(fromdhcp), callback, fromdhcp);
}

Connection DeviceClient::SetIPAddressFilterCallback(const IPAddressFilter& ipaddressfilter, boost::function<void(const SetIPAddressFilterResponse&)> callback)
{
  return signals_->setipaddressfilter_.CreateCallback(SetIPAddressFilterBody(ipaddressfilter), callback, ipaddressfilter);
}

Connection DeviceClient::SetNetworkDefaultGatewayCallback(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address, boost::function<void(const SetNetworkDefaultGatewayResponse&)> callback)
{
  return signals_->setnetworkdefaultgateway_.CreateCallback(SetNetworkDefaultGatewayBody(ipv4address, ipv6address), callback, ipv4address, ipv6address);
}

Connection DeviceClient::SetNetworkInterfacesCallback(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface, boost::function<void(const SetNetworkInterfacesResponse&)> callback)
{
  return signals_->setnetworkinterfaces_.CreateCallback(SetNetworkInterfacesBody(token, networkinterface), callback, token, networkinterface);
}

Connection DeviceClient::SetNetworkProtocolsCallback(const std::vector<NetworkProtocol>& networkprotocols, boost::function<void(const SetNetworkProtocolsResponse&)> callback)
{
  return signals_->setnetworkprotocols_.CreateCallback(SetNetworkProtocolsBody(networkprotocols), callback, networkprotocols);
}

Connection DeviceClient::SetNTPCallback(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual, boost::function<void(const SetNTPResponse&)> callback)
{
  return signals_->setntp_.CreateCallback(SetNTPBody(fromdhcp, ntpmanual), callback, fromdhcp, ntpmanual);
}

Connection DeviceClient::SetRelayOutputSettingsCallback(const std::string& relayoutputtoken, const RelayOutputSettings& properties, boost::function<void(const SetRelayOutputSettingsResponse&)> callback)
{
  return signals_->setrelayoutputsettings_.CreateCallback(SetRelayOutputSettingsBody(relayoutputtoken, properties), callback, relayoutputtoken, properties);
}

Connection DeviceClient::SetRemoteDiscoveryModeCallback(DISCOVERYMODE remotediscoverymode, boost::function<void(const SetRemoteDiscoveryModeResponse&)> callback)
{
  return signals_->setremotediscoverymode_.CreateCallback(SetRemoteDiscoveryModeBody(remotediscoverymode), callback, remotediscoverymode);
}

Connection DeviceClient::SetSystemDateAndTimeCallback(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime, boost::function<void(const SetSystemDateAndTimeResponse&)> callback)
{
  return signals_->setsystemdateandtime_.CreateCallback(SetSystemDateAndTimeBody(datetimetype, daylightsavings, timezone, utcdatetime), callback, datetimetype, daylightsavings, timezone, utcdatetime);
}

Connection DeviceClient::SetSystemFactoryDefaultCallback(FACTORYDEFAULTTYPE factorydefault, boost::function<void(const SetSystemFactoryDefaultResponse&)> callback)
{
  return signals_->setsystemfactorydefault_.CreateCallback(SetSystemFactoryDefaultBody(factorydefault), callback, factorydefault);
}

Connection DeviceClient::SetUserCallback(const std::vector<User>& users, boost::function<void(const SetUserResponse&)> callback)
{
  return signals_->setuser_.CreateCallback(SetUserBody(users), callback, users);
}

Connection DeviceClient::SetZeroConfigurationCallback(const std::string& interfacetoken, bool enabled, boost::function<void(const SetZeroConfigurationResponse&)> callback)
{
  return signals_->setzeroconfiguration_.CreateCallback(SetZeroConfigurationBody(interfacetoken, enabled), callback, interfacetoken, enabled);
}

Connection DeviceClient::SystemRebootCallback(boost::function<void(const SystemRebootResponse&)> callback)
{
  return signals_->systemreboot_.CreateCallback(SystemRebootBody(), callback);
}

// Futures
boost::unique_future<AddIPAddressFilterResponse> DeviceClient::AddIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter)
{
  return signals_->addipaddressfilter_.CreateFuture(AddIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

boost::unique_future<AddScopesResponse> DeviceClient::AddScopesFuture(const std::vector<std::string>& scopes)
{
  return signals_->addscopes_.CreateFuture(AddScopesBody(scopes), scopes);
}

boost::unique_future<CreateCertificateResponse> DeviceClient::CreateCertificateFuture(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter)
{
  return signals_->createcertificate_.CreateFuture(CreateCertificateBody(certificateid, subject, validnotbefore, validnotafter), certificateid, subject, validnotbefore, validnotafter);
}

boost::unique_future<CreateStorageConfigurationResponse> DeviceClient::CreateStorageConfigurationFuture(const StorageConfigurationData& storageconfiguration)
{
  return signals_->createstorageconfiguration_.CreateFuture(CreateStorageConfigurationBody(storageconfiguration), storageconfiguration);
}

boost::unique_future<CreateUsersResponse> DeviceClient::CreateUsersFuture(const std::vector<User>& users)
{
  return signals_->createusers_.CreateFuture(CreateUsersBody(users), users);
}

boost::unique_future<DeleteCertificatesResponse> DeviceClient::DeleteCertificatesFuture(const std::vector<std::string>& certificateid)
{
  return signals_->deletecertificates_.CreateFuture(DeleteCertificatesBody(certificateid), certificateid);
}

boost::unique_future<DeleteDot1XConfigurationResponse> DeviceClient::DeleteDot1XConfigurationFuture(const std::vector<std::string>& dot1xconfigurationtoken)
{
  return signals_->deletedot1xconfiguration_.CreateFuture(DeleteDot1XConfigurationBody(dot1xconfigurationtoken), dot1xconfigurationtoken);
}

boost::unique_future<DeleteStorageConfigurationResponse> DeviceClient::DeleteStorageConfigurationFuture(const std::vector<std::string>& token)
{
  return signals_->deletestorageconfiguration_.CreateFuture(DeleteStorageConfigurationBody(token), token);
}

boost::unique_future<DeleteUsersResponse> DeviceClient::DeleteUsersFuture(const std::vector<std::string>& usernames)
{
  return signals_->deleteusers_.CreateFuture(DeleteUsersBody(usernames), usernames);
}

boost::unique_future<GetAccessPolicyResponse> DeviceClient::GetAccessPolicyFuture()
{
  return signals_->getaccesspolicy_.CreateFuture(GetAccessPolicyBody());
}

boost::unique_future<GetCACertificatesResponse> DeviceClient::GetCACertificatesFuture()
{
  return signals_->getcacertificates_.CreateFuture(GetCACertificatesBody());
}

boost::unique_future<GetCapabilitiesResponse> DeviceClient::GetCapabilitiesFuture(CAPABILITYCATEGORY capabilitycategory)
{
  return signals_->getcapabilities_.CreateFuture(GetCapabilitiesBody(capabilitycategory), capabilitycategory);
}

boost::unique_future<GetCertificateInformationResponse> DeviceClient::GetCertificateInformationFuture(const std::string& certificateid)
{
  return signals_->getcertificateinformation_.CreateFuture(GetCertificateInformationBody(certificateid), certificateid);
}

boost::unique_future<GetCertificatesResponse> DeviceClient::GetCertificatesFuture()
{
  return signals_->getcertificates_.CreateFuture(GetCertificatesBody());
}

boost::unique_future<GetCertificatesStatusResponse> DeviceClient::GetCertificatesStatusFuture()
{
  return signals_->getcertificatesstatus_.CreateFuture(GetCertificatesStatusBody());
}

boost::unique_future<GetClientCertificateModeResponse> DeviceClient::GetClientCertificateModeFuture()
{
  return signals_->getclientcertificatemode_.CreateFuture(GetClientCertificateModeBody());
}

boost::unique_future<GetDeviceInformationResponse> DeviceClient::GetDeviceInformationFuture()
{
  return signals_->getdeviceinformation_.CreateFuture(GetDeviceInformationBody());
}

boost::unique_future<GetDiscoveryModeResponse> DeviceClient::GetDiscoveryModeFuture()
{
  return signals_->getdiscoverymode_.CreateFuture(GetDiscoveryModeBody());
}

boost::unique_future<GetDNSResponse> DeviceClient::GetDNSFuture()
{
  return signals_->getdns_.CreateFuture(GetDNSBody());
}

boost::unique_future<GetDot11CapabilitiesResponse> DeviceClient::GetDot11CapabilitiesFuture()
{
  return signals_->getdot11capabilities_.CreateFuture(GetDot11CapabilitiesBody());
}

boost::unique_future<GetDot11StatusResponse> DeviceClient::GetDot11StatusFuture(const std::string& interfacetoken)
{
  return signals_->getdot11status_.CreateFuture(GetDot11StatusBody(interfacetoken), interfacetoken);
}

boost::unique_future<GetDot1XConfigurationResponse> DeviceClient::GetDot1XConfigurationFuture(const std::string& dot1xconfigurationtoken)
{
  return signals_->getdot1xconfiguration_.CreateFuture(GetDot1XConfigurationBody(dot1xconfigurationtoken), dot1xconfigurationtoken);
}

boost::unique_future<GetDot1XConfigurationsResponse> DeviceClient::GetDot1XConfigurationsFuture()
{
  return signals_->getdot1xconfigurations_.CreateFuture(GetDot1XConfigurationsBody());
}

boost::unique_future<GetDPAddressesResponse> DeviceClient::GetDPAddressesFuture()
{
  return signals_->getdpaddresses_.CreateFuture(GetDPAddressesBody());
}

boost::unique_future<GetDynamicDNSResponse> DeviceClient::GetDynamicDNSFuture()
{
  return signals_->getdynamicdns_.CreateFuture(GetDynamicDNSBody());
}

boost::unique_future<GetEndpointReferenceResponse> DeviceClient::GetEndpointReferenceFuture()
{
  return signals_->getendpointreference_.CreateFuture(GetEndpointReferenceBody());
}

boost::unique_future<GetHostnameResponse> DeviceClient::GetHostnameFuture()
{
  return signals_->gethostname_.CreateFuture(GetHostnameBody());
}

boost::unique_future<GetIPAddressFilterResponse> DeviceClient::GetIPAddressFilterFuture()
{
  return signals_->getipaddressfilter_.CreateFuture(GetIPAddressFilterBody());
}

boost::unique_future<GetNetworkDefaultGatewayResponse> DeviceClient::GetNetworkDefaultGatewayFuture()
{
  return signals_->getnetworkdefaultgateway_.CreateFuture(GetNetworkDefaultGatewayBody());
}

boost::unique_future<GetNetworkInterfacesResponse> DeviceClient::GetNetworkInterfacesFuture()
{
  return signals_->getnetworkinterfaces_.CreateFuture(GetNetworkInterfacesBody());
}

boost::unique_future<GetNetworkProtocolsResponse> DeviceClient::GetNetworkProtocolsFuture()
{
  return signals_->getnetworkprotocols_.CreateFuture(GetNetworkProtocolsBody());
}

boost::unique_future<GetPkcs10RequestResponse> DeviceClient::GetPkcs10RequestFuture(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes)
{
  return signals_->getpkcs10request_.CreateFuture(GetPkcs10RequestBody(certificateid, subject, attributes), certificateid, subject, attributes);
}

boost::unique_future<GetRelayOutputsResponse> DeviceClient::GetRelayOutputsFuture()
{
  return signals_->getrelayoutputs_.CreateFuture(GetRelayOutputsBody());
}

boost::unique_future<GetNTPResponse> DeviceClient::GetNTPFuture()
{
  return signals_->getntp_.CreateFuture(GetNTPBody());
}

boost::unique_future<GetRemoteDiscoveryModeResponse> DeviceClient::GetRemoteDiscoveryModeFuture()
{
  return signals_->getremotediscoverymode_.CreateFuture(GetRemoteDiscoveryModeBody());
}

boost::unique_future<GetRemoteUserResponse> DeviceClient::GetRemoteUserFuture()
{
  return signals_->getremoteuser_.CreateFuture(GetRemoteUserBody());
}

boost::unique_future<GetScopesResponse> DeviceClient::GetScopesFuture()
{
  return signals_->getscopes_.CreateFuture(GetScopesBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> DeviceClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<GetServicesResponse> DeviceClient::GetServicesFuture(bool includecapability)
{
  return signals_->getservices_.CreateFuture(GetServicesBody(includecapability), includecapability);
}

boost::unique_future<GetStorageConfigurationResponse> DeviceClient::GetStorageConfigurationFuture(const std::string& token)
{
  return signals_->getstorageconfiguration_.CreateFuture(GetStorageConfigurationBody(token), token);
}

boost::unique_future<GetStorageConfigurationsResponse> DeviceClient::GetStorageConfigurationsFuture()
{
  return signals_->getstorageconfigurations_.CreateFuture(GetStorageConfigurationsBody());
}

boost::unique_future<GetSystemBackupResponse> DeviceClient::GetSystemBackupFuture()
{
  return signals_->getsystembackup_.CreateFuture(GetSystemBackupBody());
}

boost::unique_future<GetSystemDateAndTimeResponse> DeviceClient::GetSystemDateAndTimeFuture()
{
  return signals_->getsystemdateandtime_.CreateFuture(GetSystemDateAndTimeBody());
}

boost::unique_future<GetSystemLogResponse> DeviceClient::GetSystemLogFuture(SYSTEMLOGTYPE logtype)
{
  return signals_->getsystemlog_.CreateFuture(GetSystemLogBody(logtype), logtype);
}

boost::unique_future<GetSystemSupportInformationResponse> DeviceClient::GetSystemSupportInformationFuture()
{
  return signals_->getsystemsupportinformation_.CreateFuture(GetSystemSupportInformationBody());
}

boost::unique_future<GetSystemUrisResponse> DeviceClient::GetSystemUrisFuture()
{
  return signals_->getsystemuris_.CreateFuture(GetSystemUrisBody());
}

boost::unique_future<GetUsersResponse> DeviceClient::GetUsersFuture()
{
  return signals_->getusers_.CreateFuture(GetUsersBody());
}

boost::unique_future<GetWsdlUrlResponse> DeviceClient::GetWsdlUrlFuture()
{
  return signals_->getwsdlurl_.CreateFuture(GetWsdlUrlBody());
}

boost::unique_future<GetZeroConfigurationResponse> DeviceClient::GetZeroConfigurationFuture()
{
  return signals_->getzeroconfiguration_.CreateFuture(GetZeroConfigurationBody());
}

boost::unique_future<LoadCACertificatesResponse> DeviceClient::LoadCACertificatesFuture(const std::vector<Certificate>& cacertificate)
{
  return signals_->loadcacertificates_.CreateFuture(LoadCACertificatesBody(cacertificate), cacertificate);
}

boost::unique_future<LoadCertificatesResponse> DeviceClient::LoadCertificatesFuture(const std::vector<Certificate>& nvtcertificate)
{
  return signals_->loadcertificates_.CreateFuture(LoadCertificatesBody(nvtcertificate), nvtcertificate);
}

boost::unique_future<LoadCertificateWithPrivateKeyResponse> DeviceClient::LoadCertificateWithPrivateKeyFuture(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey)
{
  return signals_->loadcertificatewithprivatekey_.CreateFuture(LoadCertificateWithPrivateKeyBody(certificatewithprivatekey), certificatewithprivatekey);
}

boost::unique_future<RemoveIPAddressFilterResponse> DeviceClient::RemoveIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter)
{
  return signals_->removeipaddressfilter_.CreateFuture(RemoveIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

boost::unique_future<RemoveScopesResponse> DeviceClient::RemoveScopesFuture(const std::vector<std::string>& scopes)
{
  return signals_->removescopes_.CreateFuture(RemoveScopesBody(scopes), scopes);
}

boost::unique_future<RestoreSystemResponse> DeviceClient::RestoreSystemFuture(const std::vector<BackupFile>& backupfiles)
{
  const auto body = RestoreSystemBody(backupfiles);
  return signals_->restoresystem_.CreateFuture(body.first, body.second, backupfiles);
}

boost::unique_future<ScanAvailableDot11NetworksResponse> DeviceClient::ScanAvailableDot11NetworksFuture(const std::string& interfacetoken)
{
  return signals_->scanavailabledot11networks_.CreateFuture(ScanAvailableDot11NetworksBody(interfacetoken), interfacetoken);
}

boost::unique_future<SendAuxiliaryCommandResponse> DeviceClient::SendAuxiliaryCommandFuture(const std::string& auxiliarycommand)
{
  return signals_->sendauxiliarycommand_.CreateFuture(SendAuxiliaryCommandBody(auxiliarycommand), auxiliarycommand);
}

boost::unique_future<SetAccessPolicyResponse> DeviceClient::SetAccessPolicyFuture(const BinaryData& policyfile)
{
  return signals_->setaccesspolicy_.CreateFuture(SetAccessPolicyBody(policyfile), policyfile);
}

boost::unique_future<SetCertificatesStatusResponse> DeviceClient::SetCertificatesStatusFuture(const std::vector<CertificateStatus>& certificatestatus)
{
  return signals_->setcertificatesstatus_.CreateFuture(SetCertificatesStatusBody(certificatestatus), certificatestatus);
}

boost::unique_future<SetClientCertificateModeResponse> DeviceClient::SetClientCertificateModeFuture(bool enabled)
{
  return signals_->setclientcertificatemode_.CreateFuture(SetClientCertificateModeBody(enabled), enabled);
}

boost::unique_future<SetDiscoveryModeResponse> DeviceClient::SetDiscoveryModeFuture(DISCOVERYMODE discoverymode)
{
  return signals_->setdiscoverymode_.CreateFuture(SetDiscoveryModeBody(discoverymode), discoverymode);
}

boost::unique_future<SetDNSResponse> DeviceClient::SetDNSFuture(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual)
{
  return signals_->setdns_.CreateFuture(SetDNSBody(fromdhcp, searchdomain, dnsmanual), fromdhcp, searchdomain, dnsmanual);
}

boost::unique_future<SetDot1XConfigurationResponse> DeviceClient::SetDot1XConfigurationFuture(const Dot1XConfiguration& dot1xconfiguration)
{
  return signals_->setdot1xconfiguration_.CreateFuture(SetDot1XConfigurationBody(dot1xconfiguration), dot1xconfiguration);
}

boost::unique_future<SetDPAddressesResponse> DeviceClient::SetDPAddressesFuture(const std::vector<NetworkHost>& dpaddress)
{
  return signals_->setdpaddresses_.CreateFuture(SetDPAddressesBody(dpaddress), dpaddress);
}

boost::unique_future<SetDynamicDNSResponse> DeviceClient::SetDynamicDNSFuture(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl)
{
  return signals_->setdynamicdns_.CreateFuture(SetDynamicDNSBody(type, name, ttl), type, name, ttl);
}

boost::unique_future<SetHostnameResponse> DeviceClient::SetHostnameFuture(const std::string& name)
{
  return signals_->sethostname_.CreateFuture(SetHostnameBody(name), name);
}

boost::unique_future<SetHostnameFromDHCPResponse> DeviceClient::SetHostnameFromDHCPFuture(bool fromdhcp)
{
  return signals_->sethostnamefromdhcp_.CreateFuture(SetHostnameFromDHCPBody(fromdhcp), fromdhcp);
}

boost::unique_future<SetIPAddressFilterResponse> DeviceClient::SetIPAddressFilterFuture(const IPAddressFilter& ipaddressfilter)
{
  return signals_->setipaddressfilter_.CreateFuture(SetIPAddressFilterBody(ipaddressfilter), ipaddressfilter);
}

boost::unique_future<SetNetworkDefaultGatewayResponse> DeviceClient::SetNetworkDefaultGatewayFuture(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address)
{
  return signals_->setnetworkdefaultgateway_.CreateFuture(SetNetworkDefaultGatewayBody(ipv4address, ipv6address), ipv4address, ipv6address);
}

boost::unique_future<SetNetworkInterfacesResponse> DeviceClient::SetNetworkInterfacesFuture(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface)
{
  return signals_->setnetworkinterfaces_.CreateFuture(SetNetworkInterfacesBody(token, networkinterface), token, networkinterface);
}

boost::unique_future<SetNetworkProtocolsResponse> DeviceClient::SetNetworkProtocolsFuture(const std::vector<NetworkProtocol>& networkprotocols)
{
  return signals_->setnetworkprotocols_.CreateFuture(SetNetworkProtocolsBody(networkprotocols), networkprotocols);
}

boost::unique_future<SetNTPResponse> DeviceClient::SetNTPFuture(bool dhcp, const std::vector<NetworkHost>& ntpmanual)
{
  return signals_->setntp_.CreateFuture(SetNTPBody(dhcp, ntpmanual), dhcp, ntpmanual);
}

boost::unique_future<SetRelayOutputSettingsResponse> DeviceClient::SetRelayOutputSettingsFuture(const std::string& relayoutputtoken, const RelayOutputSettings& properties)
{
  return signals_->setrelayoutputsettings_.CreateFuture(SetRelayOutputSettingsBody(relayoutputtoken, properties), relayoutputtoken, properties);
}

boost::unique_future<SetRemoteDiscoveryModeResponse> DeviceClient::SetRemoteDiscoveryModeFuture(DISCOVERYMODE remotediscoverymode)
{
  return signals_->setremotediscoverymode_.CreateFuture(SetRemoteDiscoveryModeBody(remotediscoverymode), remotediscoverymode);
}

boost::unique_future<SetSystemDateAndTimeResponse> DeviceClient::SetSystemDateAndTimeFuture(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime)
{
  return signals_->setsystemdateandtime_.CreateFuture(SetSystemDateAndTimeBody(datetimetype, daylightsavings, timezone, utcdatetime), datetimetype, daylightsavings, timezone, utcdatetime);
}

boost::unique_future<SetSystemFactoryDefaultResponse> DeviceClient::SetSystemFactoryDefaultFuture(FACTORYDEFAULTTYPE factorydefault)
{
  return signals_->setsystemfactorydefault_.CreateFuture(SetSystemFactoryDefaultBody(factorydefault), factorydefault);
}

boost::unique_future<SetUserResponse> DeviceClient::SetUserFuture(const std::vector<User>& users)
{
  return signals_->setuser_.CreateFuture(SetUserBody(users), users);
}

boost::unique_future<SetZeroConfigurationResponse> DeviceClient::SetZeroConfigurationFuture(const std::string& interfacetoken, bool enabled)
{
  return signals_->setzeroconfiguration_.CreateFuture(SetZeroConfigurationBody(interfacetoken, enabled), interfacetoken, enabled);
}

boost::unique_future<SystemRebootResponse> DeviceClient::SystemRebootFuture()
{
  return signals_->systemreboot_.CreateFuture(SystemRebootBody());
}

// Signals
boost::signals2::signal<void(const AddIPAddressFilterResponse&)>& DeviceClient::AddIPAddressFilterSignal()
{
  return signals_->addipaddressfilter_.GetSignal();
}

boost::signals2::signal<void(const AddScopesResponse&)>& DeviceClient::AddScopesSignal()
{
  return signals_->addscopes_.GetSignal();
}

boost::signals2::signal<void(const CreateCertificateResponse&)>& DeviceClient::CreateCertificateSignal()
{
  return signals_->createcertificate_.GetSignal();
}

boost::signals2::signal<void(const CreateUsersResponse&)>& DeviceClient::GetCreateUsersSignal()
{
  return signals_->createusers_.GetSignal();
}

boost::signals2::signal<void(const DeleteCertificatesResponse&)>& DeviceClient::GetDeleteCertificatesSignal()
{
  return signals_->deletecertificates_.GetSignal();
}

boost::signals2::signal<void(const DeleteDot1XConfigurationResponse&)>& DeviceClient::GetDeleteDot1XConfigurationSignal()
{
  return signals_->deletedot1xconfiguration_.GetSignal();
}

boost::signals2::signal<void(const DeleteStorageConfigurationResponse&)>& DeviceClient::GetDeleteStorageConfigurationSignal()
{
  return signals_->deletestorageconfiguration_.GetSignal();
}

boost::signals2::signal<void(const DeleteUsersResponse&)>& DeviceClient::GetDeleteUsersSignal()
{
  return signals_->deleteusers_.GetSignal();
}

boost::signals2::signal<void(const GetAccessPolicyResponse&)>& DeviceClient::GetAccessPolicySignal()
{
  return signals_->getaccesspolicy_.GetSignal();
}

boost::signals2::signal<void(const GetCACertificatesResponse&)>& DeviceClient::GetCACertificatesSignal()
{
  return signals_->getcacertificates_.GetSignal();
}

boost::signals2::signal<void(const GetCapabilitiesResponse&)>& DeviceClient::GetCapabilitiesSignal()
{
  return signals_->getcapabilities_.GetSignal();
}

boost::signals2::signal<void(const GetCertificateInformationResponse&)>& DeviceClient::GetCertificateInformationSignal()
{
  return signals_->getcertificateinformation_.GetSignal();
}

boost::signals2::signal<void(const GetCertificatesResponse&)>& DeviceClient::GetCertificatesSignal()
{
  return signals_->getcertificates_.GetSignal();
}

boost::signals2::signal<void(const GetCertificatesStatusResponse&)>& DeviceClient::GetCertificatesStatusSignal()
{
  return signals_->getcertificatesstatus_.GetSignal();
}

boost::signals2::signal<void(const GetClientCertificateModeResponse&)>& DeviceClient::GetClientCertificateModeSignal()
{
  return signals_->getclientcertificatemode_.GetSignal();
}

boost::signals2::signal<void(const GetDeviceInformationResponse&)>& DeviceClient::GetDeviceInformationSignal()
{
  return signals_->getdeviceinformation_.GetSignal();
}

boost::signals2::signal<void(const GetDiscoveryModeResponse&)>& DeviceClient::GetDiscoveryModeSignal()
{
  return signals_->getdiscoverymode_.GetSignal();
}

boost::signals2::signal<void(const GetDNSResponse&)>& DeviceClient::GetDNSSignal()
{
  return signals_->getdns_.GetSignal();
}

boost::signals2::signal<void(const GetDot11CapabilitiesResponse&)>& DeviceClient::GetDot11CapabilitiesSignal()
{
  return signals_->getdot11capabilities_.GetSignal();
}

boost::signals2::signal<void(const GetDot11StatusResponse&)>& DeviceClient::GetDot11StatusSignal()
{
  return signals_->getdot11status_.GetSignal();
}

boost::signals2::signal<void(const GetDot1XConfigurationResponse&)>& DeviceClient::GetDot1XConfigurationSignal()
{
  return signals_->getdot1xconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetDot1XConfigurationsResponse&)>& DeviceClient::GetDot1XConfigurationsSignal()
{
  return signals_->getdot1xconfigurations_.GetSignal();
}

boost::signals2::signal<void(const GetDPAddressesResponse&)>& DeviceClient::GetDPAddressesSignal()
{
  return signals_->getdpaddresses_.GetSignal();
}

boost::signals2::signal<void(const GetDynamicDNSResponse&)>& DeviceClient::GetDynamicDNSSignal()
{
  return signals_->getdynamicdns_.GetSignal();
}

boost::signals2::signal<void(const GetEndpointReferenceResponse&)>& DeviceClient::GetEndpointReferenceSignal()
{
  return signals_->getendpointreference_.GetSignal();
}

boost::signals2::signal<void(const GetHostnameResponse&)>& DeviceClient::GetHsotnameSignal()
{
  return signals_->gethostname_.GetSignal();
}

boost::signals2::signal<void(const GetIPAddressFilterResponse&)>& DeviceClient::GetIPAddressFilterSignal()
{
  return signals_->getipaddressfilter_.GetSignal();
}

boost::signals2::signal<void(const GetNetworkDefaultGatewayResponse&)>& DeviceClient::GetNetworkDefaultGatewaySignal()
{
  return signals_->getnetworkdefaultgateway_.GetSignal();
}

boost::signals2::signal<void(const GetNetworkInterfacesResponse&)>& DeviceClient::GetNetworkInterfacesSignal()
{
  return signals_->getnetworkinterfaces_.GetSignal();
}

boost::signals2::signal<void(const GetNetworkProtocolsResponse&)>& DeviceClient::GetNetworkProtocolsSignal()
{
  return signals_->getnetworkprotocols_.GetSignal();
}

boost::signals2::signal<void(const GetPkcs10RequestResponse&)>& DeviceClient::GetPkcs10RequestSignal()
{
  return signals_->getpkcs10request_.GetSignal();
}

boost::signals2::signal<void(const GetRelayOutputsResponse&)>& DeviceClient::GetRelayOutputsSignal()
{
  return signals_->getrelayoutputs_.GetSignal();
}

boost::signals2::signal<void(const GetNTPResponse&)>& DeviceClient::GetNTPSignal()
{
  return signals_->getntp_.GetSignal();
}

boost::signals2::signal<void(const GetRemoteDiscoveryModeResponse&)>& DeviceClient::GetRemoteDiscoveryModeSignal()
{
  return signals_->getremotediscoverymode_.GetSignal();
}

boost::signals2::signal<void(const GetRemoteUserResponse&)>& DeviceClient::GetRemoteUserSignal()
{
  return signals_->getremoteuser_.GetSignal();
}

boost::signals2::signal<void(const GetScopesResponse&)>& DeviceClient::GetScopesSignal()
{
  return signals_->getscopes_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& DeviceClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const GetServicesResponse&)>& DeviceClient::GetServicesSignal()
{
  return signals_->getservices_.GetSignal();
}

boost::signals2::signal<void(const GetStorageConfigurationResponse&)>& DeviceClient::GetStorageConfigurationSignal()
{
  return signals_->getstorageconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetStorageConfigurationsResponse&)>& DeviceClient::GetStorageConfigurationsSignal()
{
  return signals_->getstorageconfigurations_.GetSignal();
}

boost::signals2::signal<void(const GetSystemBackupResponse&)>& DeviceClient::GetSystemBackupSignal()
{
  return signals_->getsystembackup_.GetSignal();
}

boost::signals2::signal<void(const GetSystemDateAndTimeResponse&)>& DeviceClient::GetSystemDateAndTimeSignal()
{
  return signals_->getsystemdateandtime_.GetSignal();
}

boost::signals2::signal<void(const GetSystemLogResponse&)>& DeviceClient::GetSystemLogSignal()
{
  return signals_->getsystemlog_.GetSignal();
}

boost::signals2::signal<void(const GetSystemSupportInformationResponse&)>& DeviceClient::GetSystemSupportInformationSignal()
{
  return signals_->getsystemsupportinformation_.GetSignal();
}

boost::signals2::signal<void(const GetSystemUrisResponse&)>& DeviceClient::GetSystemUrisSignal()
{
  return signals_->getsystemuris_.GetSignal();
}

boost::signals2::signal<void(const GetUsersResponse&)>& DeviceClient::GetUsersSignal()
{
  return signals_->getusers_.GetSignal();
}

boost::signals2::signal<void(const GetWsdlUrlResponse&)>& DeviceClient::GetWsdlUrlSignal()
{
  return signals_->getwsdlurl_.GetSignal();
}

boost::signals2::signal<void(const GetZeroConfigurationResponse&)>& DeviceClient::GetZeroConfigurationSignal()
{
  return signals_->getzeroconfiguration_.GetSignal();
}

boost::signals2::signal<void(const LoadCACertificatesResponse&)>& DeviceClient::LoadCACertificatesSignal()
{
  return signals_->loadcacertificates_.GetSignal();
}

boost::signals2::signal<void(const LoadCertificatesResponse&)>& DeviceClient::LoadCertificatesSignal()
{
  return signals_->loadcertificates_.GetSignal();
}

boost::signals2::signal<void(const LoadCertificateWithPrivateKeyResponse&)>& DeviceClient::LoadCertificateWithPrivateKeySignal()
{
  return signals_->loadcertificatewithprivatekey_.GetSignal();
}

boost::signals2::signal<void(const RemoveIPAddressFilterResponse&)>& DeviceClient::RemoveIPAddressFilterSignal()
{
  return signals_->removeipaddressfilter_.GetSignal();
}

boost::signals2::signal<void(const RemoveScopesResponse&)>& DeviceClient::RemoveScopesSignal()
{
  return signals_->removescopes_.GetSignal();
}

boost::signals2::signal<void(const RestoreSystemResponse&)>& DeviceClient::RestoreSystemSignal()
{
  return signals_->restoresystem_.GetSignal();
}

boost::signals2::signal<void(const ScanAvailableDot11NetworksResponse&)>& DeviceClient::ScanAvailableDot11NetworksSignal()
{
  return signals_->scanavailabledot11networks_.GetSignal();
}

boost::signals2::signal<void(const SendAuxiliaryCommandResponse&)>& DeviceClient::SendAuxiliaryCommandSignal()
{
  return signals_->sendauxiliarycommand_.GetSignal();
}

boost::signals2::signal<void(const SetAccessPolicyResponse&)>& DeviceClient::SetAccessPolicySignal()
{
  return signals_->setaccesspolicy_.GetSignal();
}

boost::signals2::signal<void(const SetCertificatesStatusResponse&)>& DeviceClient::SetCertificatesStatusSignal()
{
  return signals_->setcertificatesstatus_.GetSignal();
}

boost::signals2::signal<void(const SetClientCertificateModeResponse&)>& DeviceClient::SetClientCertificateModeSignal()
{
  return signals_->setclientcertificatemode_.GetSignal();
}

boost::signals2::signal<void(const SetDiscoveryModeResponse&)>& DeviceClient::SetDiscoveryModeSignal()
{
  return signals_->setdiscoverymode_.GetSignal();
}

boost::signals2::signal<void(const SetDNSResponse&)>& DeviceClient::SetDNSSignal()
{
  return signals_->setdns_.GetSignal();
}

boost::signals2::signal<void(const SetDot1XConfigurationResponse&)>& DeviceClient::SetDot1XConfigurationSignal()
{
  return signals_->setdot1xconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetDPAddressesResponse&)>& DeviceClient::SetDPAddressesSignal()
{
  return signals_->setdpaddresses_.GetSignal();
}

boost::signals2::signal<void(const SetDynamicDNSResponse&)>& DeviceClient::SetDynamicDNSSignal()
{
  return signals_->setdynamicdns_.GetSignal();
}

boost::signals2::signal<void(const SetHostnameResponse&)>& DeviceClient::SetHostnameSignal()
{
  return signals_->sethostname_.GetSignal();
}

boost::signals2::signal<void(const SetHostnameFromDHCPResponse&)>& DeviceClient::SetHostnameFromDHCPSignal()
{
  return signals_->sethostnamefromdhcp_.GetSignal();
}

boost::signals2::signal<void(const SetIPAddressFilterResponse&)>& DeviceClient::SetIPAddressFilterSignal()
{
  return signals_->setipaddressfilter_.GetSignal();
}

boost::signals2::signal<void(const SetNetworkDefaultGatewayResponse&)>& DeviceClient::SetNetworkDefaultGatewaySignal()
{
  return signals_->setnetworkdefaultgateway_.GetSignal();
}

boost::signals2::signal<void(const SetNetworkInterfacesResponse&)>& DeviceClient::SetNetworkInterfacesSignal()
{
  return signals_->setnetworkinterfaces_.GetSignal();
}

boost::signals2::signal<void(const SetNetworkProtocolsResponse&)>& DeviceClient::SetNetworkProtocolsSignal()
{
  return signals_->setnetworkprotocols_.GetSignal();
}

boost::signals2::signal<void(const SetNTPResponse&)>& DeviceClient::SetNTPSignal()
{
  return signals_->setntp_.GetSignal();
}

boost::signals2::signal<void(const SetRelayOutputSettingsResponse&)>& DeviceClient::SetRelayOutputSettingsSignal()
{
  return signals_->setrelayoutputsettings_.GetSignal();
}

boost::signals2::signal<void(const SetRemoteDiscoveryModeResponse&)>& DeviceClient::SetRemoteDiscoveryModeSignal()
{
  return signals_->setremotediscoverymode_.GetSignal();
}

boost::signals2::signal<void(const SetSystemDateAndTimeResponse&)>& DeviceClient::SetSystemDateAndTimeSignal()
{
  return signals_->setsystemdateandtime_.GetSignal();
}

boost::signals2::signal<void(const SetSystemFactoryDefaultResponse&)>& DeviceClient::SetSystemFactoryDefaultSignal()
{
  return signals_->setsystemfactorydefault_.GetSignal();
}

boost::signals2::signal<void(const SetUserResponse&)>& DeviceClient::SetUserSignal()
{
  return signals_->setuser_.GetSignal();
}

boost::signals2::signal<void(const SetZeroConfigurationResponse&)>& DeviceClient::SetZeroConfigurationSignal()
{
  return signals_->setzeroconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SystemRebootResponse&)>& DeviceClient::SystemRebootSignal()
{
  return signals_->systemreboot_.GetSignal();
}

void DeviceClient::Update(DEVICEOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case DEVICEOPERATION_ADDIPADDRESSFILTER:
    {
      auto addipaddressfilterresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddIPAddressFilterResponse']");
      if (!addipaddressfilterresponse.node())
      {
        SignalError(DEVICEOPERATION_ADDIPADDRESSFILTER, handle, localendpoint, latency, "/Envelope/Body/AddIPAddressFilterResponse element not found");
        break;
      }
      
      signals_->addipaddressfilter_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_ADDSCOPES:
    {
      auto addscopesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddScopesResponse']");
      if (!addscopesresponse.node())
      {
        SignalError(DEVICEOPERATION_ADDSCOPES, handle, localendpoint, latency, "/Envelope/Body/AddScopesResponse element not found");
        break;
      }

      signals_->addscopes_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_CREATECERTIFICATE:
    {
      auto createcertificateresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateCertificateResponse']");
      if (!createcertificateresponse.node())
      {
        SignalError(DEVICEOPERATION_CREATECERTIFICATE, handle, localendpoint, latency, "/Envelope/Body/CreateCertificateResponse element not found");
        break;
      }

      signals_->createcertificate_.Emit(handle, localendpoint, latency, std::string(), GetClass<Certificate>(createcertificateresponse, "*[local-name()='NvtCertificate']"));
      break;
    }
    case DEVICEOPERATION_CREATESTORAGECONFIGURATION:
    {
      auto createcertificateresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateStorageConfigurationResponse']");
      if (!createcertificateresponse.node())
      {
        SignalError(DEVICEOPERATION_CREATESTORAGECONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/CreateStorageConfigurationResponse element not found");
        break;
      }

      signals_->createstorageconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetText(createcertificateresponse, "*[local-name()='Token']"));
      break;
    }
    case DEVICEOPERATION_CREATEUSERS:
    {
      auto createusersresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateUsersResponse']");
      if (!createusersresponse.node())
      {
        SignalError(DEVICEOPERATION_CREATEUSERS, handle, localendpoint, latency, "/Envelope/Body/CreateUsersResponse element not found");
        break;
      }

      signals_->createusers_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_DELETECERTIFICATES:
    {
      auto deletecertificatesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteCertificatesResponse']");
      if (!deletecertificatesresponse.node())
      {
        SignalError(DEVICEOPERATION_DELETECERTIFICATES, handle, localendpoint, latency, "/Envelope/Body/DeleteCertificatesResponse element not found");
        break;
      }

      signals_->deletecertificates_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_DELETEDOT1XCONFIGURATION:
    {
      auto deletedot1xconfigurationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteDot1XConfigurationResponse']");
      if (!deletedot1xconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_DELETEDOT1XCONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/DeleteDot1XConfigurationResponse element not found");
        break;
      }

      signals_->deletedot1xconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_DELETESTORAGECONFIGURATION:
    {
      auto deletestorageconfigurationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteStorageConfigurationResponse']");
      if (!deletestorageconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_DELETESTORAGECONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/DeleteStorageConfigurationResponse element not found");
        break;
      }

      signals_->deletestorageconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_DELETEUSERS:
    {
      auto deletesersresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteUsersResponse']");
      if (!deletesersresponse.node())
      {
        SignalError(DEVICEOPERATION_DELETEUSERS, handle, localendpoint, latency, "/Envelope/Body/DeleteUsersResponse element not found");
        break;
      }
      
      signals_->deleteusers_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_GETACCESSPOLICY:
    {
      auto getaccesspolicyresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAccessPolicyResponse']");
      if (!getaccesspolicyresponse.node())
      {
        SignalError(DEVICEOPERATION_GETACCESSPOLICY, handle, localendpoint, latency, "/Envelope/Body/GetAccessPolicyResponse element not found");
        break;
      }

      signals_->getaccesspolicy_.Emit(handle, localendpoint, latency, std::string(), GetClass<BinaryData>(getaccesspolicyresponse, "*[local-name()='PolicyFile']"));
      break;
    }
    case DEVICEOPERATION_GETCACERTIFICATES:
    {
      auto getcacertificatesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCACertificatesResponse']");
      if (!getcacertificatesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETCACERTIFICATES, handle, localendpoint, latency, "/Envelope/Body/GetCACertificatesResponse element not found");
        break;
      }

      std::vector<Certificate> certificates;
      for (const auto& getcacertificateresponse : getcacertificatesresponse.node().select_nodes("*[local-name()='CACertificate']"))
      {
        certificates.push_back(Certificate(getcacertificateresponse.node()));

      }

      signals_->getcacertificates_.Emit(handle, localendpoint, latency, std::string(), certificates);
      break;
    }
    case DEVICEOPERATION_GETCAPABILITIES:
    {
      auto getcapabilitiesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCapabilitiesResponse']");
      if (!getcapabilitiesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETCAPABILITIES, handle, localendpoint, latency, "/Envelope/Body/GetCapabilitiesResponse element not found");
        break;
      }
      
      signals_->getcapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getcapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATEINFORMATION:
    {
      auto getcertificateinformationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificateInformationResponse']");
      if (!getcertificateinformationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETCERTIFICATEINFORMATION, handle, localendpoint, latency, "/Envelope/Body/GetCertificateInformationResponse element not found");
        break;
      }

      signals_->getcertificateinformation_.Emit(handle, localendpoint, latency, std::string(), GetClass<CertificateInformation>(getcertificateinformationresponse, "*[local-name()='CertificateInformation']"));
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATES:
    {
      auto getcertificatesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificatesResponse']");
      if (!getcertificatesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETCERTIFICATES, handle, localendpoint, latency, "/Envelope/Body/GetCertificatesResponse element not found");
        break;
      }

      std::vector<Certificate> certificates;
      for (const auto& getcertificateresponse : getcertificatesresponse.node().select_nodes("*[local-name()='NvtCertificate']"))
      {
        certificates.push_back(Certificate(getcertificateresponse.node()));

      }

      signals_->getcertificates_.Emit(handle, localendpoint, latency, std::string(), certificates);
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATESSTATUS:
    {
      auto getcertificatesstatusresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificatesStatusResponse']");
      if (!getcertificatesstatusresponse.node())
      {
        SignalError(DEVICEOPERATION_GETCERTIFICATESSTATUS, handle, localendpoint, latency, "/Envelope/Body/GetCertificatesStatusResponse element not found");
        break;
      }

      std::vector<CertificateStatus> certificatesstatus;
      for (const auto& getcertificateresponse : getcertificatesstatusresponse.node().select_nodes("*[local-name()='CertificateStatus']"))
      {
        certificatesstatus.push_back(CertificateStatus(getcertificateresponse.node()));

      }

      signals_->getcertificatesstatus_.Emit(handle, localendpoint, latency, std::string(), certificatesstatus);
      break;
    }
    case DEVICEOPERATION_GETCLIENTCERTIFICATEMODE:
    {
      auto getclientcertificatemoderesponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetClientCertificateModeResponse']");
      if (!getclientcertificatemoderesponse.node())
      {
        SignalError(DEVICEOPERATION_GETCLIENTCERTIFICATEMODE, handle, localendpoint, latency, "/Envelope/Body/GetClientCertificateModeResponse element not found");
        break;
      }
      
      signals_->getclientcertificatemode_.Emit(handle, localendpoint, latency, std::string(), GetBool(getclientcertificatemoderesponse, "*[local-name()='Enabled']"));
      break;
    }
    case DEVICEOPERATION_GETDEVICEINFORMATION:
    {
      auto getdeviceinformationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDeviceInformationResponse']");
      if (!getdeviceinformationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDEVICEINFORMATION, handle, localendpoint, latency, "/Envelope/Body/GetDeviceInformationResponse element not found");
        break;
      }

      signals_->getdeviceinformation_.Emit(handle, localendpoint, latency, std::string(), GetText(getdeviceinformationresponse, "*[local-name()='Manufacturer']"), GetText(getdeviceinformationresponse, "*[local-name()='Model']"), GetText(getdeviceinformationresponse, "*[local-name()='FirmwareVersion']"), GetText(getdeviceinformationresponse, "*[local-name()='SerialNumber']"), GetText(getdeviceinformationresponse, "*[local-name()='HardwareId']"));
      break;
    }
    case DEVICEOPERATION_GETDISCOVERYMODE:
    {
      auto discoverymodenode = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDiscoveryModeResponse']/*[local-name()='DiscoveryMode']");
      if (!discoverymodenode.node())
      {
        SignalError(DEVICEOPERATION_GETDISCOVERYMODE, handle, localendpoint, latency, "/Envelope/Body/GetDiscoveryModeResponse/DiscoveryMode element not found");
        break;
      }

      signals_->getdiscoverymode_.Emit(handle, localendpoint, latency, std::string(), onvif::GetDiscoveryMode(discoverymodenode.node().text().get()));
      break;
    }
    case DEVICEOPERATION_GETDNS:
    {
      auto getdnsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDNSResponse']");
      if (!getdnsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDNS, handle, localendpoint, latency, "/Envelope/Body/GetDNSResponse element not found");
        break;
      }
      
      signals_->getdns_.Emit(handle, localendpoint, latency, std::string(), GetClass<DNSInformation>(getdnsresponse, "*[local-name()='DNSInformation']"));
      break;
    }
    case DEVICEOPERATION_GETDOT11CAPABILITIES:
    {
      auto getdot11capabilitiesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot11CapabilitiesResponse']");
      if (!getdot11capabilitiesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDOT11CAPABILITIES, handle, localendpoint, latency, "/Envelope/Body/GetDot11CapabilitiesResponse element not found");
        break;
      }
      
      signals_->getdot11capabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Dot11Capabilities>(getdot11capabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case DEVICEOPERATION_GETDOT11STATUS:
    {
      auto getdot11statusresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot11StatusResponse']");
      if (!getdot11statusresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDOT11STATUS, handle, localendpoint, latency, "/Envelope/Body/GetDot11CapabilitiesResponse element not found");
        break;
      }
      
      signals_->getdot11status_.Emit(handle, localendpoint, latency, std::string(), GetClass<Dot11Status>(getdot11statusresponse, "*[local-name()='Status']"));
      break;
    }
    case DEVICEOPERATION_GETDOT1XCONFIGURATION:
    {
      auto getdot1xconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot1XConfigurationResponse']");
      if (!getdot1xconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDOT1XCONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/GetDot1XConfigurationResponse element not found");
        break;
      }
      
      signals_->getdot1xconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<Dot1XConfiguration>(getdot1xconfigurationresponse, "*[local-name()='Dot1XConfiguration']"));
      break;
    }
    case DEVICEOPERATION_GETDOT1XCONFIGURATIONS:
    {
      auto getdot1xconfigurationsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot1XConfigurationsResponse']");
      if (!getdot1xconfigurationsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDOT1XCONFIGURATIONS, handle, localendpoint, latency, "/Envelope/Body/GetDot1XConfigurationsResponse element not found");
        break;
      }

      std::vector<Dot1XConfiguration> dot1xconfigurations;
      for (const auto& dot1xconfiguration : getdot1xconfigurationsresponse.node().select_nodes("*[local-name()='Dot1XConfiguration']"))
      {
        dot1xconfigurations.push_back(Dot1XConfiguration(dot1xconfiguration.node()));

      }

      signals_->getdot1xconfigurations_.Emit(handle, localendpoint, latency, std::string(), dot1xconfigurations);
      break;
    }
    case DEVICEOPERATION_GETDPADDRESSES:
    {
      auto getdpaddressesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDPAddressesResponse']");
      if (!getdpaddressesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDPADDRESSES, handle, localendpoint, latency, "/Envelope/Body/GetDPAddressesResponse element not found");
        break;
      }

      std::vector<NetworkHost> dpaddresses;
      for (const auto& dpaddress : getdpaddressesresponse.node().select_nodes("*[local-name()='DPAddress']"))
      {
        dpaddresses.push_back(NetworkHost(dpaddress.node()));

      }

      signals_->getdpaddresses_.Emit(handle, localendpoint, latency, std::string(), dpaddresses);
      break;
    }
    case DEVICEOPERATION_GETDYNAMICDNS:
    {
      auto getdynamicdnsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDynamicDNSResponse']");
      if (!getdynamicdnsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETDYNAMICDNS, handle, localendpoint, latency, "/Envelope/Body/GetDynamicDNSResponse element not found");
        break;
      }
      
      signals_->getdynamicdns_.Emit(handle, localendpoint, latency, std::string(), GetClass<DynamicDNSInformation>(getdynamicdnsresponse, "*[local-name()='DynamicDNSInformation']"));
      break;
    }
    case DEVICEOPERATION_GETENDPOINTREFERENCE:
    {
      auto getendpointreferenceresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetEndpointReferenceResponse']");
      if (!getendpointreferenceresponse.node())
      {
        SignalError(DEVICEOPERATION_GETENDPOINTREFERENCE, handle, localendpoint, latency, "/Envelope/Body/GetEndpointReference element not found");
        break;
      }

      signals_->getendpointreference_.Emit(handle, localendpoint, latency, std::string(), GetText(getendpointreferenceresponse, "*[local-name()='GUID']"));
      break;
    }
    case DEVICEOPERATION_GETHOSTNAME:
    {
      auto gethostnameresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetHostnameResponse']");
      if (!gethostnameresponse.node())
      {
        SignalError(DEVICEOPERATION_GETHOSTNAME, handle, localendpoint, latency, "/Envelope/Body/GetHostnameResponse element not found");
        break;
      }
      
      signals_->gethostname_.Emit(handle, localendpoint, latency, std::string(), GetClass<HostnameInformation>(gethostnameresponse, "*[local-name()='HostnameInformation']"));
      break;
    }
    case DEVICEOPERATION_GETIPADDRESSFILTER:
    {
      auto getipaddressfilterresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetIPAddressFilterResponse']");
      if (!getipaddressfilterresponse.node())
      {
        SignalError(DEVICEOPERATION_GETIPADDRESSFILTER, handle, localendpoint, latency, "/Envelope/Body/GetIPAddressFilterResponse element not found");
        break;
      }
      
      signals_->getipaddressfilter_.Emit(handle, localendpoint, latency, std::string(), GetClass<IPAddressFilter>(getipaddressfilterresponse, "*[local-name()='IPAddressFilter']"));
      break;
    }
    case DEVICEOPERATION_GETNETWORKDEFAULTGATEWAY:
    {
      auto getnetworkdefaultgatewayresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkDefaultGatewayResponse']");
      if (!getnetworkdefaultgatewayresponse.node())
      {
        SignalError(DEVICEOPERATION_GETNETWORKDEFAULTGATEWAY, handle, localendpoint, latency, "/Envelope/Body/GetNetworkDefaultGatewayResponse element not found");
        break;
      }
      
      signals_->getnetworkdefaultgateway_.Emit(handle, localendpoint, latency, std::string(), GetClass<NetworkGateway>(getnetworkdefaultgatewayresponse, "*[local-name()='NetworkGateway']"));
      break;
    }
    case DEVICEOPERATION_GETNETWORKINTERFACES:
    {
      auto getnetworkinterfacesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkInterfacesResponse']");
      if (!getnetworkinterfacesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETNETWORKINTERFACES, handle, localendpoint, latency, "/Envelope/Body/GetNetworkInterfacesResponse element not found");
        break;
      }

      std::vector<NetworkInterface> networkinterfaces;
      for (const auto& networkinterface : getnetworkinterfacesresponse.node().select_nodes("*[local-name()='NetworkInterfaces']"))
      {
        networkinterfaces.push_back(NetworkInterface(networkinterface.node()));

      }

      signals_->getnetworkinterfaces_.Emit(handle, localendpoint, latency, std::string(), networkinterfaces);
      break;
    }
    case DEVICEOPERATION_GETNETWORKPROTOCOLS:
    {
      auto getnetworkprotocolsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkProtocolsResponse']");
      if (!getnetworkprotocolsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETNETWORKINTERFACES, handle, localendpoint, latency, "/Envelope/Body/GetNetworkProtocolsResponse element not found");
        break;
      }

      std::vector<NetworkProtocol> networkprotocols;
      for (const auto& networkprotocol : getnetworkprotocolsresponse.node().select_nodes("*[local-name()='NetworkProtocols']"))
      {
        networkprotocols.push_back(NetworkProtocol(networkprotocol.node()));

      }

      signals_->getnetworkprotocols_.Emit(handle, localendpoint, latency, std::string(), networkprotocols);
      break;
    }
    case DEVICEOPERATION_GETPKCS10REQUEST:
    {
      auto getpkcs10requestresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetPkcs10RequestResponse']");
      if (!getpkcs10requestresponse.node())
      {
        SignalError(DEVICEOPERATION_GETPKCS10REQUEST, handle, localendpoint, latency, "/Envelope/Body/GetPkcs10RequestResponse element not found");
        break;
      }

      signals_->getpkcs10request_.Emit(handle, localendpoint, latency, std::string(), GetClass<BinaryData>(getpkcs10requestresponse, "*[local-name()='Pkcs10Request']"));
      break;
    }
    case DEVICEOPERATION_GETRELAYOUTPUTS:
    {
      auto getrelayoutputsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputsResponse']");
      if (!getrelayoutputsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETRELAYOUTPUTS, handle, localendpoint, latency, "/Envelope/Body/GetRelayOutputsResponse element not found");
        break;
      }

      std::vector<RelayOutput> relayoutputs;
      for (const auto& relayoutput : getrelayoutputsresponse.node().select_nodes("*[local-name()='RelayOutputs']"))
      {
        relayoutputs.push_back(RelayOutput(relayoutput.node()));

      }

      signals_->getrelayoutputs_.Emit(handle, localendpoint, latency, std::string(), relayoutputs);
      break;
    }
    case DEVICEOPERATION_GETNTP:
    {
      auto getntpresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNTPResponse']");
      if (!getntpresponse.node())
      {
        SignalError(DEVICEOPERATION_GETNTP, handle, localendpoint, latency, "/Envelope/Body/GetNTPResponse element not found");
        break;
      }

      signals_->getntp_.Emit(handle, localendpoint, latency, std::string(), GetClass<NTPInformation>(getntpresponse, "*[local-name()='NTPInformation']"));
      break;
    }
    case DEVICEOPERATION_GETREMOTEDISCOVERYMODE:
    {
      auto remotediscoverymodenode = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRemoteDiscoveryModeResponse']/*[local-name()='RemoteDiscoveryMode']");
      if (!remotediscoverymodenode.node())
      {
        SignalError(DEVICEOPERATION_GETREMOTEDISCOVERYMODE, handle, localendpoint, latency, "/Envelope/Body/GetRemoteDiscoveryModeResponse/RemoteDiscoveryMode element not found");
        break;
      }

      signals_->getremotediscoverymode_.Emit(handle, localendpoint, latency, std::string(), onvif::GetDiscoveryMode(remotediscoverymodenode.node().text().get()));
      break;
    }
    case DEVICEOPERATION_GETREMOTEUSER:
    {
      auto getremoteuserresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRemoteUserResponse']");
      if (!getremoteuserresponse.node())
      {
        SignalError(DEVICEOPERATION_GETREMOTEUSER, handle, localendpoint, latency, "/Envelope/Body/GetRemoteUserResponse element not found");
        break;
      }
      
      signals_->getremoteuser_.Emit(handle, localendpoint, latency, std::string(), GetClass<RemoteUser>(getremoteuserresponse, "*[local-name()='RemoteUser']"));
      break;
    }
    case DEVICEOPERATION_GETSCOPES:
    {
      auto getscopesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetScopesResponse']");
      if (!getscopesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSCOPES, handle, localendpoint, latency, "/Envelope/Body/GetScopesResponse element not found");
        break;
      }

      std::vector<Scope> scopes;
      for (const auto& scope : getscopesresponse.node().select_nodes("*[local-name()='Scopes']"))
      {
        scopes.push_back(Scope(scope.node()));

      }
      
      signals_->getscopes_.Emit(handle, localendpoint, latency, std::string(), scopes);
      break;
    }
    case DEVICEOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, "/Envelope/Body/GetServiceCapabilitiesResponse element not found");
        break;
      }

      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<DeviceServiceCapabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case DEVICEOPERATION_GETSERVICES:
    {
      auto getservicesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServicesResponse']");
      if (!getservicesresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSERVICES, handle, localendpoint, latency, "/Envelope/Body/GetServicesResponse element not found");
        break;
      }

      std::vector<Service> services;
      for (const auto& service : getservicesresponse.node().select_nodes("*[local-name()='Service']"))
      {
        services.push_back(Service(service.node()));

      }
      
      signals_->getservices_.Emit(handle, localendpoint, latency, std::string(), services);
      break;
    }
    case DEVICEOPERATION_GETSTORAGECONFIGURATION:
    {
      auto getstorageconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStorageConfigurationResponse']");
      if (!getstorageconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSTORAGECONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/GetStorageConfigurationResponse element not found");
        break;
      }
      
      signals_->getstorageconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<StorageConfiguration>(getstorageconfigurationresponse, "*[local-name()='StorageConfiguration']"));
      break;
    }
    case DEVICEOPERATION_GETSTORAGECONFIGURATIONS:
    {
      auto getstorageconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStorageConfigurationsResponse']");
      if (!getstorageconfigurationsresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSTORAGECONFIGURATIONS, handle, localendpoint, latency, "/Envelope/Body/GetStorageConfigurationsResponse element not found");
        break;
      }

      std::vector<StorageConfiguration> storageconfigurations;
      for (const auto& service : getstorageconfigurationsresponse.node().select_nodes("*[local-name()='StorageConfigurations']"))
      {
        storageconfigurations.push_back(StorageConfiguration(service.node()));

      }
      
      signals_->getstorageconfigurations_.Emit(handle, localendpoint, latency, std::string(), storageconfigurations);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMBACKUP:
    {
      auto getsystembackupresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemBackupResponse']");
      if (!getsystembackupresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSYSTEMBACKUP, handle, localendpoint, latency, "/Envelope/Body/GetSystemBackupResponse element not found");
        break;
      }

      bool error = false;
      std::vector<BackupFile> backupfiles;
      for (const auto& backupfilenode : getsystembackupresponse.node().select_nodes("*[local-name()='BackupFiles']"))
      {
        // Organise references to MTOM data
        BackupFile backupfile = BackupFile(backupfilenode.node());
        if (!backupfile.data_.is_initialized() || !backupfile.data_->include_.is_initialized() || !backupfile.data_->include_->href_.is_initialized())
        {

          continue;
        }

        // We do a reverse lookup here, because MTOM should contain cid: prefix, but probably often won't because bad software engineers
        const auto href = std::find_if(mtomdata.begin(), mtomdata.end(), [&backupfile](const std::pair< std::string, std::vector<char> >& element){ return (backupfile.data_->include_->href_->find(element.first) != std::string::npos); });
        if (href == mtomdata.end())
        {
          error = true;
          continue;
        }

        backupfile.data_->include_->data_ = href->second;
        backupfiles.push_back(backupfile);
      }

      if (error)
      {
        SignalError(DEVICEOPERATION_GETSYSTEMBACKUP, handle, localendpoint, latency, "Illegal MTOM data");
        break;
      }

      signals_->getsystembackup_.Emit(handle, localendpoint, latency, std::string(), backupfiles);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMDATEANDTIME:
    {
      auto getsystemdateandtimeresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemDateAndTimeResponse']");
      if (!getsystemdateandtimeresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSYSTEMDATEANDTIME, handle, localendpoint, latency, "/Envelope/Body/GetSystemDateAndTimeResponse element not found");
        break;
      }
      
      signals_->getsystemdateandtime_.Emit(handle, localendpoint, latency, std::string(), GetClass<SystemDateTime>(getsystemdateandtimeresponse, "*[local-name()='SystemDateAndTime']"));
      break;
    }
    case DEVICEOPERATION_GETSYSTEMLOG:
    {
      auto getsystemlogresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemLogResponse']");
      if (!getsystemlogresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSYSTEMLOG, handle, localendpoint, latency, "/Envelope/Body/GetSystemLogResponse element not found");
        break;
      }
      
      signals_->getsystemlog_.Emit(handle, localendpoint, latency, std::string(), GetClass<SystemLog>(getsystemlogresponse, "*[local-name()='SystemLog']"));
      break;
    }
    case DEVICEOPERATION_GETSYSTEMSUPPORTINFORMATION:
    {
      auto getsystemsupportinformationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemSupportInformationResponse']");
      if (!getsystemsupportinformationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSYSTEMSUPPORTINFORMATION, handle, localendpoint, latency, "/Envelope/Body/GetSystemSupportInformationResponse element not found");
        break;
      }

      signals_->getsystemsupportinformation_.Emit(handle, localendpoint, latency, std::string(), GetClass<SupportInformation>(getsystemsupportinformationresponse, "*[local-name()='SupportInformation']"));
      break;
    }
    case DEVICEOPERATION_GETSYSTEMURIS:
    {
      auto getsystemurisresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemUrisResponse']");
      if (!getsystemurisresponse.node())
      {
        SignalError(DEVICEOPERATION_GETSYSTEMURIS, handle, localendpoint, latency, "/Envelope/Body/GetSystemUrisResponse element not found");
        break;
      }

      signals_->getsystemuris_.Emit(handle, localendpoint, latency, std::string(), GetClass<SystemLogUriList>(getsystemurisresponse, "*[local-name()='SystemLogUris']"), GetText(getsystemurisresponse, "*[local-name()='SupportInfoUri']"), GetText(getsystemurisresponse, "*[local-name()='SystemBackupUri']"));
      break;
    }
    case DEVICEOPERATION_GETUSERS:
    {
      auto getusersresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetUsersResponse']");
      if (!getusersresponse.node())
      {
        SignalError(DEVICEOPERATION_GETUSERS, handle, localendpoint, latency, "/Envelope/Body/GetUsersResponse element not found");
        break;
      }

      std::vector<User> users;
      for (const auto& user : getusersresponse.node().select_nodes("*[local-name()='User']"))
      {
        users.push_back(User(user.node()));

      }

      signals_->getusers_.Emit(handle, localendpoint, latency, std::string(), users);
      break;
    }
    case DEVICEOPERATION_GETWSDLURL:
    {
      auto getwsdlurlresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetWsdlUrlResponse']");
      if (!getwsdlurlresponse.node())
      {
        SignalError(DEVICEOPERATION_GETWSDLURL, handle, localendpoint, latency, "/Envelope/Body/GetWsdlUrlResponse element not found");
        break;
      }
      
      signals_->getwsdlurl_.Emit(handle, localendpoint, latency, std::string(), GetText(getwsdlurlresponse, "*[local-name()='WsdlUrl']"));
      break;
    }
    case DEVICEOPERATION_GETZEROCONFIGURATION:
    {
      auto getzeroconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetZeroConfigurationResponse']");
      if (!getzeroconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_GETZEROCONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/GetZeroConfigurationResponse element not found");
        break;
      }

      signals_->getzeroconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<NetworkZeroConfiguration>(getzeroconfigurationresponse, "*[local-name()='ZeroConfiguration']"));
      break;
    }
    case DEVICEOPERATION_LOADCACERTIFICATES:
    {
      auto loadcacertificatesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCACertificatesResponse']");
      if (!loadcacertificatesresponse.node())
      {
        SignalError(DEVICEOPERATION_LOADCACERTIFICATES, handle, localendpoint, latency, "/Envelope/Body/LoadCACertificatesResponse element not found");
        break;
      }

      signals_->loadcacertificates_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_LOADCERTIFICATES:
    {
      auto loadcertificatesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCertificatesResponse']");
      if (!loadcertificatesresponse.node())
      {
        SignalError(DEVICEOPERATION_LOADCERTIFICATES, handle, localendpoint, latency, "/Envelope/Body/LoadCertificatesResponse element not found");
        break;
      }

      signals_->loadcertificates_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_LOADCERTIFICATEWITHPRIVATEKEY:
    {
      auto loadcertificatewithprivatekeyresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCertificateWithPrivateKeyResponse']");
      if (!loadcertificatewithprivatekeyresponse.node())
      {
        SignalError(DEVICEOPERATION_LOADCERTIFICATEWITHPRIVATEKEY, handle, localendpoint, latency, "/Envelope/Body/LoadCertificateWithPrivateKeyResponse element not found");
        break;
      }

      signals_->loadcertificatewithprivatekey_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETHOSTNAME:
    {
      auto sethostnameresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetHostnameResponse']");
      if (!sethostnameresponse.node())
      {
        SignalError(DEVICEOPERATION_SETHOSTNAME, handle, localendpoint, latency, "/Envelope/Body/SetHostnameResponse element not found");
        break;
      }
      
      signals_->sethostname_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETHOSTNAMEFROMDHCP:
    {
      auto sethostnamefromdhcpresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetHostnameFromDHCPResponse']");
      if (!sethostnamefromdhcpresponse.node())
      {
        SignalError(DEVICEOPERATION_SETHOSTNAMEFROMDHCP, handle, localendpoint, latency, "/Envelope/Body/SetHostnameFromDHCPResponse element not found");
        break;
      }
      
      signals_->sethostnamefromdhcp_.Emit(handle, localendpoint, latency, std::string(), GetBool(sethostnamefromdhcpresponse, "*[local-name()='RebootNeeded']"));
      break;
    }
    case DEVICEOPERATION_SETIPADDRESSFILTER:
    {
      auto setipaddressfilterresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetIPAddressFilterResponse']");
      if (!setipaddressfilterresponse.node())
      {
        SignalError(DEVICEOPERATION_SETIPADDRESSFILTER, handle, localendpoint, latency, "/Envelope/Body/SetIPAddressFilterResponse element not found");
        break;
      }
      
      signals_->setipaddressfilter_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETNETWORKDEFAULTGATEWAY:
    {
      auto setnetworkdefaultgatewayresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkDefaultGatewayResponse']");
      if (!setnetworkdefaultgatewayresponse.node())
      {
        SignalError(DEVICEOPERATION_SETNETWORKPROTOCOLS, handle, localendpoint, latency, "/Envelope/Body/SetNetworkDefaultGatewayResponse element not found");
        break;
      }
      
      signals_->setnetworkdefaultgateway_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETNETWORKINTERFACES:
    {
      auto setnetworkinterfacesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkInterfacesResponse']");
      if (!setnetworkinterfacesresponse.node())
      {
        SignalError(DEVICEOPERATION_SETNETWORKINTERFACES, handle, localendpoint, latency, "/Envelope/Body/SetNetworkInterfacesResponse element not found");
        break;
      }
      
      signals_->setnetworkinterfaces_.Emit(handle, localendpoint, latency, std::string(), GetBool(setnetworkinterfacesresponse, "*[local-name()='RebootNeeded']"));
      break;
    }
    case DEVICEOPERATION_SETNETWORKPROTOCOLS:
    {
      auto setnetworkprotocolsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkProtocolsResponse']");
      if (!setnetworkprotocolsresponse.node())
      {
        SignalError(DEVICEOPERATION_SETNETWORKPROTOCOLS, handle, localendpoint, latency, "/Envelope/Body/SetNetworkProtocolsResponse element not found");
        break;
      }
      
      signals_->setnetworkprotocols_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETNTP:
    {
      auto setntpresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNTPResponse']");
      if (!setntpresponse.node())
      {
        SignalError(DEVICEOPERATION_SETNTP, handle, localendpoint, latency, "/Envelope/Body/SetNTPResponse element not found");
        break;
      }
      
      signals_->setntp_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_REMOVEIPADDRESSFILTER:
    {
      auto removeipaddressfilterresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveIPAddressFilterResponse']");
      if (!removeipaddressfilterresponse.node())
      {
        SignalError(DEVICEOPERATION_REMOVEIPADDRESSFILTER, handle, localendpoint, latency, "/Envelope/Body/RemoveIPAddressFilterResponse element not found");
        break;
      }
      
      signals_->removeipaddressfilter_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_REMOVESCOPES:
    {
      auto removescopesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveScopesResponse']");
      if (!removescopesresponse.node())
      {
        SignalError(DEVICEOPERATION_REMOVESCOPES, handle, localendpoint, latency, "/Envelope/Body/RemoveScopesResponse element not found");
        break;
      }

      signals_->removescopes_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_RESTORESYSTEM:
    {
      auto restoresystemresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RestoreSystemResponse']");
      if (!restoresystemresponse.node())
      {
        SignalError(DEVICEOPERATION_RESTORESYSTEM, handle, localendpoint, latency, "/Envelope/Body/RestoreSystemResponse element not found");
        break;
      }

      signals_->restoresystem_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SCANAVAILABLEDOT11NETWORKS:
    {
      auto scanavailabledot11networksresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ScanAvailableDot11NetworksResponse']");
      if (!scanavailabledot11networksresponse.node())
      {
        SignalError(DEVICEOPERATION_SCANAVAILABLEDOT11NETWORKS, handle, localendpoint, latency, "/Envelope/Body/ScanAvailableDot11NetworksResponse element not found");
        break;
      }

      std::vector<Dot11AvailableNetworks> networks;
      for (const auto& network : scanavailabledot11networksresponse.node().select_nodes("*[local-name()='Networks']"))
      {
        networks.push_back(Dot11AvailableNetworks(network.node()));

      }

      signals_->scanavailabledot11networks_.Emit(handle, localendpoint, latency, std::string(), networks);
      break;
    }
    case DEVICEOPERATION_SENDAUXILIARYCOMMAND:
    {
      auto sendauxiliarycommandresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SendAuxiliaryCommandResponse']");
      if (!sendauxiliarycommandresponse.node())
      {
        SignalError(DEVICEOPERATION_SENDAUXILIARYCOMMAND, handle, localendpoint, latency, "/Envelope/Body/SendAuxiliaryCommandResponse element not found");
        break;
      }

      signals_->sendauxiliarycommand_.Emit(handle, localendpoint, latency, std::string(), GetText(sendauxiliarycommandresponse, "*[local-name()='AuxiliaryCommandResponse']"));
      break;
    }
    case DEVICEOPERATION_SETACCESSPOLICY:
    {
      auto setaccesspolicyresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAccessPolicyResponse']");
      if (!setaccesspolicyresponse.node())
      {
        SignalError(DEVICEOPERATION_SETACCESSPOLICY, handle, localendpoint, latency, "/Envelope/Body/SetAccessPolicyResponse element not found");
        break;
      }

      signals_->setaccesspolicy_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETCERTIFICATESSTATUS:
    {
      auto setcertificatesstatusresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetCertificatesStatusResponse']");
      if (!setcertificatesstatusresponse.node())
      {
        SignalError(DEVICEOPERATION_SETCERTIFICATESSTATUS, handle, localendpoint, latency, "/Envelope/Body/SetCertificatesStatusResponse element not found");
        break;
      }

      signals_->setcertificatesstatus_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETCLIENTCERTIFICATEMODE:
    {
      auto setclientcertificatemoderesponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetClientCertificateModeResponse']");
      if (!setclientcertificatemoderesponse.node())
      {
        SignalError(DEVICEOPERATION_SETCLIENTCERTIFICATEMODE, handle, localendpoint, latency, "/Envelope/Body/SetClientCertificateModeResponse element not found");
        break;
      }

      signals_->setclientcertificatemode_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETDISCOVERYMODE:
    {
      auto setdiscoverymoderesponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDiscoveryModeResponse']");
      if (!setdiscoverymoderesponse.node())
      {
        SignalError(DEVICEOPERATION_SETDISCOVERYMODE, handle, localendpoint, latency, "/Envelope/Body/SetDiscoveryModeResponse element not found");
        break;
      }
      
      signals_->setdiscoverymode_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETDNS:
    {
      auto setdnsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDNSResponse']");
      if (!setdnsresponse.node())
      {
        SignalError(DEVICEOPERATION_SETDNS, handle, localendpoint, latency, "/Envelope/Body/SetDNSResponse element not found");
        break;
      }
      
      signals_->setdns_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETDOT1XCONFIGURATION:
    {
      auto setdot1xconfigurationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDot1XConfigurationResponse']");
      if (!setdot1xconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_SETDOT1XCONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/SetDot1XConfigurationResponse element not found");
        break;
      }

      signals_->setdot1xconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETDPADDRESSES:
    {
      auto setdpaddressesresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDPAddressesResponse']");
      if (!setdpaddressesresponse.node())
      {
        SignalError(DEVICEOPERATION_SETDPADDRESSES, handle, localendpoint, latency, "/Envelope/Body/SetDPAddressesResponse element not found");
        break;
      }
      
      signals_->setdpaddresses_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETDYNAMICDNS:
    {
      auto setdynamicdnsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDynamicDNSResponse']");
      if (!setdynamicdnsresponse.node())
      {
        SignalError(DEVICEOPERATION_SETDYNAMICDNS, handle, localendpoint, latency, "/Envelope/Body/SetDynamicDNSResponse element not found");
        break;
      }
      
      signals_->setdynamicdns_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETRELAYOUTPUTSETTINGS:
    {
      auto setrelayoutputsettingsresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputSettingsResponse']");
      if (!setrelayoutputsettingsresponse.node())
      {
        SignalError(DEVICEOPERATION_SETREMOTEDISCOVERYMODE, handle, localendpoint, latency, "/Envelope/Body/SetRelayOutputSettingsResponse element not found");
        break;
      }

      signals_->setrelayoutputsettings_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETREMOTEDISCOVERYMODE:
    {
      auto setremotediscoverymoderesponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRemoteDiscoveryModeResponse']");
      if (!setremotediscoverymoderesponse.node())
      {
        SignalError(DEVICEOPERATION_SETREMOTEDISCOVERYMODE, handle, localendpoint, latency, "/Envelope/Body/SetRemoteDiscoveryModeResponse element not found");
        break;
      }

      signals_->setremotediscoverymode_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETSYSTEMDATEANDTIME:
    {
      auto setsystemdateandtimeresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSystemDateAndTimeResponse']");
      if (!setsystemdateandtimeresponse.node())
      {
        SignalError(DEVICEOPERATION_SETSYSTEMDATEANDTIME, handle, localendpoint, latency, "/Envelope/Body/SetSystemDateAndTimeResponse element not found");
        break;
      }
      
      signals_->setsystemdateandtime_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETSYSTEMFACTORYDEFAULT:
    {
      auto setsystemfactorydefaultresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSystemFactoryDefaultResponse']");
      if (!setsystemfactorydefaultresponse.node())
      {
        SignalError(DEVICEOPERATION_SETSYSTEMFACTORYDEFAULT, handle, localendpoint, latency, "/Envelope/Body/SetSystemFactoryDefaultResponse element not found");
        break;
      }
      
      signals_->setsystemfactorydefault_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETUSER:
    {
      auto setuserresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetUserResponse']");
      if (!setuserresponse.node())
      {
        SignalError(DEVICEOPERATION_SETUSER, handle, localendpoint, latency, "/Envelope/Body/SetUserResponse element not found");
        break;
      }
      
      signals_->setuser_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SETZEROCONFIGURATION:
    {
      auto setzeroconfigurationresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetZeroConfigurationResponse']");
      if (!setzeroconfigurationresponse.node())
      {
        SignalError(DEVICEOPERATION_SETZEROCONFIGURATION, handle, localendpoint, latency, "/Envelope/Body/SetZeroConfigurationResponse element not found");
        break;
      }
      
      signals_->setzeroconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEOPERATION_SYSTEMREBOOT:
    {
      auto systemrebootresponse = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SystemRebootResponse']");
      if (!systemrebootresponse.node())
      {
        SignalError(DEVICEOPERATION_SYSTEMREBOOT, handle, localendpoint, latency, "/Envelope/Body/SystemRebootResponse element not found");
        break;
      }
      
      signals_->systemreboot_.Emit(handle, localendpoint, latency, std::string(), GetText(systemrebootresponse, "*[local-name()='Message']"));
      break;
    }
    default:
    {

      break;
    }
  }
}

void DeviceClient::SignalError(DEVICEOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case DEVICEOPERATION_ADDIPADDRESSFILTER:
    {
      signals_->addipaddressfilter_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_ADDSCOPES:
    {
      signals_->addscopes_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_CREATECERTIFICATE:
    {
      signals_->createcertificate_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_CREATESTORAGECONFIGURATION:
    {
      signals_->createstorageconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_CREATEUSERS:
    {
      signals_->createusers_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_DELETECERTIFICATES:
    {
      signals_->deletecertificates_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_DELETEDOT1XCONFIGURATION:
    {
      signals_->deletedot1xconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_DELETESTORAGECONFIGURATION:
    {
      signals_->deletestorageconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_DELETEUSERS:
    {
      signals_->deleteusers_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETACCESSPOLICY:
    {
      signals_->getaccesspolicy_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCACERTIFICATES:
    {
      signals_->getcacertificates_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCAPABILITIES:
    {
      signals_->getcapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATEINFORMATION:
    {
      signals_->getcertificateinformation_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATES:
    {
      signals_->getcertificates_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCERTIFICATESSTATUS:
    {
      signals_->getcertificatesstatus_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETCLIENTCERTIFICATEMODE:
    {
      signals_->getclientcertificatemode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDEVICEINFORMATION:
    {
      signals_->getdeviceinformation_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDISCOVERYMODE:
    {
      signals_->getdiscoverymode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDNS:
    {
      signals_->getdns_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDOT11CAPABILITIES:
    {
      signals_->getdot11capabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDOT11STATUS:
    {
      signals_->getdot11status_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDOT1XCONFIGURATION:
    {
      signals_->getdot1xconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDOT1XCONFIGURATIONS:
    {
      signals_->getdot1xconfigurations_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDPADDRESSES:
    {
      signals_->getdpaddresses_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETDYNAMICDNS:
    {
      signals_->getdynamicdns_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETENDPOINTREFERENCE:
    {
      signals_->getendpointreference_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETHOSTNAME:
    {
      signals_->gethostname_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETIPADDRESSFILTER:
    {
      signals_->getipaddressfilter_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETNETWORKDEFAULTGATEWAY:
    {
      signals_->getnetworkdefaultgateway_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETNETWORKINTERFACES:
    {
      signals_->getnetworkinterfaces_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETNETWORKPROTOCOLS:
    {
      signals_->getnetworkprotocols_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETPKCS10REQUEST:
    {
      signals_->getpkcs10request_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETRELAYOUTPUTS:
    {
      signals_->getrelayoutputs_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETNTP:
    {
      signals_->getntp_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETREMOTEDISCOVERYMODE:
    {
      signals_->getremotediscoverymode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETREMOTEUSER:
    {
      signals_->getremoteuser_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSCOPES:
    {
      signals_->getscopes_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSERVICES:
    {
      signals_->getservices_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSTORAGECONFIGURATION:
    {
      signals_->getstorageconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSTORAGECONFIGURATIONS:
    {
      signals_->getstorageconfigurations_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMBACKUP:
    {
      signals_->getsystembackup_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMDATEANDTIME:
    {
      signals_->getsystemdateandtime_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMLOG:
    {
      signals_->getsystemlog_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMSUPPORTINFORMATION:
    {
      signals_->getsystemsupportinformation_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETSYSTEMURIS:
    {
      signals_->getsystemuris_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETUSERS:
    {
      signals_->getusers_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETWSDLURL:
    {
      signals_->getwsdlurl_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_GETZEROCONFIGURATION:
    {
      signals_->getzeroconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_LOADCACERTIFICATES:
    {
      signals_->loadcacertificates_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_LOADCERTIFICATES:
    {
      signals_->loadcertificates_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_LOADCERTIFICATEWITHPRIVATEKEY:
    {
      signals_->loadcertificatewithprivatekey_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_REMOVEIPADDRESSFILTER:
    {
      signals_->removeipaddressfilter_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_REMOVESCOPES:
    {
      signals_->removescopes_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_RESTORESYSTEM:
    {
      signals_->restoresystem_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SCANAVAILABLEDOT11NETWORKS:
    {
      signals_->scanavailabledot11networks_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SENDAUXILIARYCOMMAND:
    {
      signals_->sendauxiliarycommand_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETACCESSPOLICY:
    {
      signals_->setaccesspolicy_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETCERTIFICATESSTATUS:
    {
      signals_->setcertificatesstatus_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETCLIENTCERTIFICATEMODE:
    {
      signals_->setclientcertificatemode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETDISCOVERYMODE:
    {
      signals_->setdiscoverymode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETDNS:
    {
      signals_->setdns_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETDOT1XCONFIGURATION:
    {
      signals_->setdot1xconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETDPADDRESSES:
    {
      signals_->setdpaddresses_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETDYNAMICDNS:
    {
      signals_->setdynamicdns_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETHOSTNAME:
    {
      signals_->sethostname_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETHOSTNAMEFROMDHCP:
    {
      signals_->sethostnamefromdhcp_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETIPADDRESSFILTER:
    {
      signals_->setipaddressfilter_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETNETWORKDEFAULTGATEWAY:
    {
      signals_->setnetworkdefaultgateway_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETNETWORKINTERFACES:
    {
      signals_->setnetworkinterfaces_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETNETWORKPROTOCOLS:
    {
      signals_->setnetworkprotocols_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETNTP:
    {
      signals_->setntp_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETRELAYOUTPUTSETTINGS:
    {
      signals_->setrelayoutputsettings_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETREMOTEDISCOVERYMODE:
    {
      signals_->setremotediscoverymode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETSYSTEMDATEANDTIME:
    {
      signals_->setsystemdateandtime_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETSYSTEMFACTORYDEFAULT:
    {
      signals_->setsystemfactorydefault_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETUSER:
    {
      signals_->setuser_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SETZEROCONFIGURATION:
    {
      signals_->setzeroconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEOPERATION_SYSTEMREBOOT:
    {
      signals_->systemreboot_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string DeviceClient::AddIPAddressFilterBody(const IPAddressFilter& ipaddressfilter)
{
  return std::string("<s:Body><tds:AddIPAddressFilter>" + ipaddressfilter.ToXml("tds:IPAddressFilter") + "</tds:AddIPAddressFilter></s:Body>");
}

std::string DeviceClient::AddScopesBody(const std::vector<std::string>& scopes)
{
  return std::string("<s:Body><tds:AddScopes>" + ToXml("tds:ScopeItem", scopes) + "</tds:AddScopes></s:Body>");
}

std::string DeviceClient::CreateCertificateBody(const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter)
{
  return std::string("<s:Body><tds:CreateCertificate>" + ToXml("tds:CertificateID", certificateid) + ToXml("tds:Subject", subject) + (validnotbefore.is_initialized() ? validnotbefore->ToXml("tds:ValidNotBefore") : std::string()) + (validnotafter.is_initialized() ? validnotafter->ToXml("tds:ValidNotAfter") : std::string()) + "</tds:CreateCertificate></s:Body>");
}

std::string DeviceClient::CreateStorageConfigurationBody(const StorageConfigurationData& storageconfiguration)
{
  return std::string("<s:Body><tds:CreateStorageConfiguration>" + storageconfiguration.ToXml("tds:StorageConfiguration") + "</tds:CreateStorageConfiguration></s:Body>");
}

std::string DeviceClient::CreateUsersBody(const std::vector<User>& users)
{
  return std::string("<s:Body><tds:CreateUsers>" + ToXml("tds:User", users) + "</tds:CreateUsers></s:Body>");
}

std::string DeviceClient::DeleteCertificatesBody(const std::vector<std::string>& certificateid)
{
  return std::string("<s:Body><tds:DeleteCertificates>" + ToXml("tds:CertificateID", certificateid) + "</tds:DeleteCertificates></s:Body>");
}

std::string DeviceClient::DeleteDot1XConfigurationBody(const std::vector<std::string>& dot1xconfigurationtoken)
{
  return std::string("<s:Body><tds:DeleteDot1XConfiguration>" + ToXml("tds:Dot1XConfigurationToken", dot1xconfigurationtoken) + "</tds:DeleteDot1XConfiguration></s:Body>");
}

std::string DeviceClient::DeleteStorageConfigurationBody(const std::vector<std::string>& token)
{
  return std::string("<s:Body><tds:DeleteStorageConfiguration>" + ToXml("tds:Token", token) + "</tds:DeleteStorageConfiguration></s:Body>");
}

std::string DeviceClient::DeleteUsersBody(const std::vector<std::string>& usernames)
{
  return std::string("<s:Body><tds:DeleteUsers>" + ToXml("tds:Username", usernames) + "</tds:DeleteUsers></s:Body>");
}

std::string DeviceClient::GetAccessPolicyBody()
{
  return std::string("<s:Body><tds:GetAccessPolicy/></s:Body>");
}

std::string DeviceClient::GetCACertificatesBody()
{
  return std::string("<s:Body><tds:GetCACertificates/></s:Body>");
}

std::string DeviceClient::GetCapabilitiesBody(CAPABILITYCATEGORY capabilitycategory)
{
  return std::string("<s:Body><tds:GetCapabilities>" + ToXmlEnum("tds:Category", capabilitycategory) + "</tds:GetCapabilities></s:Body>");
}

std::string DeviceClient::GetCertificateInformationBody(const std::string& certificateid)
{
  return std::string("<s:Body><tds:GetCertificateInformation>" + ToXml("tds:CertificateID", boost::optional<std::string>(certificateid)) + "</tds:GetCertificateInformation></s:Body>");
}

std::string DeviceClient::GetCertificatesBody()
{
  return std::string("<s:Body><tds:GetCertificates/></s:Body>");
}

std::string DeviceClient::GetCertificatesStatusBody()
{
  return std::string("<s:Body><tds:GetCertificatesStatus/></s:Body>");
}

std::string DeviceClient::GetClientCertificateModeBody()
{
  return std::string("<s:Body><tds:GetClientCertificateMode/></s:Body>");
}

std::string DeviceClient::GetDeviceInformationBody()
{
  return std::string("<s:Body><tds:GetDeviceInformation/></s:Body>");
}

std::string DeviceClient::GetDiscoveryModeBody()
{
  return std::string("<s:Body><tds:GetDiscoveryMode/></s:Body>");
}

std::string DeviceClient::GetDNSBody()
{
  return std::string("<s:Body><tds:GetDNS/></s:Body>");
}

std::string DeviceClient::GetDot11CapabilitiesBody()
{
  return std::string("<s:Body><tds:GetDot11Capabilities/></s:Body>");
}

std::string DeviceClient::GetDot11StatusBody(const std::string& interfacetoken)
{
  return (std::string("<s:Body><tds:GetDot11Status><tds:InterfaceToken>") + interfacetoken + std::string("</tds:InterfaceToken></tds:GetDot11Status></s:Body>"));
}

std::string DeviceClient::GetDot1XConfigurationBody(const std::string& dot1xconfigurationtoken)
{
  return (std::string("<s:Body><tds:GetDot1XConfiguration><tds:Dot1XConfigurationToken>") + dot1xconfigurationtoken + std::string("</tds:Dot1XConfigurationToken></tds:GetDot1XConfiguration></s:Body>"));
}

std::string DeviceClient::GetDot1XConfigurationsBody()
{
  return std::string("<s:Body><tds:GetDot1XConfigurations/></s:Body>");
}

std::string DeviceClient::GetDPAddressesBody()
{
  return std::string("<s:Body><tds:GetDPAddresses/></s:Body>");
}

std::string DeviceClient::GetDynamicDNSBody()
{
  return std::string("<s:Body><tds:GetDynamicDNS/></s:Body>");
}

std::string DeviceClient::GetEndpointReferenceBody()
{
  return std::string("<s:Body><tds:GetEndpointReference/></s:Body>");
}

std::string DeviceClient::GetHostnameBody()
{
  return std::string("<s:Body><tds:GetHostname/></s:Body>");
}

std::string DeviceClient::GetIPAddressFilterBody()
{
  return std::string("<s:Body><tds:GetIPAddressFilter/></s:Body>");
}

std::string DeviceClient::GetNetworkDefaultGatewayBody()
{
  return std::string("<s:Body><tds:GetNetworkDefaultGateway/></s:Body>");
}

std::string DeviceClient::GetNetworkInterfacesBody()
{
  return std::string("<s:Body><tds:GetNetworkInterfaces/></s:Body>");
}

std::string DeviceClient::GetNetworkProtocolsBody()
{
  return std::string("<s:Body><tds:GetNetworkProtocols/></s:Body>");
}

std::string DeviceClient::GetPkcs10RequestBody(const std::string& certificateid, const boost::optional<std::string>& subject, const boost::optional<BinaryData>& attributes)
{
  return std::string("<s:Body><tds:GetPkcs10Request>") + ToXml("tds:CertificateID", boost::optional<std::string>(certificateid)) + ToXml("tds:Subject", boost::optional<std::string>(subject)) + onvif::ToXmlClass("tds:Attributes", attributes) + ("</tds:GetPkcs10Request></s:Body>");
}

std::string DeviceClient::GetRelayOutputsBody()
{
  return std::string("<s:Body><tds:GetRelayOutputs/></s:Body>");
}

std::string DeviceClient::GetNTPBody()
{
  return std::string("<s:Body><tds:GetNTP/></s:Body>");
}

std::string DeviceClient::GetRemoteDiscoveryModeBody()
{
  return std::string("<s:Body><tds:GetRemoteDiscoveryMode/></s:Body>");
}

std::string DeviceClient::GetRemoteUserBody()
{
  return std::string("<s:Body><tds:GetRemoteUser/></s:Body>");
}

std::string DeviceClient::GetScopesBody()
{
  return std::string("<s:Body><tds:GetScopes/></s:Body>");
}

std::string DeviceClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><tds:GetServiceCapabilities/></s:Body>");
}

std::string DeviceClient::GetServicesBody(bool includecapability)
{
  return std::string("<s:Body><tds:GetServices>" + ToXml("tds:IncludeCapability", boost::optional<bool>(includecapability)) + "</tds:GetServices></s:Body>");
}

std::string DeviceClient::GetStorageConfigurationBody(const std::string& token)
{
  return std::string("<s:Body><tds:GetStorageConfiguration>" + ToXml("tds:Token", boost::optional<std::string>(token)) + "</tds:GetStorageConfiguration></s:Body>");
}

std::string DeviceClient::GetStorageConfigurationsBody()
{
  return std::string("<s:Body><tds:GetStorageConfigurations/></s:Body>");
}

std::string DeviceClient::GetSystemBackupBody()
{
  return std::string("<s:Body><tds:GetSystemBackup/></s:Body>");
}

std::string DeviceClient::GetSystemDateAndTimeBody()
{
  return std::string("<s:Body><tds:GetSystemDateAndTime/></s:Body>");
}

std::string DeviceClient::GetSystemLogBody(SYSTEMLOGTYPE logtype)
{
  return std::string("<s:Body><tds:GetSystemLog>" + ToXmlEnum("tds:LogType", logtype) + "</tds:GetSystemLog></s:Body>");
}

std::string DeviceClient::GetSystemSupportInformationBody()
{
  return std::string("<s:Body><tds:GetSystemSupportInformation/></s:Body>");
}

std::string DeviceClient::GetSystemUrisBody()
{
  return std::string("<s:Body><tds:GetSystemUris/></s:Body>");
}

std::string DeviceClient::GetUsersBody()
{
  return std::string("<s:Body><tds:GetUsers/></s:Body>");
}

std::string DeviceClient::GetWsdlUrlBody()
{
  return std::string("<s:Body><tds:GetWsdlUrl/></s:Body>");
}

std::string DeviceClient::GetZeroConfigurationBody()
{
  return std::string("<s:Body><tds:GetZeroConfiguration/></s:Body>");
}

std::string DeviceClient::LoadCACertificatesBody(const std::vector<Certificate>& cacertificate)
{
  return std::string("<s:Body><tds:LoadCACertificates>" + ToXml("tds:CACertificate", cacertificate) + "</tds:LoadCACertificates></s:Body>");
}

std::string DeviceClient::LoadCertificatesBody(const std::vector<Certificate>& nvtcertificate)
{
  return std::string("<s:Body><tds:LoadCertificates>" + ToXml("tds:NVTCertificate", nvtcertificate) + "</tds:LoadCertificates></s:Body>");
}

std::string DeviceClient::LoadCertificateWithPrivateKeyBody(const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey)
{
  return std::string("<s:Body><tds:LoadCertificateWithPrivateKey>" + ToXml("tds:CertificateWithPrivateKey", certificatewithprivatekey) + "</tds:LoadCertificateWithPrivateKey></s:Body>");
}

std::string DeviceClient::RemoveIPAddressFilterBody(const IPAddressFilter& ipaddressfilter)
{
  return std::string("<s:Body><tds:RemoveIPAddressFilter>" + ipaddressfilter.ToXml("tds:IPAddressFilter") + "</tds:RemoveIPAddressFilter></s:Body>");
}

std::string DeviceClient::RemoveScopesBody(const std::vector<std::string>& scopes)
{
  return std::string("<s:Body><tds:RemoveScopes>" + ToXml("tds:ScopeItem", scopes) + "</tds:RemoveScopes></s:Body>");
}

std::pair<std::string, std::map<std::string, std::vector<char> > > DeviceClient::RestoreSystemBody(std::vector<BackupFile> backupfiles)
{
  const boost::regex cid("[\\s]*cid[\\s]*:[\\s]*(.*)");

  std::map<std::string, std::vector<char> > mtomdata;
  for (auto& backupfile : backupfiles)
  {
    if (!backupfile.data_.is_initialized() || !backupfile.data_->include_.is_initialized() || !backupfile.data_->include_->href_.is_initialized() || backupfile.data_->include_->href_->empty())
    {
      
      continue;
    }

    // Remove cid: prefix if it exists
    std::string contentid = *backupfile.data_->include_->href_;
    boost::smatch match;
    if (boost::regex_search(*backupfile.data_->include_->href_, match, cid))
    {
      contentid = match[1].str();

    }

    mtomdata[contentid] = backupfile.data_->include_->data_;
    backupfile.data_->include_->data_.clear();
  }

  return std::make_pair(std::string("<s:Body><tds:RestoreSystem>" + ToXml("tds:BackupFiles", backupfiles) + "</tds:RestoreSystem></s:Body>"), mtomdata);
}

std::string DeviceClient::ScanAvailableDot11NetworksBody(const std::string& interfacetoken)
{
  return std::string("<s:Body><tds:ScanAvailableDot11Networks><tt:InterfaceToken>" + interfacetoken + "</tt:InterfaceToken></tds:ScanAvailableDot11Networks></s:Body>");
}

std::string DeviceClient::SendAuxiliaryCommandBody(const std::string& auxiliarycommand)
{
  return std::string("<s:Body><tds:SendAuxiliaryCommand><tt:AuxiliaryCommand>" + auxiliarycommand + "</tt:AuxiliaryCommand></tds:SendAuxiliaryCommand></s:Body>");
}

std::string DeviceClient::SetAccessPolicyBody(const BinaryData& policyfile)
{
  return std::string("<s:Body><tds:SetAccessPolicy>" + onvif::ToXmlClass("tds:PolicyFile", boost::optional<BinaryData>(policyfile)) + "</tds:SetAccessPolicy></s:Body>");
}

std::string DeviceClient::SetCertificatesStatusBody(const std::vector<CertificateStatus>& certificatestatus)
{
  return std::string("<s:Body><tds:SetCertificatesStatus>" + onvif::ToXml("tds:CertificateStatus", certificatestatus) + "</tds:SetCertificatesStatus></s:Body>");
}

std::string DeviceClient::SetClientCertificateModeBody(bool enabled)
{
  return std::string("<s:Body><tds:SetClientCertificateMode>" + onvif::ToXml("tds:Enabled", boost::optional<bool>(enabled)) + "</tds:SetClientCertificateMode></s:Body>");
}

std::string DeviceClient::SetDiscoveryModeBody(DISCOVERYMODE discoverymode)
{
  return std::string("<s:Body><tds:SetDiscoveryMode><tds:DiscoveryMode>" + onvif::ToString(discoverymode) + "</tds:DiscoveryMode></tds:SetDiscoveryMode></s:Body>");
}

std::string DeviceClient::SetDNSBody(bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual)
{
  return std::string("<s:Body><tds:SetDNS>" + ToXml("tds:FromDHCP", boost::optional<bool>(fromdhcp)) + ToXml("tds:SearchDomain", searchdomain) + ToXml("tds:DNSManual", dnsmanual) + "</tds:SetDNS></s:Body>");
}

std::string DeviceClient::SetDot1XConfigurationBody(const onvif::Dot1XConfiguration& dot1xconfiguration)
{
  return std::string("<s:Body><tds:SetDot1XConfiguration>" + onvif::ToXmlClass("tds:Dot1XConfiguration", boost::optional<Dot1XConfiguration>(dot1xconfiguration)) + "</tds:SetDot1XConfiguration></s:Body>");
}

std::string DeviceClient::SetDPAddressesBody(const std::vector<NetworkHost>& dpaddress)
{
  return std::string("<s:Body><tds:SetDPAddresses>" + ToXml("tds:DPAddress", dpaddress) + "</tds:SetDPAddresses></s:Body>");
}

std::string DeviceClient::SetDynamicDNSBody(DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl)
{
  return std::string("<s:Body><tds:SetDynamicDNS>" + ToXmlEnum("tds:Type", type) + ToXml("tds:Name", name) + ToXml("tds:TTL", ttl) + "</tds:SetDynamicDNS></s:Body>");
}

std::string DeviceClient::SetHostnameBody(const std::string& name)
{
  return std::string("<s:Body><tds:SetHostname>" + ToXml("tds:Name", boost::optional<std::string>(name)) + "</tds:SetHostname></s:Body>");
}

std::string DeviceClient::SetHostnameFromDHCPBody(bool fromdhcp)
{
  return std::string("<s:Body><tds:SetHostnameFromDHCP>" + ToXml("tds:FromDHCP", boost::optional<bool>(fromdhcp)) + "</tds:SetHostnameFromDHCP></s:Body>");
}

std::string DeviceClient::SetIPAddressFilterBody(const IPAddressFilter& ipaddressfilter)
{
  return std::string("<s:Body><tds:SetIPAddressFilter>" + ipaddressfilter.ToXml("tds:IPAddressFilter") + "</tds:SetIPAddressFilter></s:Body>");
}

std::string DeviceClient::SetNetworkDefaultGatewayBody(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address)
{
  return std::string("<s:Body><tds:SetNetworkDefaultGateway>" + ToXml("tds:IPv4Address", ipv4address) + ToXml("tds:IPv6Address", ipv6address) + "</tds:SetNetworkDefaultGateway></s:Body>");
}

std::string DeviceClient::SetNetworkInterfacesBody(const std::string& token, const onvif::NetworkInterfaceSetConfiguration& networkinterface)
{
  return std::string("<s:Body><tds:SetNetworkInterfaces>" + ToXml("tds:InterfaceToken", boost::optional<std::string>(token)) + onvif::ToXmlClass("tds:NetworkInterface", boost::optional<NetworkInterfaceSetConfiguration>(networkinterface)) + "</tds:SetNetworkInterfaces></s:Body>");
}

std::string DeviceClient::SetNetworkProtocolsBody(const std::vector<NetworkProtocol>& networkprotocols)
{
  return std::string("<s:Body><tds:SetNetworkProtocols>" + ToXml("tds:NetworkProtocols", networkprotocols) + "</tds:SetNetworkProtocols></s:Body>");
}

std::string DeviceClient::SetNTPBody(bool fromdhcp, const std::vector<NetworkHost>& ntpmanual)
{
  return std::string("<s:Body><tds:SetNTP>" + onvif::ToXml("tds:FromDHCP", boost::optional<bool>(fromdhcp)) + ToXml("tds:NTPManual", ntpmanual) + "</tds:SetNTP></s:Body>");
}

std::string DeviceClient::SetRelayOutputSettingsBody(const std::string& relayoutputtoken, const RelayOutputSettings& properties)
{
  return std::string("<s:Body><tds:SetRelayOutputSettings>" + onvif::ToXml("tt:RelayOutputToken", boost::optional<std::string>(relayoutputtoken)) + properties.ToXml("tt:Properties") + "</tds:SetRelayOutputSettings></s:Body>");
}

std::string DeviceClient::SetRemoteDiscoveryModeBody(DISCOVERYMODE remotediscoverymode)
{
  return std::string("<s:Body><tds:SetRemoteDiscoveryMode><tds:RemoteDiscoveryMode>" + onvif::ToString(remotediscoverymode) + "</tds:RemoteDiscoveryMode></tds:SetRemoteDiscoveryMode></s:Body>");
}

std::string DeviceClient::SetSystemDateAndTimeBody(DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime)
{
  return std::string("<s:Body><tds:SetSystemDateAndTime>" + ToXmlEnum("tds:DateTimeType", datetimetype) + ToXml("tds:DaylightSavings", boost::optional<bool>(daylightsavings)) + onvif::ToXmlClass("tds:TimeZone", timezone) + onvif::ToXmlClass("tds:UTCDateTime", utcdatetime) + "</tds:SetSystemDateAndTime></s:Body>");
}

std::string DeviceClient::SetSystemFactoryDefaultBody(FACTORYDEFAULTTYPE factorydefault)
{
  return std::string("<s:Body><tds:SetSystemFactoryDefault>" + ToXmlEnum("tds:FactoryDefault", factorydefault) + "</tds:SetSystemFactoryDefault></s:Body>");
}

std::string DeviceClient::SetUserBody(const std::vector<User>& users)
{
  return std::string("<s:Body><tds:SetUser>" + ToXml("tds:User", users) + "</tds:SetUser></s:Body>");
}

std::string DeviceClient::SetZeroConfigurationBody(const std::string& interfacetoken, bool enabled)
{
  return std::string("<s:Body><tds:SetZeroConfiguration>" + ToXml("tds:InterfaceToken", boost::optional<std::string>(interfacetoken)) + ToXml("tds:Enabled", boost::optional<bool>(enabled)) + "</tds:SetZeroConfiguration></s:Body>");
}

std::string DeviceClient::SystemRebootBody()
{
  return std::string("<s:Body><tds:SystemReboot/></s:Body>");
}

}

}
