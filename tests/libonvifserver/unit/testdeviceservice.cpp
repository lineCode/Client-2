// testdeviceservice.cpp
//

///// Includes /////

#include "testdeviceservice.hpp"

#include <boost/asio.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 9000;
const std::string DEVICEURI = "/device";
const std::string WSDLDEVICEURI = "/wsdl/devicemgmt.wsdl";

///// Declarations /////

ClientThread<onvif::device::DeviceClient> TestDeviceService::deviceclient_;
const std::string TestDeviceService::username_("username");
const onvif::USERLEVEL TestDeviceService::userlevel_(onvif::USERLEVEL_ADMINISTRATOR);
const std::string TestDeviceService::password_("password");

///// Functions /////

static size_t WriteFunction(void* data, size_t size, size_t nmemb, void* body)
{
  static_cast<std::string*>(body)->append(static_cast<char*>(data), static_cast<char*>(data) + (size*nmemb));
  return (size*nmemb);
}

///// Methods /////

TestDeviceService::TestDeviceService() :
  httpserver_("realm@host.com"),
  dummydeviceservice_(boost::make_shared<DummyDeviceService>()),
  dummywsdlservice_(boost::make_shared<DummyService>()),
  run_(false)
{

}

TestDeviceService::~TestDeviceService()
{

}

void TestDeviceService::SetUpTestCase()
{
  ASSERT_FALSE(deviceclient_.Init(sock::ProxyParams(), std::string("http://127.0.0.1:") + std::to_string(PORT) + DEVICEURI, username_, password_));

}

void TestDeviceService::TearDownTestCase()
{
  deviceclient_.Destroy();

}

void TestDeviceService::SetUp()
{
  ASSERT_FALSE(curl_global_init(CURL_GLOBAL_ALL));
  ASSERT_FALSE(httpserver_.Init("0.0.0.0", PORT));
  httpserver_.AddServer(DEVICEURI, dummydeviceservice_.get());
  dummywsdlservice_->SetRestricted(false);
  httpserver_.AddServer(WSDLDEVICEURI, dummywsdlservice_.get());
  httpserverthread_ = std::thread(std::bind(&TestDeviceService::Run, this));
}

void TestDeviceService::TearDown()
{
  run_ = false;
  httpserverthread_.join();

  ASSERT_FALSE(httpserver_.Destroy());
  curl_global_cleanup();
}

void TestDeviceService::Run()
{
  ASSERT_FALSE(dummydeviceservice_->Init());
  dummydeviceservice_->SetUsername(username_);
  dummydeviceservice_->SetUserLevel(userlevel_);
  dummydeviceservice_->SetPassword(password_);

  run_ = true;
  while (run_)
  {
    ASSERT_FALSE(httpserver_.Update());
    utility::Sleep(std::chrono::milliseconds(10));
  }

  dummydeviceservice_->Destroy();
}

///// Tests /////

TEST_F(TestDeviceService, AddIPAddressFilter)
{
  onvif::device::AddIPAddressFilterResponse addipaddressfilterresponse = deviceclient_->AddIPAddressFilterFuture(IPAddressFilter(IPADDRESSFILTERTYPE_ALLOW, { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) })).get();
  ASSERT_GE(addipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(addipaddressfilterresponse.Error());
}

TEST_F(TestDeviceService, AddScopes)
{
  onvif::device::AddScopesResponse addscopesresponse = deviceclient_->AddScopesFuture({ std::string("test1"), std::string("test2"), std::string("test3") }).get();
  ASSERT_GE(addscopesresponse.latency_, 0);
  ASSERT_FALSE(addscopesresponse.Error());
}

TEST_F(TestDeviceService, CreateCertificate)
{
  onvif::device::CreateCertificateResponse createcertificateresponse = deviceclient_->CreateCertificateFuture(std::string("CertificateID"), std::string("Subject"), ws::DateTime(2000, 10, 24, 1, 2, 3, 0), ws::DateTime(2001, 10, 24, 1, 2, 3, 0)).get();
  ASSERT_GE(createcertificateresponse.latency_, 0);
  ASSERT_FALSE(createcertificateresponse.Error());
}

TEST_F(TestDeviceService, CreateStorageConfiguration)
{
  onvif::device::CreateStorageConfigurationResponse createstorageconfigurationresponse = deviceclient_->CreateStorageConfigurationFuture(onvif::device::StorageConfigurationData(std::string("localpath"), std::string("storageuri"), UserCredential(std::string("username"), std::string("password")), std::string("type"))).get();
  ASSERT_GE(createstorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(createstorageconfigurationresponse.Error());
}

TEST_F(TestDeviceService, CreateUsers)
{
  onvif::device::CreateUsersResponse createusersresponse = deviceclient_->CreateUsersFuture({ onvif::User(std::string("username1"), std::string("password1"), USERLEVEL_ADMINISTRATOR), onvif::User(std::string("username2"), std::string("password2"), USERLEVEL_ADMINISTRATOR) }).get();
  ASSERT_GE(createusersresponse.latency_, 0);
  ASSERT_FALSE(createusersresponse.Error());
}

TEST_F(TestDeviceService, DeleteCertificates)
{
  onvif::device::DeleteCertificatesResponse deletecertificatesresponse = deviceclient_->DeleteCertificatesFuture({ std::string("certificateid1"), std::string("certificateid2"), std::string("certificateid3") }).get();
  ASSERT_GE(deletecertificatesresponse.latency_, 0);
  ASSERT_FALSE(deletecertificatesresponse.Error());
}

TEST_F(TestDeviceService, DeleteDot1XConfiguration)
{
  onvif::device::DeleteDot1XConfigurationResponse deletedot1xconfigurationresponse = deviceclient_->DeleteDot1XConfigurationFuture({ std::string("certificateid1"), std::string("certificateid2"), std::string("certificateid3") }).get();
  ASSERT_GE(deletedot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(deletedot1xconfigurationresponse.Error());
}

TEST_F(TestDeviceService, DeleteStorageConfiguration)
{
  onvif::device::DeleteStorageConfigurationResponse deletestorageconfigurationresponse = deviceclient_->DeleteStorageConfigurationFuture({ std::string("token1"), std::string("token2"), std::string("token3") }).get();
  ASSERT_GE(deletestorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(deletestorageconfigurationresponse.Error());
}

TEST_F(TestDeviceService, DeleteUsers)
{
  onvif::device::DeleteUsersResponse deleteusersresponse = deviceclient_->DeleteUsersFuture({ std::string("username1"), std::string("username2") }).get();
  ASSERT_GE(deleteusersresponse.latency_, 0);
  ASSERT_FALSE(deleteusersresponse.Error());
}

TEST_F(TestDeviceService, GetAccessPolicy)
{
  onvif::device::GetAccessPolicyResponse getaccesspolicyresponse = deviceclient_->GetAccessPolicyFuture().get();
  ASSERT_GE(getaccesspolicyresponse.latency_, 0);
  ASSERT_FALSE(getaccesspolicyresponse.Error());
}

TEST_F(TestDeviceService, GetCACertificates)
{
  onvif::device::GetCACertificatesResponse getcacertificatesresponse = deviceclient_->GetCACertificatesFuture().get();
  ASSERT_GE(getcacertificatesresponse.latency_, 0);
  ASSERT_FALSE(getcacertificatesresponse.Error());
}

TEST_F(TestDeviceService, GetCapabilities)
{
  onvif::device::GetCapabilitiesResponse getcapabilitiesresponse = deviceclient_->GetCapabilitiesFuture(CAPABILITYCATEGORY_ALL).get();
  ASSERT_GE(getcapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getcapabilitiesresponse.Error());
}

TEST_F(TestDeviceService, GetCertificateInformation)
{
  onvif::device::GetCertificateInformationResponse getcertificateinformationresponse = deviceclient_->GetCertificateInformationFuture(std::string("CertificateID")).get();
  ASSERT_GE(getcertificateinformationresponse.latency_, 0);
  ASSERT_FALSE(getcertificateinformationresponse.Error());
}

TEST_F(TestDeviceService, GetCertificates)
{
  onvif::device::GetCertificatesResponse getcertificatesresponse = deviceclient_->GetCertificatesFuture().get();
  ASSERT_GE(getcertificatesresponse.latency_, 0);
  ASSERT_FALSE(getcertificatesresponse.Error());
}

TEST_F(TestDeviceService, GetCertificatesStatus)
{
  onvif::device::GetCertificatesStatusResponse getcertificatesstatusresponse = deviceclient_->GetCertificatesStatusFuture().get();
  ASSERT_GE(getcertificatesstatusresponse.latency_, 0);
  ASSERT_FALSE(getcertificatesstatusresponse.Error());
}

TEST_F(TestDeviceService, GetClientCertificateMode)
{
  onvif::device::GetClientCertificateModeResponse getclientcertificatemoderesponse = deviceclient_->GetClientCertificateModeFuture().get();
  ASSERT_GE(getclientcertificatemoderesponse.latency_, 0);
  ASSERT_FALSE(getclientcertificatemoderesponse.Error());
}

TEST_F(TestDeviceService, GetDeviceInformation)
{
  onvif::device::GetDeviceInformationResponse getdeviceinformationresponse = deviceclient_->GetDeviceInformationFuture().get();
  ASSERT_GE(getdeviceinformationresponse.latency_, 0);
  ASSERT_FALSE(getdeviceinformationresponse.Error());
}

TEST_F(TestDeviceService, GetDiscoveryMode)
{
  onvif::device::GetDiscoveryModeResponse getdiscoverymoderesponse = deviceclient_->GetDiscoveryModeFuture().get();
  ASSERT_GE(getdiscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(getdiscoverymoderesponse.Error());
}

TEST_F(TestDeviceService, GetDNS)
{
  onvif::device::GetDNSResponse getdnsresponse = deviceclient_->GetDNSFuture().get();
  ASSERT_GE(getdnsresponse.latency_, 0);
  ASSERT_FALSE(getdnsresponse.Error());
}

TEST_F(TestDeviceService, GetDot11Capabilities)
{
  onvif::device::GetDot11CapabilitiesResponse getdot11capabilitiesresponse = deviceclient_->GetDot11CapabilitiesFuture().get();
  ASSERT_GE(getdot11capabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getdot11capabilitiesresponse.Error());
}

TEST_F(TestDeviceService, GetDot11Status)
{
  onvif::device::GetDot11StatusResponse getdot11statusresponse = deviceclient_->GetDot11StatusFuture(std::string("InterfaceToken")).get();
  ASSERT_GE(getdot11statusresponse.latency_, 0);
  ASSERT_FALSE(getdot11statusresponse.Error());
}

TEST_F(TestDeviceService, GetDot1XConfiguration)
{
  onvif::device::GetDot1XConfigurationResponse getdot1xconfigurationresponse = deviceclient_->GetDot1XConfigurationFuture(std::string("Dot1XConfigurationToken")).get();
  ASSERT_GE(getdot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getdot1xconfigurationresponse.Error());
}

TEST_F(TestDeviceService, GetDot1XConfigurations)
{
  onvif::device::GetDot1XConfigurationsResponse getdot1xconfigurationsresponse = deviceclient_->GetDot1XConfigurationsFuture().get();
  ASSERT_GE(getdot1xconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getdot1xconfigurationsresponse.Error());
}

TEST_F(TestDeviceService, GetDPAddresses)
{
  onvif::device::GetDPAddressesResponse getdpaddressesresponse = deviceclient_->GetDPAddressesFuture().get();
  ASSERT_GE(getdpaddressesresponse.latency_, 0);
  ASSERT_FALSE(getdpaddressesresponse.Error());
}

TEST_F(TestDeviceService, GetDynamicDNS)
{
  onvif::device::GetDynamicDNSResponse getdynamicdnsresponse = deviceclient_->GetDynamicDNSFuture().get();
  ASSERT_GE(getdynamicdnsresponse.latency_, 0);
  ASSERT_FALSE(getdynamicdnsresponse.Error());
}

TEST_F(TestDeviceService, GetEndpointReference)
{
  onvif::device::GetEndpointReferenceResponse getendpointreferenceresponse = deviceclient_->GetEndpointReferenceFuture().get();
  ASSERT_GE(getendpointreferenceresponse.latency_, 0);
  ASSERT_FALSE(getendpointreferenceresponse.Error());
}

TEST_F(TestDeviceService, GetHostname)
{
  onvif::device::GetHostnameResponse gethostnameresponse = deviceclient_->GetHostnameFuture().get();
  ASSERT_GE(gethostnameresponse.latency_, 0);
  ASSERT_FALSE(gethostnameresponse.Error());
}

TEST_F(TestDeviceService, GetIPAddressFilter)
{
  onvif::device::GetIPAddressFilterResponse getipaddressfilterresponse = deviceclient_->GetIPAddressFilterFuture().get();
  ASSERT_GE(getipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(getipaddressfilterresponse.Error());
}

TEST_F(TestDeviceService, GetNetworkDefaultGateway)
{
  onvif::device::GetNetworkDefaultGatewayResponse getnetworkdefaultgateway = deviceclient_->GetNetworkDefaultGatewayFuture().get();
  ASSERT_GE(getnetworkdefaultgateway.latency_, 0);
  ASSERT_FALSE(getnetworkdefaultgateway.Error());
}

TEST_F(TestDeviceService, GetNetworkInterfaces)
{
  onvif::device::GetNetworkInterfacesResponse getnetworkinterfacesresponse = deviceclient_->GetNetworkInterfacesFuture().get();
  ASSERT_GE(getnetworkinterfacesresponse.latency_, 0);
  ASSERT_FALSE(getnetworkinterfacesresponse.Error());
}

TEST_F(TestDeviceService, GetNetworkProtocols)
{
  onvif::device::GetNetworkProtocolsResponse getnetworkprotocolsresponse = deviceclient_->GetNetworkProtocolsFuture().get();
  ASSERT_GE(getnetworkprotocolsresponse.latency_, 0);
  ASSERT_FALSE(getnetworkprotocolsresponse.Error());
}

TEST_F(TestDeviceService, GetNTP)
{
  onvif::device::GetNTPResponse getntpresponse = deviceclient_->GetNTPFuture().get();
  ASSERT_GE(getntpresponse.latency_, 0);
  ASSERT_FALSE(getntpresponse.Error());
}

TEST_F(TestDeviceService, GetRelayOutputs)
{
  onvif::device::GetRelayOutputsResponse getrelayoutputsresponse = deviceclient_->GetRelayOutputsFuture().get();
  ASSERT_GE(getrelayoutputsresponse.latency_, 0);
  ASSERT_FALSE(getrelayoutputsresponse.Error());
}

TEST_F(TestDeviceService, GetRemoteDiscoveryMode)
{
  onvif::device::GetRemoteDiscoveryModeResponse getremotediscoverymoderesponse = deviceclient_->GetRemoteDiscoveryModeFuture().get();
  ASSERT_GE(getremotediscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(getremotediscoverymoderesponse.Error());
}

TEST_F(TestDeviceService, GetRemoteUser)
{
  onvif::device::GetRemoteUserResponse getremoteuserresponse = deviceclient_->GetRemoteUserFuture().get();
  ASSERT_GE(getremoteuserresponse.latency_, 0);
  ASSERT_FALSE(getremoteuserresponse.Error());
}

TEST_F(TestDeviceService, GetScopes)
{
  onvif::device::GetScopesResponse getscopesresponse = deviceclient_->GetScopesFuture().get();
  ASSERT_GE(getscopesresponse.latency_, 0);
  ASSERT_FALSE(getscopesresponse.Error());
}

TEST_F(TestDeviceService, GetServiceCapabilities)
{
  onvif::device::GetServiceCapabilitiesResponse getservicecapabilitiesresponse = deviceclient_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getservicecapabilitiesresponse.Error());
}

TEST_F(TestDeviceService, GetServices)
{
  onvif::device::GetServicesResponse getservicesresponse = deviceclient_->GetServicesFuture(true).get();
  ASSERT_GE(getservicesresponse.latency_, 0);
  ASSERT_FALSE(getservicesresponse.Error());
}

TEST_F(TestDeviceService, GetStorageConfiguration)
{
  onvif::device::GetStorageConfigurationResponse getstorageconfigurationresponse = deviceclient_->GetStorageConfigurationFuture(std::string("token")).get();
  ASSERT_GE(getstorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getstorageconfigurationresponse.Error());
}

TEST_F(TestDeviceService, GetStorageConfigurations)
{
  onvif::device::GetStorageConfigurationsResponse getstorageconfigurationsresponse = deviceclient_->GetStorageConfigurationsFuture().get();
  ASSERT_GE(getstorageconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getstorageconfigurationsresponse.Error());
}

TEST_F(TestDeviceService, GetSystemBackup)
{
  onvif::device::GetSystemBackupResponse getsystembackupresponse = deviceclient_->GetSystemBackupFuture().get();
  ASSERT_GE(getsystembackupresponse.latency_, 0);
  ASSERT_FALSE(getsystembackupresponse.Error());
}

TEST_F(TestDeviceService, GetSystemDateAndTime)
{
  onvif::device::GetSystemDateAndTimeResponse getsystemdateandtimeresponse = deviceclient_->GetSystemDateAndTimeFuture().get();
  ASSERT_GE(getsystemdateandtimeresponse.latency_, 0);
  ASSERT_FALSE(getsystemdateandtimeresponse.Error());
}

TEST_F(TestDeviceService, GetSystemLog)
{
  onvif::device::GetSystemLogResponse getsystemlogresponse = deviceclient_->GetSystemLogFuture(SYSTEMLOGLEVEL_SYSTEM).get();
  ASSERT_GE(getsystemlogresponse.latency_, 0);
  ASSERT_FALSE(getsystemlogresponse.Error());
}

TEST_F(TestDeviceService, GetSystemSupportInformation)
{
  onvif::device::GetSystemSupportInformationResponse getsystemsupportinformationresponse = deviceclient_->GetSystemSupportInformationFuture().get();
  ASSERT_GE(getsystemsupportinformationresponse.latency_, 0);
  ASSERT_FALSE(getsystemsupportinformationresponse.Error());
}

TEST_F(TestDeviceService, GetSystemUris)
{
  onvif::device::GetSystemUrisResponse getsystemurisresponse = deviceclient_->GetSystemUrisFuture().get();
  ASSERT_GE(getsystemurisresponse.latency_, 0);
  ASSERT_FALSE(getsystemurisresponse.Error());
}

TEST_F(TestDeviceService, GetUsers)
{
  onvif::device::GetUsersResponse getusersresponse = deviceclient_->GetUsersFuture().get();
  ASSERT_GE(getusersresponse.latency_, 0);
  ASSERT_FALSE(getusersresponse.Error());
}

TEST_F(TestDeviceService, GetWsdlUrl)
{
  const std::string response("response");
  dummywsdlservice_->SetResponse(response);

  onvif::device::GetWsdlUrlResponse getwsdlurlresponse = deviceclient_->GetWsdlUrlFuture().get();
  ASSERT_GE(getwsdlurlresponse.latency_, 0);
  ASSERT_FALSE(getwsdlurlresponse.Error());
  ASSERT_TRUE(getwsdlurlresponse.wsdlurl_.is_initialized());

  CURL* curl = curl_easy_init();
  ASSERT_TRUE(curl);
  curl_easy_setopt(curl, CURLOPT_URL, ((*getwsdlurlresponse.wsdlurl_) + std::string("devicemgmt.wsdl")).c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
  std::string text;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(curl));
  curl_easy_cleanup(curl);
  ASSERT_EQ(text, response);
}

TEST_F(TestDeviceService, GetZeroConfiguration)
{
  onvif::device::GetZeroConfigurationResponse getzeroconfigurationresponse = deviceclient_->GetZeroConfigurationFuture().get();
  ASSERT_GE(getzeroconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getzeroconfigurationresponse.Error());
}

TEST_F(TestDeviceService, LoadCACertificates)
{
  onvif::device::LoadCACertificatesResponse loadcacertificatesresponse = deviceclient_->LoadCACertificatesFuture({ Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))), Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))) }).get();
  ASSERT_GE(loadcacertificatesresponse.latency_, 0);
  ASSERT_FALSE(loadcacertificatesresponse.Error());
}

TEST_F(TestDeviceService, LoadCertificates)
{
  onvif::device::LoadCertificatesResponse loadcertificatesresponse = deviceclient_->LoadCertificatesFuture({ Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))), Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))) }).get();
  ASSERT_GE(loadcertificatesresponse.latency_, 0);
  ASSERT_FALSE(loadcertificatesresponse.Error());
}

TEST_F(TestDeviceService, LoadCertificateWithPrivateKey)
{
  onvif::device::LoadCertificateWithPrivateKeyResponse loadcertificatewithprivatekeyresponse = deviceclient_->LoadCertificateWithPrivateKeyFuture({ CertificateWithPrivateKey(std::string("certificateid1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), BinaryData({ 'd', 'e', 'f' }, std::string("contenttype"))), CertificateWithPrivateKey(std::string("certificateid2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), BinaryData({ 'd', 'e', 'f' }, std::string("contenttype"))) }).get();
  ASSERT_GE(loadcertificatewithprivatekeyresponse.latency_, 0);
  ASSERT_FALSE(loadcertificatewithprivatekeyresponse.Error());
}

TEST_F(TestDeviceService, RemoveIPAddressFilter)
{
  onvif::device::RemoveIPAddressFilterResponse removeipaddressfilterresponse = deviceclient_->RemoveIPAddressFilterFuture(IPAddressFilter(IPADDRESSFILTERTYPE_ALLOW, { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) })).get();
  ASSERT_GE(removeipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(removeipaddressfilterresponse.Error());
}

TEST_F(TestDeviceService, RemoveScopes)
{
  onvif::device::RemoveScopesResponse removescopesresponse = deviceclient_->RemoveScopesFuture({ std::string("test1"), std::string("test2") }).get();
  ASSERT_GE(removescopesresponse.latency_, 0);
  ASSERT_FALSE(removescopesresponse.Error());
}

TEST_F(TestDeviceService, RestoreSystem)
{
  onvif::device::RestoreSystemResponse restoresystemresponse = deviceclient_->RestoreSystemFuture({ BackupFile(std::string("name1"), AttachmentData(Include(std::string("href1"), std::string("data1")), std::string("contenttype1"))), BackupFile(std::string("name2"), AttachmentData(Include(std::string("href2"), std::string("data2")), std::string("contenttype2"))) }).get();
  ASSERT_GE(restoresystemresponse.latency_, 0);
  ASSERT_FALSE(restoresystemresponse.Error());
}

TEST_F(TestDeviceService, ScanAvailableDot11Networks)
{
  onvif::device::ScanAvailableDot11NetworksResponse scanavailabledot11networksresponse = deviceclient_->ScanAvailableDot11NetworksFuture(std::string("interfacetoken")).get();
  ASSERT_GE(scanavailabledot11networksresponse.latency_, 0);
  ASSERT_FALSE(scanavailabledot11networksresponse.Error());
}

TEST_F(TestDeviceService, SendAuxiliaryCommand)
{
  onvif::device::SendAuxiliaryCommandResponse sendauxiliarycommandresponse = deviceclient_->SendAuxiliaryCommandFuture("AuxiliaryCommand").get();
  ASSERT_GE(sendauxiliarycommandresponse.latency_, 0);
  ASSERT_FALSE(sendauxiliarycommandresponse.Error());
}

TEST_F(TestDeviceService, SetAccessPolicy)
{
  onvif::device::SetAccessPolicyResponse setaccesspolicyresponse = deviceclient_->SetAccessPolicyFuture(BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))).get();
  ASSERT_GE(setaccesspolicyresponse.latency_, 0);
  ASSERT_FALSE(setaccesspolicyresponse.Error());
}

TEST_F(TestDeviceService, SetCertificatesStatus)
{
  onvif::device::SetCertificatesStatusResponse setcertificatesstatusresponse = deviceclient_->SetCertificatesStatusFuture({ CertificateStatus(std::string("token1"), true), CertificateStatus(std::string("token2"), true) }).get();
  ASSERT_GE(setcertificatesstatusresponse.latency_, 0);
  ASSERT_FALSE(setcertificatesstatusresponse.Error());
}

TEST_F(TestDeviceService, SetClientCertificateMode)
{
  onvif::device::SetClientCertificateModeResponse setclientcertificatemoderesponse = deviceclient_->SetClientCertificateModeFuture(true).get();
  ASSERT_GE(setclientcertificatemoderesponse.latency_, 0);
  ASSERT_FALSE(setclientcertificatemoderesponse.Error());
}

TEST_F(TestDeviceService, SetDiscoveryMode)
{
  onvif::device::SetDiscoveryModeResponse setdiscoverymoderesponse = deviceclient_->SetDiscoveryModeFuture(DISCOVERYMODE_NONDISCOVERABLE).get();
  ASSERT_GE(setdiscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(setdiscoverymoderesponse.Error());
}

TEST_F(TestDeviceService, SetDNS)
{
  onvif::device::SetDNSResponse setdnsresponse = deviceclient_->SetDNSFuture(true, { std::string("searchdomain1"), std::string("searchdomain2") }, { IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), IPAddress(IPTYPE_IPV4, std::string("6.6.6.6"), boost::none) }).get();
  ASSERT_GE(setdnsresponse.latency_, 0);
  ASSERT_FALSE(setdnsresponse.Error());
}

TEST_F(TestDeviceService, SetDot1XConfiguration)
{
  onvif::device::SetDot1XConfigurationResponse setdot1xconfigurationresponse = deviceclient_->SetDot1XConfigurationFuture(Dot1XConfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password")))).get();
  ASSERT_GE(setdot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setdot1xconfigurationresponse.Error());
}

TEST_F(TestDeviceService, SetDPAddresses)
{
  onvif::device::SetDPAddressesResponse setdpaddressesresponse = deviceclient_->SetDPAddressesFuture({ NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")), NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2")) }).get();
  ASSERT_GE(setdpaddressesresponse.latency_, 0);
  ASSERT_FALSE(setdpaddressesresponse.Error());
}

TEST_F(TestDeviceService, SetDynamicDNS)
{
  onvif::device::SetDynamicDNSResponse setdynamicdnsresponse = deviceclient_->SetDynamicDNSFuture(DYNAMICDNSTYPE_CLIENTUPDATES, std::string("name"), Duration(0, 1, 2, 3, 4, 5)).get();
  ASSERT_GE(setdynamicdnsresponse.latency_, 0);
  ASSERT_FALSE(setdynamicdnsresponse.Error());
}

TEST_F(TestDeviceService, SetHostname)
{
  onvif::device::SetHostnameResponse sethostnameresponse = deviceclient_->SetHostnameFuture(std::string("hostname")).get();
  ASSERT_GE(sethostnameresponse.latency_, 0);
  ASSERT_FALSE(sethostnameresponse.Error());
}

TEST_F(TestDeviceService, SetHostnameFromDHCP)
{
  onvif::device::SetHostnameFromDHCPResponse sethostnamefromdhcpresponse = deviceclient_->SetHostnameFromDHCPFuture(true).get();
  ASSERT_GE(sethostnamefromdhcpresponse.latency_, 0);
  ASSERT_FALSE(sethostnamefromdhcpresponse.Error());
}

TEST_F(TestDeviceService, SetIPAddressFilter)
{
  onvif::device::SetIPAddressFilterResponse setipaddressfilterresponse = deviceclient_->SetIPAddressFilterFuture(IPAddressFilter(IPADDRESSFILTERTYPE_ALLOW, { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) })).get();
  ASSERT_GE(setipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(setipaddressfilterresponse.Error());
}

TEST_F(TestDeviceService, SetNetworkProtocols)
{
  onvif::device::SetNetworkProtocolsResponse setnetworkprotocolsresponse = deviceclient_->SetNetworkProtocolsFuture({ NetworkProtocol(NETWORKPROTOCOLTYPE_HTTP, true,{ 5, 6 }), NetworkProtocol(NETWORKPROTOCOLTYPE_HTTPS, true,{ 5, 6 }) }).get();
  ASSERT_GE(setnetworkprotocolsresponse.latency_, 0);
  ASSERT_FALSE(setnetworkprotocolsresponse.Error());
}

TEST_F(TestDeviceService, SetNetworkDefaultGateway)
{
  onvif::device::SetNetworkDefaultGatewayResponse setnetworkdefaultgatewayresponse = deviceclient_->SetNetworkDefaultGatewayFuture({ std::string("5.5.5.5"), std::string("6.6.6.6") }, { std::string("7.7.7.7"), std::string("8.8.8.8") }).get();
  ASSERT_GE(setnetworkdefaultgatewayresponse.latency_, 0);
  ASSERT_FALSE(setnetworkdefaultgatewayresponse.Error());
}

TEST_F(TestDeviceService, SetNetworkInterfaces)
{
  onvif::device::SetNetworkInterfacesResponse setnetworkinterfacesresponse = deviceclient_->SetNetworkInterfacesFuture(std::string("token"), onvif::NetworkInterfaceSetConfiguration(true, NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), 5, IPv4NetworkInterfaceSetConfiguration(true, { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, true), IPv6NetworkInterfaceSetConfiguration(true, true, { PrefixedIPv6Address(std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), 5), PrefixedIPv6Address(std::string("FE80::0202:B3FF:FE1E:8329"), 6) }, IPV6DHCPCONFIGURATION_STATEFUL), { Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x"))) })).get();
  ASSERT_GE(setnetworkinterfacesresponse.latency_, 0);
  ASSERT_FALSE(setnetworkinterfacesresponse.Error());
}

TEST_F(TestDeviceService, SetNTP)
{
  onvif::device::SetNTPResponse setntpresponse = deviceclient_->SetNTPFuture(false, std::vector<onvif::NetworkHost>()).get();
  ASSERT_GE(setntpresponse.latency_, 0);
  ASSERT_FALSE(setntpresponse.Error());
}

TEST_F(TestDeviceService, GetPkcs10Request)
{
  onvif::device::GetPkcs10RequestResponse getpkcs10requestresponse = deviceclient_->GetPkcs10RequestFuture(std::string("certificateid"), std::string("subject"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype1"))).get();
  ASSERT_GE(getpkcs10requestresponse.latency_, 0);
  ASSERT_FALSE(getpkcs10requestresponse.Error());
}

TEST_F(TestDeviceService, SetRelayOutputSettings)
{
  onvif::device::SetRelayOutputSettingsResponse setrelayoutputsettingsresponse = deviceclient_->SetRelayOutputSettingsFuture(std::string("relayoutputtoken"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN)).get();
  ASSERT_GE(setrelayoutputsettingsresponse.latency_, 0);
  ASSERT_FALSE(setrelayoutputsettingsresponse.Error());
}

TEST_F(TestDeviceService, SetRemoteDiscoveryMode)
{
  onvif::device::SetRemoteDiscoveryModeResponse setremotediscoverymoderesponse = deviceclient_->SetRemoteDiscoveryModeFuture(onvif::DISCOVERYMODE_DISCOVERABLE).get();
  ASSERT_GE(setremotediscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(setremotediscoverymoderesponse.Error());
}

TEST_F(TestDeviceService, SetSystemDateAndTime)
{
  const auto now = boost::posix_time::second_clock::universal_time();
  onvif::device::SetSystemDateAndTimeResponse setsystemdateandtimeresponse = deviceclient_->SetSystemDateAndTimeFuture(DATETIME_MANUAL, true, onvif::TimeZone(std::string("tz")), onvif::DateTime(Time(now.time_of_day().hours(), now.time_of_day().minutes(), now.time_of_day().seconds()), Date(static_cast<int>(now.date().year()), now.date().month().as_number(), now.date().day().as_number()))).get();
  ASSERT_GE(setsystemdateandtimeresponse.latency_, 0);
  ASSERT_FALSE(setsystemdateandtimeresponse.Error());
}

TEST_F(TestDeviceService, SetSystemFactoryDefault)
{
  onvif::device::SetSystemFactoryDefaultResponse setsystemfactorydefaultresponse = deviceclient_->SetSystemFactoryDefaultFuture(onvif::FACTORYDEFAULTTYPE_SOFT).get();
  ASSERT_GE(setsystemfactorydefaultresponse.latency_, 0);
  ASSERT_FALSE(setsystemfactorydefaultresponse.Error());
}

TEST_F(TestDeviceService, SetUser)
{
  onvif::device::SetUserResponse setuserresponse = deviceclient_->SetUserFuture({ onvif::User(std::string("username1"), std::string("password1"), USERLEVEL_ADMINISTRATOR), onvif::User(std::string("username2"), std::string("password2"), USERLEVEL_ANONYMOUS), }).get();
  ASSERT_GE(setuserresponse.latency_, 0);
  ASSERT_FALSE(setuserresponse.Error());
}

TEST_F(TestDeviceService, SystemReboot)
{
  onvif::device::SystemRebootResponse systemrebootresponse = deviceclient_->SystemRebootFuture().get();
  ASSERT_GE(systemrebootresponse.latency_, 0);
  ASSERT_FALSE(systemrebootresponse.Error());
}

TEST_F(TestDeviceService, SetZeroConfiguration)
{
  onvif::device::SetZeroConfigurationResponse setzeroconfigurationresponse = deviceclient_->SetZeroConfigurationFuture(std::string("interfacetoken"), true).get();
  ASSERT_GE(setzeroconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setzeroconfigurationresponse.Error());
}

}

}

}
