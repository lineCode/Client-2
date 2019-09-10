// dummydeviceservice.hpp
//

///// Includes /////

#include "dummydeviceservice.hpp"

#include <algorithm>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyDeviceService::DummyDeviceService()
{
  
}

DummyDeviceService::~DummyDeviceService()
{

}

boost::shared_future<server::ServerResponse> DummyDeviceService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  std::map< std::string, std::vector<char> > mtomresponse;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddIPAddressFilter']"))
  {
    response = std::string("<tds:AddIPAddressFilterResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddScopes']"))
  {
    response = std::string("<tds:AddScopesResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateCertificate']"))
  {
    response = std::string("<tds:CreateCertificateResponse>") + createcertificate_.ToXml("tt:NvtCertificate") + std::string("</tds:CreateCertificateResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateStorageConfiguration']"))
  {
    response = std::string("<tds:CreateStorageConfigurationResponse><tt:Token>") + createstorageconfiguration_ + std::string("</tt:Token></tds:CreateStorageConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateUsers']"))
  {
    response = std::string("<tds:CreateUsersResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteCertificates']"))
  {
    response = std::string("<tds:DeleteCertificatesResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteDot1XConfiguration']"))
  {
    response = std::string("<tds:DeleteDot1XConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteStorageConfiguration']"))
  {
    response = std::string("<tds:DeleteStorageConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteUsers']"))
  {
    response = std::string("<tds:DeleteUsersResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAccessPolicy']"))
  {
    response = std::string("<tds:GetAccessPolicyResponse>") + getaccesspolicy_.ToXml("tt:PolicyFile") + std::string("</tds:GetAccessPolicyResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCACertificates']"))
  {
    std::string cacertificates;
    for (const auto& getcacertificate : getcacertificates_)
    {
      cacertificates += getcacertificate.ToXml("tds:CACertificate");
  
    }
  
    response = std::string("<tds:GetCACertificatesResponse>") + cacertificates + std::string("</tds:GetCACertificatesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCapabilities']"))
  {
    response = std::string("<tds:GetCapabilitiesResponse>") + getcapabilities_.ToXml("tds:Capabilities") + std::string("</tds:GetCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificateInformation']"))
  {
    response = std::string("<tds:GetCertificateInformationResponse>") + getcertificateinformation_.ToXml("tt:CertificateInformation") + std::string("</tds:GetCertificateInformationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificates']"))
  {
    std::string certificates;
    for (const auto& getcertificate : getcertificates_)
    {
      certificates += getcertificate.ToXml("tds:NvtCertificate");
  
    }
  
    response = std::string("<tds:GetCertificatesResponse>") + certificates + std::string("</tds:GetCertificatesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCertificatesStatus']"))
  {
    std::string certificatesstatus;
    for (const auto& getcertificatestatus : getcertificatesstatus_)
    {
      certificatesstatus += getcertificatestatus.ToXml("tds:CertificateStatus");
  
    }
    
    response = std::string("<tds:GetCertificatesStatusResponse>") + certificatesstatus + std::string("</tds:GetCertificatesStatusResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetClientCertificateMode']"))
  {
    response = std::string("<tds:GetClientCertificateModeResponse>") + onvif::ToXml("tds:Enabled", boost::optional<bool>(getclientcertificatemode_)) + std::string("</tds:GetClientCertificateModeResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDeviceInformation']"))
  {
    response = std::string("<tds:GetDeviceInformationResponse>") + ToXml("tds:Manufacturer", manufacturer_) + ToXml("tds:Model", model_) + ToXml("tds:FirmwareVersion", firmwareversion_) + ToXml("tds:SerialNumber", serialnumber_) + ToXml("tds:HardwareId", hardwareid_) + std::string("</tds:GetDeviceInformationResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDiscoveryMode']"))
  {
    response = std::string("<tds:GetDiscoveryModeResponse>") + onvif::ToXmlEnum("tds:DiscoveryMode", getdiscoverymode_) + std::string("</tds:GetDiscoveryModeResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDNS']"))
  {
    response = std::string("<tds:GetDNSResponse>") + getdns_.ToXml("tds:DNSInformation") + std::string("</tds:GetDNSResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot11Capabilities']"))
  {
    response = std::string("<tds:GetDot11CapabilitiesResponse>") + getdot11capabilities_.ToXml("tds:Capabilities") + std::string("</tds:GetDot11CapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot11Status']"))
  {
    response = std::string("<tds:GetDot11StatusResponse>") + getdot11status_.ToXml("tds:Status") + std::string("</tds:GetDot11StatusResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot1XConfiguration']"))
  {
    response = std::string("<tds:GetDot1XConfigurationResponse>") + getdot1xconfiguration_.ToXml("tds:Dot1XConfiguration") + std::string("</tds:GetDot1XConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDot1XConfigurations']"))
  {
    std::string dot1xconfigurations;
    for (const auto& dot1xconfiguration : getdot1xconfigurations_)
    {
      dot1xconfigurations += dot1xconfiguration.ToXml("tds:Dot1XConfiguration");
  
    }
    
    response = std::string("<tds:GetDot1XConfigurationsResponse>") + dot1xconfigurations + std::string("</tds:GetDot1XConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDPAddresses']"))
  {
    std::string dpaddresses;
    for (const auto& dpaddress : getdpaddresses_)
    {
      dpaddresses += dpaddress.ToXml("tds:DPAddress");
  
    }
    
    response = std::string("<tds:GetDPAddressesResponse>") + dpaddresses + std::string("</tds:GetDPAddressesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDynamicDNS']"))
  {
    response = std::string("<tds:GetDynamicDNSResponse>") + getdynamicdns_.ToXml("tds:DynamicDNSInformation") + std::string("</tds:GetDynamicDNSResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetEndpointReference']"))
  {
    response = std::string("<tds:GetEndpointReferenceResponse>") + ToXml("GUID", getendpointreference_) + std::string("</tds:GetEndpointReferenceResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetHostname']"))
  {
    response = std::string("<tds:GetHostnameResponse>") + gethostname_.ToXml("tds:HostnameInformation") + std::string("</tds:GetHostnameResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetIPAddressFilter']"))
  {
    response = std::string("<tds:GetIPAddressFilterResponse>") + getipaddressfilter_.ToXml("IPAddressFilter") + std::string("</tds:GetIPAddressFilterResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkDefaultGateway']"))
  {
    response = std::string("<tds:GetNetworkDefaultGatewayResponse>") + getnetworkdefaultgateway_.ToXml("NetworkGateway") + std::string("</tds:GetNetworkDefaultGatewayResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkInterfaces']"))
  {
    std::string networkinterfaces;
    for (const auto& networkinterface : getnetworkinterfaces_)
    {
      networkinterfaces += networkinterface.ToXml("tds:NetworkInterfaces");
  
    }
  
    response = std::string("<tds:GetNetworkInterfacesResponse>") + networkinterfaces + std::string("</tds:GetNetworkInterfacesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNetworkProtocols']"))
  {
    std::string networkprotocols;
    for (const auto& networkprotocol : getnetworkprotocols_)
    {
      networkprotocols += networkprotocol.ToXml("tds:NetworkProtocols");
  
    }
  
    response = std::string("<tds:GetNetworkProtocolsResponse>") + networkprotocols + std::string("</tds:GetNetworkProtocolsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNTP']"))
  {
    response = std::string("<tds:GetNTPResponse>") + getntp_.ToXml("tds:NTPInformation") + std::string("</tds:GetNTPResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetPkcs10Request']"))
  {
    response = std::string("<tds:GetPkcs10RequestResponse>") + getpkcs10request_.ToXml("tt:Pkcs10Request") + std::string("</tds:GetPkcs10RequestResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputs']"))
  {
    std::string relayoutputs;
    for (const auto& relayoutput : getrelayoutputs_)
    {
      relayoutputs += relayoutput.ToXml("tds:RelayOutputs");

    }

    response = std::string("<tds:GetRelayOutputsResponse>") + relayoutputs + std::string("</tds:GetRelayOutputsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRemoteDiscoveryMode']"))
  {
    response = std::string("<tds:GetRemoteDiscoveryModeResponse>") + onvif::ToXmlEnum("tds:RemoteDiscoveryMode", boost::optional<DISCOVERYMODE>(getremotediscoverymode_)) + std::string("</tds:GetRemoteDiscoveryModeResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRemoteUser']"))
  {
    response = std::string("<tds:GetRemoteUserResponse>") + getremoteuser_.ToXml("tds:RemoteUser") + std::string("</tds:GetRemoteUserResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetScopes']"))
  {
    std::string scopes;
    for (const auto& getscope : getscopes_)
    {
      scopes += getscope.ToXml("tds:Scopes");
  
    }
  
    response = std::string("<tds:GetScopesResponse>") + scopes + std::string("</tds:GetScopesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<tds:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tds:Capabilities") + std::string("</tds:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServices']"))
  {
    std::string services;
    for (const auto& getservice : getservices_)
    {
      services += getservice.ToXml("tds:Service");
  
    }

    response = std::string("<tds:GetServicesResponse>") + services + std::string("</tds:GetServicesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStorageConfiguration']"))
  {
    response = std::string("<tds:GetStorageConfigurationResponse>") + getstorageconfiguration_.ToXml("tds:StorageConfiguration") + std::string("</tds:GetStorageConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStorageConfigurations']"))
  {
    std::string getstorageconfigurations;
    for (const auto& storageconfiguration : getstorageconfigurations_)
    {
      getstorageconfigurations += storageconfiguration.ToXml("tds:StorageConfigurations");
  
    }

    response = std::string("<tds:GetStorageConfigurationsResponse>") + getstorageconfigurations + std::string("</tds:GetStorageConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemBackup']"))
  {
    std::string backupfiles;
    for (onvif::BackupFile getsystembackup : getsystembackup_)
    {
      if (!getsystembackup.data_.is_initialized() || !getsystembackup.data_->include_->href_.is_initialized() || !getsystembackup.data_->include_.is_initialized())
      {

        return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
      }

      mtomresponse[*getsystembackup.data_->include_->href_] = getsystembackup.data_->include_->data_;
      getsystembackup.data_->include_->data_.clear();

      backupfiles += getsystembackup.ToXml("tds:BackupFiles");
    }
    
    response = std::string("<tds:GetSystemBackupResponse>") + backupfiles + std::string("</tds:GetSystemBackupResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemDateAndTime']"))
  {
    response = std::string("<tds:GetSystemDateAndTimeResponse>") + getsystemdatetime_.ToXml("tds:SystemDateAndTime") + std::string("</tds:GetSystemDateAndTimeResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemLog']"))
  {
    response = std::string("<tds:GetSystemLogResponse>") + getsystemlog_.ToXml("tds:SystemLog") + std::string("</tds:GetSystemLogResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemSupportInformation']"))
  {
    response = std::string("<tds:GetSystemSupportInformationResponse>") + getsystemsupportinformation_.ToXml("tds:SupportInformation") + std::string("</tds:GetSystemSupportInformationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSystemUris']"))
  {
    response = std::string("<tds:GetSystemUrisResponse>") + systemloguris_.ToXml("tds:SystemLogUris") + ToXml("tds:SupportInfoUri", supportinfouri_) + ToXml("tds:SystemBackupUri", systembackupuri_) + std::string("</tds:GetSystemUrisResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetUsers']"))
  {
    std::string users;
    for (const auto& getuser : getusers_)
    {
      users += getuser.ToXml("tds:User");
  
    }
  
    response = std::string("<tds:GetUsersResponse>") + users + std::string("</tds:GetUsersResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetWsdlUrl']"))
  {
    response = std::string("<tds:GetWsdlUrlResponse><tds:WsdlUrl>") + getwsdlurl_ + std::string("</tds:WsdlUrl></tds:GetWsdlUrlResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetZeroConfiguration']"))
  {
    response = std::string("<tds:GetZeroConfigurationResponse>") + getzeroconfiguration_.ToXml("tds:ZeroConfiguration") + std::string("</tds:GetZeroConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCACertificates']"))
  {
    response = std::string("<tds:LoadCACertificatesResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCertificates']"))
  {
    response = std::string("<tds:LoadCertificatesResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='LoadCertificateWithPrivateKey']"))
  {
    response = std::string("<tds:LoadCertificateWithPrivateKeyResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveIPAddressFilter']"))
  {
    response = std::string("<tds:RemoveIPAddressFilterResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveScopes']"))
  {
    response = std::string("<tds:RemoveScopesResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RestoreSystem']"))
  {
    response = std::string("<tds:RestoreSystemResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ScanAvailableDot11Networks']"))
  {
    std::string networks;
    for (const auto& network : scanavailabledot11networks_)
    {
      networks += network.ToXml("tds:Networks");
  
    }
  
    response = std::string("<tds:ScanAvailableDot11NetworksResponse>") + networks + std::string("</tds:ScanAvailableDot11NetworksResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SendAuxiliaryCommand']"))
  {
    response = std::string("<tds:SendAuxiliaryCommandResponse>") + ToXml("tds:AuxiliaryCommandResponse", auxiliarycommandresponse_) + std::string("</tds:SendAuxiliaryCommandResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAccessPolicy']"))
  {
    response = std::string("<tds:SetAccessPolicyResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetCertificatesStatus']"))
  {
    response = std::string("<tds:SetCertificatesStatusResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetClientCertificateMode']"))
  {
    response = std::string("<tds:SetClientCertificateModeResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDiscoveryMode']"))
  {
    response = std::string("<tds:SetDiscoveryModeResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDNS']"))
  {
    response = std::string("<tds:SetDNSResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDot1XConfiguration']"))
  {
    response = std::string("<tds:SetDot1XConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDPAddresses']"))
  {
    response = std::string("<tds:SetDPAddressesResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDynamicDNS']"))
  {
    response = std::string("<tds:SetDynamicDNSResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetHostname']"))
  {
    response = std::string("<tds:SetHostnameResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetHostnameFromDHCP']"))
  {
    response = std::string("<tds:SetHostnameFromDHCPResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetIPAddressFilter']"))
  {
    response = std::string("<tds:SetIPAddressFilterResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkDefaultGateway']"))
  {
    response = std::string("<tds:SetNetworkDefaultGatewayResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkInterfaces']"))
  {
    response = std::string("<tds:SetNetworkInterfacesResponse>") + onvif::ToXml("RebootNeeded", boost::optional<bool>(setnetworkinterfaces_)) + std::string("</tds:SetNetworkInterfacesResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNetworkProtocols']"))
  {
    response = std::string("<tds:SetNetworkProtocolsResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetNTP']"))
  {
    response = std::string("<tds:SetNTPResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputSettings']"))
  {
    response = std::string("<tds:SetRelayOutputSettingsResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRemoteDiscoveryMode']"))
  {
    response = std::string("<tds:SetRemoteDiscoveryModeResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSystemDateAndTime']"))
  {
    response = std::string("<tds:SetSystemDateAndTimeResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSystemFactoryDefault']"))
  {
    response = std::string("<tds:SetSystemFactoryDefaultResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetUser']"))
  {
    response = std::string("<tds:SetUserResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetZeroConfiguration']"))
  {
    response = std::string("<tds:SetZeroConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SystemReboot']"))
  {
    response = std::string("<tds:SystemRebootResponse>") + ToXml("tds:Message", getsystemreboot_) + std::string("</tds:SystemRebootResponse>");
    
  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }
  
  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>"), mtomresponse));
}

std::string DummyDeviceService::ToXml(const std::string& name, const std::string& text)
{
  if (!text.empty())
    return ("<"+name+">" + text + "</"+name+">");
  else
    return std::string();
}

}

}
