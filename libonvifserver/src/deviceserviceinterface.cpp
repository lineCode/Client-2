// deviceserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/deviceserviceinterface.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <onviftypes/onviftypes.hpp>
#include <pugixml.hpp>

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace device
{

///// Globals /////

const std::string DEVICENAMESPACEURI("http://www.onvif.org/ver10/device/wsdl");

const std::string ADDIPADDRESSFILTER("AddIPAddressFilter");
const std::string ADDSCOPES("AddScopes");
const std::string CREATECERTIFICATE("CreateCertificate");
const std::string CREATEDOT1XCONFIGURATION("CreateDot1XConfiguration");
const std::string CREATESTORAGECONFIGURATION("CreateStorageConfiguration");
const std::string CREATEUSERS("CreateUsers");
const std::string DELETECERTIFICATES("DeleteCertificates");
const std::string DELETEDOT1XCONFIGURATION("DeleteDot1XConfiguration");
const std::string DELETEGEOLOCATION("DeleteGeoLocation");
const std::string DELETESTORAGECONFIGURATION("DeleteStorageConfiguration");
const std::string DELETEUSERS("DeleteUsers");
const std::string GETACCESSPOLICY("GetAccessPolicy");
const std::string GETCACERTIFICATES("GetCACertificates");
const std::string GETCAPABILITIES("GetCapabilities");
const std::string GETCERTIFICATEINFORMATION("GetCertificateInformation");
const std::string GETCERTIFICATES("GetCertificates");
const std::string GETCERTIFICATESSTATUS("GetCertificatesStatus");
const std::string GETCLIENTCERTIFICATEMODE("GetClientCertificateMode");
const std::string GETDEVICEINFORMATION("GetDeviceInformation");
const std::string GETDISCOVERYMODE("GetDiscoveryMode");
const std::string GETDNS("GetDNS");
const std::string GETDOT11CAPABILITIES("GetDot11Capabilities");
const std::string GETDOT11STATUS("GetDot11Status");
const std::string GETDOT1XCONFIGURATION("GetDot1XConfiguration");
const std::string GETDOT1XCONFIGURATIONS("GetDot1XConfigurations");
const std::string GETDPADDRESSES("GetDPAddresses");
const std::string GETDYNAMICDNS("GetDynamicDNS");
const std::string GETENDPOINTREFERENCE("GetEndpointReference");
const std::string GETGEOLOCATION("GetGeoLocation");
const std::string GETHOSTNAME("GetHostname");
const std::string GETIPADDRESSFILTER("GetIPAddressFilter");
const std::string GETNETWORKDEFAULTGATEWAY("GetNetworkDefaultGateway");
const std::string GETNETWORKINTERFACES("GetNetworkInterfaces");
const std::string GETNETWORKPROTOCOLS("GetNetworkProtocols");
const std::string GETNTP("GetNTP");
const std::string GETPKCS10REQUEST("GetPkcs10Request");
const std::string GETRELAYOUTPUTS("GetRelayOutputs");
const std::string GETREMOTEDISCOVERYMODE("GetRemoteDiscoveryMode");
const std::string GETREMOTEUSER("GetRemoteUser");
const std::string GETSCOPES("GetScopes");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");
const std::string GETSERVICES("GetServices");
const std::string GETSTORAGECONFIGURATION("GetStorageConfiguration");
const std::string GETSTORAGECONFIGURATIONS("GetStorageConfigurations");
const std::string GETSYSTEMBACKUP("GetSystemBackup");
const std::string GETSYSTEMDATEANDTIME("GetSystemDateAndTime");
const std::string GETSYSTEMLOG("GetSystemLog");
const std::string GETSYSTEMSUPPORTINFORMATION("GetSystemSupportInformation");
const std::string GETSYSTEMURIS("GetSystemUris");
const std::string GETUSERS("GetUsers");
const std::string GETWSDLURL("GetWsdlUrl");
const std::string GETZEROCONFIGURATION("GetZeroConfiguration");
const std::string LOADCACERTIFICATES("LoadCACertificates");
const std::string LOADCERTIFICATES("LoadCertificates");
const std::string LOADCERTIFICATEWITHPRIVATEKEY("LoadCertificateWithPrivateKey");
const std::string REMOVEIPADDRESSFILTER("RemoveIPAddressFilter");
const std::string REMOVESCOPES("RemoveScopes");
const std::string RESTORESYSTEM("RestoreSystem");
const std::string SCANAVAILABLEDOT11NETWORKS("ScanAvailableDot11Networks");
const std::string SENDAUXILIARYCOMMAND("SendAuxiliaryCommand");
const std::string SETACCESSPOLICY("SetAccessPolicy");
const std::string SETCERTIFICATESSTATUS("SetCertificatesStatus");
const std::string SETCLIENTCERTIFICATEMODE("SetClientCertificateMode");
const std::string SETDISCOVERYMODE("SetDiscoveryMode");
const std::string SETDNS("SetDNS");
const std::string SETDOT1XCONFIGURATION("SetDot1XConfiguration");
const std::string SETDPADDRESSES("SetDPAddresses");
const std::string SETDYNAMICDNS("SetDynamicDNS");
const std::string SETGEOLOCATION("SetGeoLocation");
const std::string SETHOSTNAME("SetHostname");
const std::string SETHOSTNAMEFROMDHCP("SetHostnameFromDHCP");
const std::string SETIPADDRESSFILTER("SetIPAddressFilter");
const std::string SETNETWORKDEFAULTGATEWAY("SetNetworkDefaultGateway");
const std::string SETNETWORKINTERFACES("SetNetworkInterfaces");
const std::string SETNETWORKPROTOCOLS("SetNetworkProtocols");
const std::string SETNTP("SetNTP");
const std::string SETRELAYOUTPUTSETTINGS("SetRelayOutputSettings");
const std::string SETRELAYOUTPUTSTATE("SetRelayOutputState");
const std::string SETREMOTEDISCOVERYMODE("SetRemoteDiscoveryMode");
const std::string SETREMOTEUSER("SetRemoteUser");
const std::string SETSCOPES("SetScopes");
const std::string SETSTORAGECONFIGURATION("SetStorageConfiguration");
const std::string SETSYSTEMDATEANDTIME("SetSystemDateAndTime");
const std::string SETSYSTEMFACTORYDEFAULT("SetSystemFactoryDefault");
const std::string SETUSER("SetUser");
const std::string SETZEROCONFIGURATION("SetZeroConfiguration");
const std::string STARTFIRMWAREUPGRADE("StartFirmwareUpgrade");
const std::string STARTSYSTEMRESTORE("StartSystemRestore");
const std::string SYSTEMREBOOT("SystemReboot");
const std::string UPGRADESYSTEMFIRMWARE("UpgradeSystemFirmware");

///// Declarations /////

const std::string DeviceServiceInterface::XADDR_("/onvif/device_service");

///// Methods /////

DeviceServiceInterface::DeviceServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 21, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, DEVICE_NAMESPACE_ATTRIBUTE, XS_NAMESPACE_ATTRIBUTE }),
  cominitialised_(false)
{

}

DeviceServiceInterface::~DeviceServiceInterface()
{
  Destroy();

}

int DeviceServiceInterface::Init()
{
  Destroy();
#ifdef _WIN32
  // Stop the Windows time/NTP service
  utility::ServiceMgr servicemgr;
  if (servicemgr.Init(SC_MANAGER_ALL_ACCESS))
  {

    return 1;
  }

  auto timeservice = servicemgr.GetService(std::string("W32Time"), SERVICE_ALL_ACCESS);
  if (!timeservice)
  {

    return 1;
  }

  SERVICE_STATUS servicestatus;
  if (timeservice->GetStatus(servicestatus))
  {

    return 1;
  }

  if ((servicestatus.dwCurrentState != SERVICE_STOP_PENDING) && (servicestatus.dwCurrentState != SERVICE_STOPPED)) // If the service is not stopping or stopped, stop it now
  {
    if (timeservice->Stop())
    {

      return 1;
    }
  }
#endif
  return 0;
}

void DeviceServiceInterface::Destroy()
{
#ifdef _WIN32
  if (cominitialised_)
  {
    CoUninitialize();

  }
#endif
}

boost::shared_future<ServerResponse> DeviceServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != DEVICENAMESPACEURI)
  {
    
    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing device namespace uri")).ToString()));
  }

  if (action == ADDIPADDRESSFILTER)
  {
    const auto ipaddressfilter = operationnode.select_node("*[local-name()='IPAddressFilter']");
    if (!ipaddressfilter)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing IPAddressFilter argument")).ToString()));
    }

    return boost::make_ready_future(AddIPAddressFilter(onvif::IPAddressFilter(ipaddressfilter.node())));
  }
  else if (action == ADDSCOPES)
  {
    const auto scopeitemnodes = operationnode.select_nodes("*[local-name()='ScopeItem']");
    std::vector<std::string> scopeitems;
    for (const auto& scope : scopeitemnodes)
    {
      if (!scope.node())
      {

        continue;
      }

      scopeitems.push_back(scope.node().text().get());
    }

    return boost::make_ready_future(AddScopes(scopeitems));
  }
  else if (action == CREATECERTIFICATE)
  {
    const auto certificateidnode = operationnode.select_node("*[local-name()='CertificateID']");
    boost::optional<std::string> certificateid;
    if (certificateidnode)
    {
      certificateid = certificateidnode.node().text().get();

    }

    const auto subjectnode = operationnode.select_node("*[local-name()='Subject']");
    boost::optional<std::string> subject;
    if (subjectnode)
    {
      subject = subjectnode.node().text().get();

    }

    const auto validnotbeforenode = operationnode.select_node("*[local-name()='ValidNotBefore']");
    boost::optional<onvif::ws::DateTime> validnotbefore;
    if (validnotbeforenode)
    {
      validnotbefore = onvif::ws::DateTime(validnotbeforenode.node().text().get());

    }

    const auto validnotafternode = operationnode.select_node("*[local-name()='ValidNotAfter']");
    boost::optional<onvif::ws::DateTime> validnotafter;
    if (validnotafternode)
    {
      validnotafter = onvif::ws::DateTime(validnotafternode.node().text().get());

    }

    return boost::make_ready_future(CreateCertificate(certificateid, subject, validnotbefore, validnotafter));
  }
  else if (action == CREATEDOT1XCONFIGURATION)
  {
    const auto dot1xconfigurationnode = operationnode.select_node("*[local-name()='Dot1XConfiguration']");
    if (!dot1xconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Dot1XConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(CreateDot1XConfiguration(onvif::Dot1XConfiguration(dot1xconfigurationnode.node())));
  }
  else if (action == CREATESTORAGECONFIGURATION)
  {
    const auto storageconfigurationnode = operationnode.select_node("*[local-name()='StorageConfiguration']");
    if (!storageconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing StorageConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(CreateStorageConfiguration(onvif::device::StorageConfigurationData(storageconfigurationnode.node())));
  }
  else if (action == CREATEUSERS)
  {
    const auto usernodes = operationnode.select_nodes("*[local-name()='User']");
    std::vector<onvif::User> users;
    for (const auto& usernode : usernodes)
    {
      if (!usernode.node())
      {

        continue;
      }

      users.push_back(onvif::User(usernode.node()));
    }

    return boost::make_ready_future(CreateUsers(users));
  }
  else if (action == DELETECERTIFICATES)
  {
    const auto certificateidnodes = operationnode.select_nodes("*[local-name()='CertificateID']");
    std::vector<std::string> certificateids;
    for (const auto& certificateidnode : certificateidnodes)
    {
      if (!certificateidnode.node())
      {

        continue;
      }

      certificateids.push_back(certificateidnode.node().text().get());
    }

    return boost::make_ready_future(DeleteCertificates(certificateids));
  }
  else if (action == DELETEDOT1XCONFIGURATION)
  {
    const auto dot1xconfigurationtokennodes = operationnode.select_nodes("*[local-name()='Dot1XConfigurationToken']");
    std::vector<std::string> dot1xconfigurationtokens;
    for (const auto& dot1xconfigurationtokennode : dot1xconfigurationtokennodes)
    {
      if (!dot1xconfigurationtokennode.node())
      {

        continue;
      }

      dot1xconfigurationtokens.push_back(dot1xconfigurationtokennode.node().text().get());
    }

    return boost::make_ready_future(DeleteDot1XConfiguration(dot1xconfigurationtokens));
  }
  else if (action == DELETESTORAGECONFIGURATION)
  {
    const auto tokennode = operationnode.select_node("*[local-name()='Token']");
    if (!tokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Token argument")).ToString()));
    }

    return boost::make_ready_future(DeleteStorageConfiguration(tokennode.node().text().get()));
  }
  else if (action == DELETEUSERS)
  {
    const auto usernamenodes = operationnode.select_nodes("*[local-name()='Username']");
    std::vector<std::string> usernames;
    for (const auto& usernamenode : usernamenodes)
    {
      if (!usernamenode.node())
      {

        continue;
      }

      usernames.push_back(usernamenode.node().text().get());
    }

    return boost::make_ready_future(DeleteUsers(usernames));
  }
  else if (action == GETACCESSPOLICY)
  {

    return boost::make_ready_future(GetAccessPolicy());
  }
  else if (action == GETCACERTIFICATES)
  {

    return boost::make_ready_future(GetCACertificates());
  }
  else if (action == GETCAPABILITIES)
  {
    boost::optional<onvif::CAPABILITYCATEGORY> category;
    const auto categorynode = operationnode.select_node("*[local-name()='Category']");
    if (categorynode)
    {
      category = onvif::GetCapabilityCategory(categorynode.node().text().get());
      if (!category.is_initialized())
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid Category argument")).ToString()));
      }
    }

    return boost::make_ready_future(GetCapabilities(localaddress, port, category));
  }
  else if (action == GETCERTIFICATEINFORMATION)
  {

    return boost::make_ready_future(GetCertificateInformation());
  }
  else if (action == GETCERTIFICATES)
  {

    return boost::make_ready_future(GetCertificates());
  }
  else if (action == GETCERTIFICATESSTATUS)
  {

    return boost::make_ready_future(GetCertificatesStatus());
  }
  else if (action == GETCLIENTCERTIFICATEMODE)
  {

    return boost::make_ready_future(GetClientCertificateMode());
  }
  else if (action == GETDEVICEINFORMATION)
  {

    return boost::make_ready_future(GetDeviceInformation());
  }
  else if (action == GETDISCOVERYMODE)
  {

    return boost::make_ready_future(GetDiscoveryMode());
  }
  else if (action == GETDNS)
  {

    return boost::make_ready_future(GetDNS());
  }
  else if (action == GETDOT11CAPABILITIES)
  {

    return boost::make_ready_future(GetDot11Capabilities());
  }
  else if (action == GETDOT11STATUS)
  {
    const auto interfacetokennode = operationnode.select_node("*[local-name()='InterfaceToken']");
    if (!interfacetokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing InterfaceToken argument")).ToString()));
    }

    return boost::make_ready_future(GetDot11Status(interfacetokennode.node().text().get()));
  }
  else if (action == GETDOT1XCONFIGURATION)
  {
    const auto dot1xconfigurationtokennode = operationnode.select_node("*[local-name()='Dot1XConfigurationToken']");
    if (!dot1xconfigurationtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Dot1XConfigurationToken argument")).ToString()));
    }

    return boost::make_ready_future(GetDot1XConfiguration(dot1xconfigurationtokennode.node().text().get()));
  }
  else if (action == GETDOT1XCONFIGURATIONS)
  {

    return boost::make_ready_future(GetDot1XConfigurations());
  }
  else if (action == GETDPADDRESSES)
  {

    return boost::make_ready_future(GetDPAddresses());
  }
  else if (action == GETDYNAMICDNS)
  {

    return boost::make_ready_future(GetDynamicDNS());
  }
  else if (action == GETENDPOINTREFERENCE)
  {

    return boost::make_ready_future(GetEndpointReference());
  }
  else if (action == GETHOSTNAME)
  {

    return boost::make_ready_future(GetHostname());
  }
  else if (action == GETIPADDRESSFILTER)
  {

    return boost::make_ready_future(GetIPAddressFilter());
  }
  else if (action == GETNETWORKDEFAULTGATEWAY)
  {

    return boost::make_ready_future(GetNetworkDefaultGateway());
  }
  else if (action == GETNETWORKINTERFACES)
  {

    return boost::make_ready_future(GetNetworkInterfaces());
  }
  else if (action == GETNETWORKPROTOCOLS)
  {

    return boost::make_ready_future(GetNetworkProtocols());
  }
  else if (action == GETNTP)
  {

    return boost::make_ready_future(GetNTP());
  }
  else if (action == GETPKCS10REQUEST)
  {
    const auto certificateidnode = operationnode.select_node("*[local-name()='CertificateID']");
    if (!certificateidnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing CertificateID argument")).ToString()));
    }

    const auto subjectnode = operationnode.select_node("*[local-name()='Subject']");
    boost::optional<std::string> subject;
    if (subjectnode)
    {
      subject = subjectnode.node().text().get();

    }

    const auto attributesnode = operationnode.select_node("*[local-name()='Attributes']");
    boost::optional<onvif::BinaryData> attributes;
    if (attributesnode)
    {
      attributes = onvif::BinaryData(attributesnode.node());

    }

    return boost::make_ready_future(GetPkcs10Request(certificateidnode.node().text().get(), subject, attributes));
  }
  else if (action == GETRELAYOUTPUTS)
  {

    return boost::make_ready_future(GetRelayOutputs());
  }
  else if (action == GETREMOTEDISCOVERYMODE)
  {

    return boost::make_ready_future(GetRemoteDiscoveryMode());
  }
  else if (action == GETREMOTEUSER)
  {

    return boost::make_ready_future(GetRemoteUser());
  }
  else if (action == GETSCOPES)
  {

    return boost::make_ready_future(GetScopes());
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else if (action == GETSERVICES)
  {
    const auto includecapabilitynode = operationnode.select_node("*[local-name()='IncludeCapability']");
    if (!includecapabilitynode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing IncludeCapability argument")).ToString()));
    }

    return boost::make_ready_future(GetServices(localaddress, port, includecapabilitynode.node().text().as_bool()));
  }
  else if (action == GETSTORAGECONFIGURATION)
  {
    const auto tokennode = operationnode.select_node("*[local-name()='Token']");
    if (!tokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Token argument")).ToString()));
    }

    return boost::make_ready_future(GetStorageConfiguration(tokennode.node().text().get()));
  }
  else if (action == GETSTORAGECONFIGURATIONS)
  {

    return boost::make_ready_future(GetStorageConfigurations());
  }
  else if (action == GETSYSTEMBACKUP)
  {

    return boost::make_ready_future(GetSystemBackup());
  }
  else if (action == GETSYSTEMDATEANDTIME)
  {

    return boost::make_ready_future(GetSystemDateAndTime());
  }
  else if (action == GETSYSTEMLOG)
  {
    const auto systemlognode = operationnode.select_node("*[local-name()='LogType']");
    if (!systemlognode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing LogType argument")).ToString()));
    }

    boost::optional<onvif::SYSTEMLOGTYPE> systemlogtype = onvif::GetSystemLogType(systemlognode.node().text().get());
    if (!systemlogtype.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid LogType argument")).ToString()));
    }

    return boost::make_ready_future(GetSystemLog(*systemlogtype));
  }
  else if (action == GETSYSTEMSUPPORTINFORMATION)
  {

    return boost::make_ready_future(GetSystemSupportInformation());
  }
  else if (action == GETSYSTEMURIS)
  {

    return boost::make_ready_future(GetSystemUris());
  }
  else if (action == GETUSERS)
  {

    return boost::make_ready_future(GetUsers());
  }
  else if (action == GETWSDLURL)
  {

    return boost::make_ready_future(GetWsdlUrl(localaddress, port));
  }
  else if (action == GETZEROCONFIGURATION)
  {

    return boost::make_ready_future(GetZeroConfiguration());
  }
  else if (action == LOADCACERTIFICATES)
  {
    const auto certificatenodes = operationnode.select_nodes("*[local-name()='Certificate']");
    std::vector<onvif::Certificate> cacertificates;
    for (const auto& certificatenode : certificatenodes)
    {
      if (!certificatenode.node())
      {

        continue;
      }

      cacertificates.push_back(onvif::Certificate(certificatenode.node()));
    }

    return boost::make_ready_future(LoadCACertificates(cacertificates));
  }
  else if (action == LOADCERTIFICATES)
  {
    const auto nvtcertificatenodes = operationnode.select_nodes("*[local-name()='NVTCertificate']");
    std::vector<onvif::Certificate> nvtcertificates;
    for (const auto& nvtcertificatenode : nvtcertificatenodes)
    {
      if (!nvtcertificatenode.node())
      {

        continue;
      }

      nvtcertificates.push_back(onvif::Certificate(nvtcertificatenode.node()));
    }

    return boost::make_ready_future(LoadCertificates(nvtcertificates));
  }
  else if (action == LOADCERTIFICATEWITHPRIVATEKEY)
  {
    const auto certificatewithprivatekeynodes = operationnode.select_nodes("*[local-name()='CertificateWithPrivateKey']");
    std::vector<onvif::CertificateWithPrivateKey> certificatewithprivatekeys;
    for (const auto& certificatewithprivatekeynode : certificatewithprivatekeynodes)
    {
      if (!certificatewithprivatekeynode.node())
      {

        continue;
      }

      certificatewithprivatekeys.push_back(onvif::CertificateWithPrivateKey(certificatewithprivatekeynode.node()));
    }

    return boost::make_ready_future(LoadCertificateWithPrivateKey(certificatewithprivatekeys));
  }
  else if (action == REMOVEIPADDRESSFILTER)
  {

    const auto ipaddressfilternode = operationnode.select_node("*[local-name()='IPAddressFilter']");
    if (!ipaddressfilternode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing IPAddressFilter argument")).ToString()));
    }

    return boost::make_ready_future(RemoveIPAddressFilter(onvif::IPAddressFilter(ipaddressfilternode.node())));
  }
  else if (action == REMOVESCOPES)
  {
    const auto scopeitemnodes = operationnode.select_nodes("*[local-name()='ScopeItem']");
    std::vector<std::string> scopeitems;
    for (const auto& scopeitemnode : scopeitemnodes)
    {
      if (!scopeitemnode.node())
      {

        continue;
      }

      scopeitems.push_back(std::string(scopeitemnode.node().text().get()));
    }

    return boost::make_ready_future(RemoveScopes(scopeitems));
  }
  else if (action == RESTORESYSTEM)
  {
    const auto backupfilesnodes = operationnode.select_nodes("*[local-name()='BackupFiles']");
    std::vector<onvif::BackupFile> backupfiles;
    for (const auto& backupfilesnode : backupfilesnodes)
    {
      if (!backupfilesnode.node())
      {

        continue;
      }

      backupfiles.push_back(onvif::BackupFile(backupfilesnode.node()));
    }

    return boost::make_ready_future(RestoreSystem(backupfiles));
  }
  else if (action == SCANAVAILABLEDOT11NETWORKS)
  {
    const auto interfacetokennode = operationnode.select_node("*[local-name()='InterfaceToken']");
    if (!interfacetokennode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing InterfaceToken argument")).ToString()));
    }

    return boost::make_ready_future(ScanAvailableDot11Networks(interfacetokennode.node().text().get()));
  }
  else if (action == SENDAUXILIARYCOMMAND)
  {
    const auto auxiliarycommandnode = operationnode.select_node("*[local-name()='AuxiliaryCommand']");
    if (!auxiliarycommandnode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing AuxiliaryCommand argument")).ToString()));
    }

    return boost::make_ready_future(SendAuxiliaryCommand(auxiliarycommandnode.node().text().get()));
  }
  else if (action == SETACCESSPOLICY)
  {
    const auto policyfilenode = operationnode.select_node("*[local-name()='PolicyFile']");
    if (!policyfilenode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing PolicyFile argument")).ToString()));
    }

    return boost::make_ready_future(SetAccessPolicy(onvif::BinaryData(policyfilenode.node())));
  }
  else if (action == SETCERTIFICATESSTATUS)
  {
    const auto certificatestatusnodes = operationnode.select_nodes("*[local-name()='CertificateStatus']");
    std::vector<onvif::CertificateStatus> certificatestatus;
    for (const auto& certificatestatusnode : certificatestatusnodes)
    {
      if (!certificatestatusnode.node())
      {

        continue;
      }

      certificatestatus.push_back(onvif::CertificateStatus(certificatestatusnode.node()));
    }

    return boost::make_ready_future(SetCertificatesStatus(certificatestatus));
  }
  else if (action == SETCLIENTCERTIFICATEMODE)
  {
    const auto enablednode = operationnode.select_node("*[local-name()='Enabled']");
    if (!enablednode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Enabled argument")).ToString()));
    }

    return boost::make_ready_future(SetClientCertificateMode(enablednode.node().text().as_bool()));
  }
  else if (action == SETDISCOVERYMODE)
  {
    const auto discoverymodenode = operationnode.select_node("*[local-name()='DiscoveryMode']");
    if (!discoverymodenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing DiscoveryMode argument")).ToString()));
    }

    boost::optional<onvif::DISCOVERYMODE> discoverymode = onvif::GetDiscoveryMode(discoverymodenode.node().text().get());
    if (!discoverymode.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid DiscoveryMode argument")).ToString()));
    }

    return boost::make_ready_future(SetDiscoveryMode(*discoverymode));
  }
  else if (action == SETDNS)
  {
    const auto fromdhcpnode = operationnode.select_node("*[local-name()='FromDHCP']");
    if (!fromdhcpnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing FromDHCP argument")).ToString()));
    }

    const auto searchdomainnodes = operationnode.select_nodes("*[local-name()='SearchDomain']");
    std::vector<std::string> searchdomains;
    for (const auto& searchdomainnode : searchdomainnodes)
    {
      if (!searchdomainnode.node())
      {

        continue;
      }

      searchdomains.push_back(std::string(searchdomainnode.node().text().get()));
    }

    const auto dnsmanualnodes = operationnode.select_nodes("*[local-name()='DNSManual']");
    std::vector<onvif::IPAddress> dnsmanual;
    for (const auto& dnsmanualnode : dnsmanualnodes)
    {
      if (!dnsmanualnode.node())
      {

        continue;
      }

      dnsmanual.push_back(onvif::IPAddress(dnsmanualnode.node()));
    }

    return boost::make_ready_future(SetDNS(fromdhcpnode.node().text().as_bool(), searchdomains, dnsmanual));
  }
  else if (action == SETDOT1XCONFIGURATION)
  {
    const auto dot1xconfigurationnode = operationnode.select_node("*[local-name()='Dot1XConfiguration']");
    if (!dot1xconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Dot1XConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(SetDot1XConfiguration(onvif::Dot1XConfiguration(dot1xconfigurationnode.node())));
  }
  else if (action == SETDPADDRESSES)
  {
    const auto dpaddressnodes = operationnode.select_nodes("*[local-name()='DPAddress']");
    std::vector<onvif::NetworkHost> dpaddress;
    for (const auto& dpaddressnode : dpaddressnodes)
    {
      if (!dpaddressnode.node())
      {

        continue;
      }

      dpaddress.push_back(onvif::NetworkHost(dpaddressnode.node()));
    }

    return boost::make_ready_future(SetDPAddresses(dpaddress));
  }
  else if (action == SETDYNAMICDNS)
  {
    // Type
    const auto typenode = operationnode.select_node("*[local-name()='Type']");
    if (!typenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Type argument")).ToString()));
    }

    const boost::optional<onvif::DYNAMICDNSTYPE> type = onvif::GetDynamicDNSType(typenode.node().text().get());
    if (!type.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid Type argument")).ToString()));
    }

    // Name
    const auto namenode = operationnode.select_node("*[local-name()='Name']");
    boost::optional<std::string> name;
    if (namenode)
    {
      name = namenode.node().text().get();

    }

    // TTL
    const auto ttlnode = operationnode.select_node("*[local-name()='TTL']");
    boost::optional<onvif::Duration> ttl;
    if (ttlnode)
    {
      ttl = onvif::GetDuration(ttlnode.node().text().get());
      if (!ttl.is_initialized())
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid TTL argument")).ToString()));
      }
    }

    return boost::make_ready_future(SetDynamicDNS(*type, name, ttl));
  }
  else if (action == SETHOSTNAME)
  {
    const auto namenode = operationnode.select_node("*[local-name()='Name']");
    if (!namenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Name argument")).ToString()));
    }

    return boost::make_ready_future(SetHostname(namenode.node().text().get()));
  }
  else if (action == SETHOSTNAMEFROMDHCP)
  {
    const auto fromdhcpnode = operationnode.select_node("*[local-name()='FromDHCP']");
    if (!fromdhcpnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing FromDHCP argument")).ToString()));
    }

    return boost::make_ready_future(SetHostnameFromDHCP(fromdhcpnode.node().text().as_bool()));
  }
  else if (action == SETIPADDRESSFILTER)
  {
    const auto ipaddressfilternode = operationnode.select_node("*[local-name()='IPAddressFilter']");
    if (!ipaddressfilternode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing IPAddressFilter argument")).ToString()));
    }

    return boost::make_ready_future(SetIPAddressFilter(onvif::IPAddressFilter(ipaddressfilternode.node())));
  }
  else if (action == SETNETWORKDEFAULTGATEWAY)
  {
    const auto ipv4addressnodes = operationnode.select_nodes("*[local-name()='IPv4Address']");
    std::vector<std::string> ipv4address;
    for (const auto& ipv4addressnode : ipv4addressnodes)
    {
      if (!ipv4addressnode.node())
      {

        continue;
      }

      ipv4address.push_back(std::string(ipv4addressnode.node().text().get()));
    }

    const auto ipv6addressnodes = operationnode.select_nodes("*[local-name()='IPv6Address']");
    std::vector<std::string> ipv6address;
    for (const auto& ipv6addressnode : ipv6addressnodes)
    {
      if (!ipv6addressnode.node())
      {

        continue;
      }

      ipv6address.push_back(std::string(ipv6addressnode.node().text().get()));
    }

    return boost::make_ready_future(SetNetworkDefaultGateway(ipv4address, ipv6address));
  }
  else if (action == SETNETWORKINTERFACES)
  {
    const auto interfacetokennode = operationnode.select_node("*[local-name()='InterfaceToken']");
    if (!interfacetokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing InterfaceToken argument")).ToString()));
    }

    const auto networkinterfacenode = operationnode.select_node("*[local-name()='NetworkInterface']");
    if (!networkinterfacenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing NetworkInterface argument")).ToString()));
    }

    return boost::make_ready_future(SetNetworkInterfaces(interfacetokennode.node().text().get(), onvif::NetworkInterfaceSetConfiguration(networkinterfacenode.node())));
  }
  else if (action == SETNETWORKPROTOCOLS)
  {
    const auto networkprotocolsnodes = operationnode.select_nodes("*[local-name()='NetworkProtocols']");
    std::vector<onvif::NetworkProtocol> networkprotocols;
    for (const auto& networkprotocolsnode : networkprotocolsnodes)
    {
      if (!networkprotocolsnode.node())
      {

        continue;
      }

      networkprotocols.push_back(onvif::NetworkProtocol(networkprotocolsnode.node()));
    }

    return boost::make_ready_future(SetNetworkProtocols(networkprotocols));
  }
  else if (action == SETNTP)
  {
    const auto fromdhcpnode = operationnode.select_node("*[local-name()='FromDHCP']");
    if (!fromdhcpnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing FromDHCP argument")).ToString()));
    }

    const auto ntpmanualnodes = operationnode.select_nodes("*[local-name()='NTPManual']");
    std::vector<onvif::NetworkHost> ntpmanual;
    for (const auto& ntpmanualnode : ntpmanualnodes)
    {
      if (!ntpmanualnode.node())
      {

        continue;
      }

      ntpmanual.push_back(onvif::NetworkHost(ntpmanualnode.node()));
    }

    return boost::make_ready_future(SetNTP(fromdhcpnode.node().text().as_bool(), ntpmanual));
  }
  else if (action == SETRELAYOUTPUTSETTINGS)
  {
    const auto relayoutputtokennode = operationnode.select_node("*[local-name()='RelayOutputToken']");
    if (!relayoutputtokennode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RelayOutputToken argument")).ToString()));
    }

    const auto propertiesnode = operationnode.select_node("*[local-name()='Properties']");
    if (!propertiesnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Properties argument")).ToString()));
    }

    return boost::make_ready_future(SetRelayOutputSettings(std::string(relayoutputtokennode.node().text().get()), onvif::RelayOutputSettings(propertiesnode.node())));
  }
  else if (action == SETRELAYOUTPUTSTATE)
  {
    const auto relayoutputtokennode = operationnode.select_node("*[local-name()='RelayOutputToken']");
    if (!relayoutputtokennode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RelayOutputToken argument")).ToString()));
    }

    const auto logicalstatenode = operationnode.select_node("*[local-name()='LogicalState']");
    if (!logicalstatenode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing LogicalState argument")).ToString()));
    }

    boost::optional<onvif::RELAYLOGICALSTATE> logicalstate = onvif::GetRelayLogicalState(logicalstatenode.node().text().get());
    if (!logicalstate.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid RemoteDiscoveryMode argument")).ToString()));
    }

    return boost::make_ready_future(SetRelayOutputState(std::string(relayoutputtokennode.node().text().get()), *logicalstate));
  }
  else if (action == SETREMOTEDISCOVERYMODE)
  {
    const auto remotediscoverymodenode = operationnode.select_node("*[local-name()='RemoteDiscoveryMode']");
    if (!remotediscoverymodenode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RemoteDiscoveryMode argument")).ToString()));
    }

    boost::optional<onvif::DISCOVERYMODE> remotediscoverymode = onvif::GetDiscoveryMode(remotediscoverymodenode.node().text().get());
    if (!remotediscoverymode.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid RemoteDiscoveryMode argument")).ToString()));
    }

    return boost::make_ready_future(SetRemoteDiscoveryMode(*remotediscoverymode));
  }
  else if (action == SETREMOTEUSER)
  {
    const auto remoteusernode = operationnode.select_node("*[local-name()='RemoteUser']");
    boost::optional<onvif::RemoteUser> remoteuser;
    if (remoteusernode)
    {
      remoteuser = onvif::RemoteUser(remoteusernode.node());

    }

    return boost::make_ready_future(SetRemoteUser(remoteuser));
  }
  else if (action == SETSCOPES)
  {
    const auto scopesnode = operationnode.select_nodes("*[local-name()='Scopes']");
    std::vector<std::string> scopes;
    for (const auto& scope : scopesnode)
    {
      if (!scope.node())
      {

        continue;
      }

      scopes.push_back(scope.node().text().get());
    }

    return boost::make_ready_future(SetScopes(scopes));
  }
  else if (action == SETSYSTEMDATEANDTIME)
  {
    const auto datetimetypenode = operationnode.select_node("*[local-name()='DateTimeType']");
    if (!datetimetypenode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing DateTimeType argument")).ToString()));
    }

    boost::optional<onvif::DATETIMETYPE> datetimetype = onvif::GetDateTimeType(datetimetypenode.node().text().get());
    if (!datetimetype.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid DateTimeType argument")).ToString()));
    }

    const auto daylightsavingsnode = operationnode.select_node("*[local-name()='DaylightSavings']");
    if (!daylightsavingsnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing DaylightSavings argument")).ToString()));
    }

    const auto timezonenode = operationnode.select_node("*[local-name()='TimeZone']");
    boost::optional<onvif::TimeZone> timezone;
    if (timezonenode)
    {
      timezone = onvif::TimeZone(timezonenode.node());

    }

    const auto utcdatetimenode = operationnode.select_node("*[local-name()='UTCDateTime']");
    if (!utcdatetimenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing UTCDateTime argument")).ToString()));
    }

    return boost::make_ready_future(SetSystemDateAndTime(*datetimetype, daylightsavingsnode.node().text().as_bool(), timezone, onvif::DateTime(utcdatetimenode.node())));
  }
  else if (action == SETSYSTEMFACTORYDEFAULT)
  {
    const auto factorydefaultnode = operationnode.select_node("*[local-name()='FactoryDefault']");
    if (!factorydefaultnode.node())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing FactoryDefault argument")).ToString()));
    }

    boost::optional<onvif::FACTORYDEFAULTTYPE> factorydefault = onvif::GetFactoryDefaultType(factorydefaultnode.node().text().get());
    if (!factorydefault.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid FactoryDefault argument")).ToString()));
    }

    return boost::make_ready_future(SetSystemFactoryDefault(*factorydefault));
  }
  else if (action == SETUSER)
  {
    const auto usersnode = operationnode.select_nodes("*[local-name()='User']");
    std::vector<onvif::User> users;
    for (const auto& user : usersnode)
    {
      if (!user.node())
      {

        continue;
      }

      users.push_back(onvif::User(user.node()));
    }

    return boost::make_ready_future(SetUser(users));
  }
  else if (action == SETZEROCONFIGURATION)
  {
    const auto interfacetoken = operationnode.select_node("*[local-name()='InterfaceToken']");
    if (!interfacetoken)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing InterfaceToken argument")).ToString()));
    }

    const auto enabled = operationnode.select_node("*[local-name()='Enabled']");
    if (!enabled)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Enabled argument")).ToString()));
    }
    
    return boost::make_ready_future(SetZeroConfiguration(std::string(interfacetoken.node().text().get()), interfacetoken.node().text().as_bool()));
  }
  else if (action == SYSTEMREBOOT)
  {

    return boost::make_ready_future(SystemReboot());
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

#ifdef _WIN32
std::unique_ptr<char[]> DeviceServiceInterface::GetAdaptersAddresses() const
{
  ULONG size = 25000; // We allocate a large buffer because we don't want to query all the devices many times because Microsoft says this is an expensive operation
  std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
  for (int i = 0; i < 10; ++i) // We loop because the size required can change between requests, so we attempt a set number of times
  {
    auto ret = ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_GATEWAYS | GAA_FLAG_INCLUDE_ALL_INTERFACES, nullptr, reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get()), &size);
    if (ret == ERROR_SUCCESS)
    {

      return buffer;
    }
    else if (ret == ERROR_BUFFER_OVERFLOW) // Reallocate
    {
      size += size;
      buffer = std::make_unique<char[]>(size);
    }
    else
    {
      
      return nullptr;
    }
  }
  return nullptr;
}

std::unique_ptr<char[]> DeviceServiceInterface::GetAdaptersInfo() const
{
  std::unique_ptr<char[]> buffer = std::make_unique<char[]>(sizeof(IP_ADAPTER_INFO));
  ULONG size = sizeof(IP_ADAPTER_INFO);
  switch (::GetAdaptersInfo(reinterpret_cast<IP_ADAPTER_INFO*>(buffer.get()), &size))
  {
    case ERROR_SUCCESS:
    {

      return buffer;
    }
    case ERROR_BUFFER_OVERFLOW: // Realloc and retry
    {
      size += size;
      buffer = std::make_unique<char[]>(size);
      if (::GetAdaptersInfo(reinterpret_cast<IP_ADAPTER_INFO*>(buffer.get()), &size) == ERROR_SUCCESS)
      {

        return buffer;
      }
      break;
    }
    default:
    {

      return nullptr;
    }
  }

  return nullptr;
}

const IP_ADAPTER_ADDRESSES* DeviceServiceInterface::GetAdapterAddresses(const std::string& adaptername, const IP_ADAPTER_ADDRESSES* adaptersaddresses) const
{
  const IP_ADAPTER_ADDRESSES* adapteraddresses = adaptersaddresses;
  while (adapteraddresses)
  {
    if (adaptername == adapteraddresses->AdapterName)
    {

      return adapteraddresses;
    }

    adapteraddresses = adapteraddresses->Next;
  }

  return nullptr;
}
#endif

}

}

}
