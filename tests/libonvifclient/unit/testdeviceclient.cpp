// testdeviceclient.cpp
//

///// Includes /////

#include "testdeviceclient.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 8000;
const std::string DEVICEURI("/onvif/device");

///// Methods /////

TestDeviceClient::TestDeviceClient()
{

}

TestDeviceClient::~TestDeviceClient()
{

}

void TestDeviceClient::SetUp()
{
  Init(PORT, DEVICEURI);

  // Setup dummy server
  dummydeviceservice_ = std::make_unique<DummyDeviceService>();
  ASSERT_FALSE(httpserver_.AddServer(DEVICEURI, dummydeviceservice_.get()));
}

void TestDeviceClient::TearDown()
{
  // Destroy dummy server
  ASSERT_FALSE(httpserver_.RemoveServer(DEVICEURI));
  dummydeviceservice_ = nullptr;

  Destroy();
}

///// Tests /////

TEST_F(TestDeviceClient, AddIPAddressFilter)
{
  const IPAddressFilter ipaddressfilter(IPADDRESSFILTERTYPE_ALLOW,
                                        {
                                          PrefixedIPv4Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv4Address(std::string("6.6.6.6"), 6)
                                        },
                                        {
                                          PrefixedIPv6Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv6Address(std::string("6.6.6.6"), 6)
                                        });

  device::AddIPAddressFilterResponse addipaddressfilterresponse = client_->AddIPAddressFilterFuture(ipaddressfilter).get();
  ASSERT_GE(addipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(addipaddressfilterresponse.Error());
  ASSERT_EQ(ipaddressfilter, addipaddressfilterresponse.ipaddressfilter_);
}

TEST_F(TestDeviceClient, AddScopes)
{
  const std::vector<std::string> scopes =
  {
    std::string("test1"),
    std::string("test2"),
    std::string("test3")
  };

  device::AddScopesResponse addscopesresponse = client_->AddScopesFuture(scopes).get();
  ASSERT_GE(addscopesresponse.latency_, 0);
  ASSERT_FALSE(addscopesresponse.Error());
  ASSERT_EQ(scopes, addscopesresponse.scopes_);
}

TEST_F(TestDeviceClient, CreateCertificate)
{
  boost::optional<std::string> certificateid(std::string("CertificateID"));
  boost::optional<std::string> subject(std::string("Subject"));
  boost::optional<ws::DateTime> validnotbefore(ws::DateTime(2000, 10, 24, 1, 2, 3, 0));
  boost::optional<ws::DateTime> validnotafter(ws::DateTime(2001, 10, 24, 1, 2, 3, 0));
  const onvif::Certificate nvtcertificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")));
  dummydeviceservice_->SetCreateCertificate(nvtcertificate);

  device::CreateCertificateResponse createcertificateresponse = client_->CreateCertificateFuture(certificateid, subject, validnotbefore, validnotafter).get();
  ASSERT_GE(createcertificateresponse.latency_, 0);
  ASSERT_FALSE(createcertificateresponse.Error());
  ASSERT_EQ(certificateid, createcertificateresponse.certificateid_);
  ASSERT_EQ(subject, createcertificateresponse.subject_);
  ASSERT_EQ(validnotbefore, createcertificateresponse.validnotbefore_);
  ASSERT_EQ(validnotafter, createcertificateresponse.validnotafter_);
  ASSERT_EQ(nvtcertificate, createcertificateresponse.nvtcertificate_);
}

TEST_F(TestDeviceClient, CreateStorageConfiguration)
{
  const device::StorageConfigurationData storageconfiguration(std::string("localpath"), std::string("storageuri"), UserCredential(std::string("username"), std::string("password")), std::string("type"));
  const std::string token("Token");
  dummydeviceservice_->SetCreateStorageConfiguration(token);

  device::CreateStorageConfigurationResponse createstorageconfigurationresponse = client_->CreateStorageConfigurationFuture(storageconfiguration).get();
  ASSERT_GE(createstorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(createstorageconfigurationresponse.Error());
  ASSERT_EQ(storageconfiguration, createstorageconfigurationresponse.storageconfiguration_);
  ASSERT_EQ(token, createstorageconfigurationresponse.token_);
}

TEST_F(TestDeviceClient, CreateUsers)
{
  std::vector<User> users =
  {
    User(std::string("username1"), std::string("password1"), USERLEVEL_ADMINISTRATOR),
    User(std::string("username2"), std::string("password2"), USERLEVEL_ADMINISTRATOR),
    User(std::string("username3"), std::string("password3"), USERLEVEL_ADMINISTRATOR),
    User(std::string("username4"), std::string("password4"), USERLEVEL_ADMINISTRATOR)
  };

  device::CreateUsersResponse createusersresponse = client_->CreateUsersFuture(users).get();
  ASSERT_GE(createusersresponse.latency_, 0);
  ASSERT_FALSE(createusersresponse.Error());
  ASSERT_EQ(users, createusersresponse.users_);
}

TEST_F(TestDeviceClient, DeleteCertificates)
{
  std::vector<std::string> certificateid =
  {
    std::string("certificateid1"),
    std::string("certificateid2"),
    std::string("certificateid3")
  };

  device::DeleteCertificatesResponse deletecertificatesresponse = client_->DeleteCertificatesFuture(certificateid).get();
  ASSERT_GE(deletecertificatesresponse.latency_, 0);
  ASSERT_FALSE(deletecertificatesresponse.Error());
  ASSERT_EQ(certificateid, deletecertificatesresponse.certificateid_);
}

TEST_F(TestDeviceClient, DeleteDot1XConfiguration)
{
  std::vector<std::string> dot1xconfigurationtoken =
  {
    std::string("certificateid1"),
    std::string("certificateid2"),
    std::string("certificateid3")
  };

  device::DeleteDot1XConfigurationResponse deletedot1xconfigurationresponse = client_->DeleteDot1XConfigurationFuture(dot1xconfigurationtoken).get();
  ASSERT_GE(deletedot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(deletedot1xconfigurationresponse.Error());
  ASSERT_EQ(dot1xconfigurationtoken, deletedot1xconfigurationresponse.dot1xconfigurationtoken_);
}

TEST_F(TestDeviceClient, DeleteStorageConfiguration)
{
  std::vector<std::string> token =
  {
    std::string("token1"),
    std::string("token2"),
    std::string("token3")
  };

  device::DeleteStorageConfigurationResponse deletestorageconfigurationresponse = client_->DeleteStorageConfigurationFuture(token).get();
  ASSERT_GE(deletestorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(deletestorageconfigurationresponse.Error());
  ASSERT_EQ(token, deletestorageconfigurationresponse.token_);
}

TEST_F(TestDeviceClient, DeleteUsers)
{
  std::vector<std::string> usernames =
  {
    std::string("username1"),
    std::string("username2"),
    std::string("username3"),
    std::string("username4")
  };

  device::DeleteUsersResponse deleteusersresponse = client_->DeleteUsersFuture(usernames).get();
  ASSERT_GE(deleteusersresponse.latency_, 0);
  ASSERT_FALSE(deleteusersresponse.Error());
  ASSERT_EQ(usernames, deleteusersresponse.usernames_);
}

TEST_F(TestDeviceClient, GetAccessPolicy)
{
  const BinaryData policyfile({ 'a', 'b', 'c' }, std::string("contenttype"));
  dummydeviceservice_->SetGetAccessPolicy(policyfile);

  device::GetAccessPolicyResponse getaccesspolicyresponse = client_->GetAccessPolicyFuture().get();
  ASSERT_GE(getaccesspolicyresponse.latency_, 0);
  ASSERT_FALSE(getaccesspolicyresponse.Error());
  ASSERT_TRUE(getaccesspolicyresponse.policyfile_.is_initialized());
  ASSERT_EQ(policyfile, *getaccesspolicyresponse.policyfile_);
}

TEST_F(TestDeviceClient, GetCACertificates)
{
  const std::vector<Certificate> cacertificates =
  {
    Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))),
    Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")))
  };
  dummydeviceservice_->SetGetCACertificates(cacertificates);

  device::GetCACertificatesResponse getcacertificatesresponse = client_->GetCACertificatesFuture().get();
  ASSERT_GE(getcacertificatesresponse.latency_, 0);
  ASSERT_FALSE(getcacertificatesresponse.Error());
  ASSERT_EQ(cacertificates, getcacertificatesresponse.certificates_);
}

TEST_F(TestDeviceClient, GetCapabilities)
{
  const CAPABILITYCATEGORY capabilitycategory = CAPABILITYCATEGORY_ALL;
  const Capabilities capabilities(AnalyticsCapabilities(std::string("xaddr"), true, true),
                                  DeviceCapabilities(std::string("xaddr"),
                                  NetworkCapabilities(true, true, true, true, true),
                                  SystemCapabilities(true, true, true, true, true, true, { onvif::OnvifVersion(5, 6), onvif::OnvifVersion(7, 8) }, true, true, true, true),
                                  IOCapabilities(5, 5),
                                  SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, true)),
                                  EventCapabilities(std::string("xaddr"), true, true, true),
                                  ImagingCapabilities(std::string("xaddr")),
                                  MediaCapabilities(std::string("xaddr"),
                                  RealTimeStreamingCapabilities(true, true, true),
                                  ProfileCapabilities(5)),
                                  PTZCapabilities(std::string("xaddr")),
                                  DeviceIOCapabilities(std::string("xaddr"), 5, 5, 5, 5, 5),
                                  DisplayCapabilities(std::string("xaddr"), true),
                                  RecordingCapabilities(std::string("xaddr"), true, true, true, true, 5),
                                  SearchCapabilities(std::string("xaddr"), true),
                                  ReplayCapabilities(std::string("xaddr")),
                                  ReceiverCapabilities(std::string("xaddr"), true, true, true, 5, 5),
                                  AnalyticsDeviceCapabilities(std::string("xaddr"), true));
  dummydeviceservice_->SetGetCapabilities(capabilities);
  
  device::GetCapabilitiesResponse getcapabilitiesresponse = client_->GetCapabilitiesFuture(capabilitycategory).get();
  ASSERT_GE(getcapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getcapabilitiesresponse.Error());
  ASSERT_EQ(CAPABILITYCATEGORY_ALL, getcapabilitiesresponse.capabilitycategory_);
  ASSERT_TRUE(getcapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getcapabilitiesresponse.capabilities_);
}

TEST_F(TestDeviceClient, GetCertificateInformation)
{
  const std::string certificateid("certificateid");
  const CertificateInformation certificateinformation(std::string("certificateid"), std::string("issuerdn"), std::string("subjectdn"), CertificateUsage(std::string("usage1"), true), CertificateUsage(std::string("usage2"), true), 5, std::string("version"), std::string("serialnum"), std::string("signaturealgorithm"), DateTimeRange(ws::DateTime(std::string("2002-05-30T09:00:00")), ws::DateTime(std::string("2002-05-30T09:00:01"))));
  dummydeviceservice_->SetGetCertificateInformation(certificateinformation);

  device::GetCertificateInformationResponse getcertificateinformationresponse = client_->GetCertificateInformationFuture(certificateid).get();
  ASSERT_GE(getcertificateinformationresponse.latency_, 0);
  ASSERT_FALSE(getcertificateinformationresponse.Error());
  ASSERT_EQ(certificateid, getcertificateinformationresponse.certificateid_);
  ASSERT_TRUE(getcertificateinformationresponse.certificateinformation_.is_initialized());
  ASSERT_EQ(certificateinformation, *getcertificateinformationresponse.certificateinformation_);
}

TEST_F(TestDeviceClient, GetCertificates)
{
  const std::vector<Certificate> certificates =
  {
    Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))),
    Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")))
  };
  dummydeviceservice_->SetGetCertificates(certificates);

  device::GetCertificatesResponse getcertificatesresponse = client_->GetCertificatesFuture().get();
  ASSERT_GE(getcertificatesresponse.latency_, 0);
  ASSERT_FALSE(getcertificatesresponse.Error());
  ASSERT_EQ(certificates, getcertificatesresponse.certificates_);
}

TEST_F(TestDeviceClient, GetCertificatesStatus)
{
  const std::vector<CertificateStatus> certificatesstatus =
  {
    CertificateStatus(std::string("certificateid1"), true),
    CertificateStatus(std::string("certificateid2"), true)
  };
  dummydeviceservice_->SetGetCertificatesStatus(certificatesstatus);

  device::GetCertificatesStatusResponse getcertificatesstatusresponse = client_->GetCertificatesStatusFuture().get();
  ASSERT_GE(getcertificatesstatusresponse.latency_, 0);
  ASSERT_FALSE(getcertificatesstatusresponse.Error());
  ASSERT_EQ(certificatesstatus, getcertificatesstatusresponse.certificatesstatus_);
}

TEST_F(TestDeviceClient, GetClientCertificateMode)
{
  const bool enabled = true;
  dummydeviceservice_->SetGetClientCertificateMode(enabled);

  device::GetClientCertificateModeResponse getclientcertificatemoderesponse = client_->GetClientCertificateModeFuture().get();
  ASSERT_GE(getclientcertificatemoderesponse.latency_, 0);
  ASSERT_FALSE(getclientcertificatemoderesponse.Error());
  ASSERT_TRUE(getclientcertificatemoderesponse.enabled_.is_initialized());
  ASSERT_EQ(enabled, *getclientcertificatemoderesponse.enabled_);
}

TEST_F(TestDeviceClient, GetDeviceInformation)
{
  std::string manufacturer("manufacturer");
  std::string model("model");
  std::string firmwareversion("firmwareversion");
  std::string serialnumber("serialnumber");
  std::string hardwareid("hardwareid");
  dummydeviceservice_->SetGetDeviceInformation(manufacturer, model, firmwareversion, serialnumber, hardwareid);

  device::GetDeviceInformationResponse getdeviceinformationresponse = client_->GetDeviceInformationFuture().get();
  ASSERT_GE(getdeviceinformationresponse.latency_, 0);
  ASSERT_FALSE(getdeviceinformationresponse.Error());
  ASSERT_TRUE(getdeviceinformationresponse.manufacturer_.is_initialized());
  ASSERT_TRUE(getdeviceinformationresponse.model_.is_initialized());
  ASSERT_TRUE(getdeviceinformationresponse.firmwareversion_.is_initialized());
  ASSERT_TRUE(getdeviceinformationresponse.serialnumber_.is_initialized());
  ASSERT_TRUE(getdeviceinformationresponse.hardwareid_.is_initialized());
  ASSERT_EQ(manufacturer, *getdeviceinformationresponse.manufacturer_);
  ASSERT_EQ(model, *getdeviceinformationresponse.model_);
  ASSERT_EQ(firmwareversion, *getdeviceinformationresponse.firmwareversion_);
  ASSERT_EQ(serialnumber, *getdeviceinformationresponse.serialnumber_);
  ASSERT_EQ(hardwareid, *getdeviceinformationresponse.hardwareid_);
}

TEST_F(TestDeviceClient, GetDiscoveryMode)
{
  const DISCOVERYMODE discoverymode = DISCOVERYMODE_DISCOVERABLE;
  dummydeviceservice_->SetGetDiscoveryMode(discoverymode);

  device::GetDiscoveryModeResponse getdiscoverymoderesponse = client_->GetDiscoveryModeFuture().get();
  ASSERT_GE(getdiscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(getdiscoverymoderesponse.Error());
  ASSERT_TRUE(getdiscoverymoderesponse.discoverymode_.is_initialized());
  ASSERT_EQ(discoverymode, *getdiscoverymoderesponse.discoverymode_);
}

TEST_F(TestDeviceClient, GetDNS)
{
  const DNSInformation dnsinformation(true, { std::string("searchdomain1"), std::string("searchdomain2") }, { IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), IPAddress(IPTYPE_IPV4, std::string("6.6.6.6"), boost::none) }, { IPAddress(IPTYPE_IPV4, std::string("7.7.7.7"), boost::none), IPAddress(IPTYPE_IPV4, std::string("8.8.8.8"), boost::none) });
  dummydeviceservice_->SetGetDNS(dnsinformation);

  device::GetDNSResponse getdnsresponse = client_->GetDNSFuture().get();
  ASSERT_GE(getdnsresponse.latency_, 0);
  ASSERT_FALSE(getdnsresponse.Error());
  ASSERT_TRUE(getdnsresponse.dnsinformation_.is_initialized());
  ASSERT_EQ(dnsinformation, *getdnsresponse.dnsinformation_);
}

TEST_F(TestDeviceClient, GetDot11Capabilities)
{
  const Dot11Capabilities capabilities(true, true, true, true, true);
  dummydeviceservice_->SetGetDot11Capabilities(capabilities);

  device::GetDot11CapabilitiesResponse getdot11capabilitiesresponse = client_->GetDot11CapabilitiesFuture().get();
  ASSERT_GE(getdot11capabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getdot11capabilitiesresponse.Error());
  ASSERT_TRUE(getdot11capabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getdot11capabilitiesresponse.capabilities_);
}

TEST_F(TestDeviceClient, GetDot11Status)
{
  const std::string& interfacetoken("interfacetoken");
  const Dot11Status status(std::string("ssid"), std::string("bssid"), DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD, std::string("activeconfigalias"));
  dummydeviceservice_->SetGetDot11Status(status);

  device::GetDot11StatusResponse getdot11statusresponse = client_->GetDot11StatusFuture(interfacetoken).get();
  ASSERT_GE(getdot11statusresponse.latency_, 0);
  ASSERT_FALSE(getdot11statusresponse.Error());
  ASSERT_EQ(interfacetoken, getdot11statusresponse.interfacetoken_);
  ASSERT_TRUE(getdot11statusresponse.status_.is_initialized());
  ASSERT_EQ(status, *getdot11statusresponse.status_);
}

TEST_F(TestDeviceClient, GetDot1XConfiguration)
{
  const std::string dot1xconfigurationtoken("dot1xconfigurationtoken");
  const Dot1XConfiguration dot1xconfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password")));
  dummydeviceservice_->SetGetDot1XConfiguration(dot1xconfiguration);

  device::GetDot1XConfigurationResponse getdot1xconfigurationresponse = client_->GetDot1XConfigurationFuture(dot1xconfigurationtoken).get();
  ASSERT_GE(getdot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getdot1xconfigurationresponse.Error());
  ASSERT_EQ(dot1xconfigurationtoken, getdot1xconfigurationresponse.dot1xconfigurationtoken_);
  ASSERT_TRUE(getdot1xconfigurationresponse.dot1xconfiguration_.is_initialized());
  ASSERT_EQ(dot1xconfiguration, *getdot1xconfigurationresponse.dot1xconfiguration_);
}

TEST_F(TestDeviceClient, GetDot1XConfigurations)
{
  const std::vector<Dot1XConfiguration> dot1xconfiguration =
  {
    Dot1XConfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password"))),
    Dot1XConfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password")))
  };
  dummydeviceservice_->SetGetDot1XConfigurations(dot1xconfiguration);

  device::GetDot1XConfigurationsResponse getdot1xconfigurationsresponse = client_->GetDot1XConfigurationsFuture().get();
  ASSERT_GE(getdot1xconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getdot1xconfigurationsresponse.Error());
  ASSERT_EQ(dot1xconfiguration, getdot1xconfigurationsresponse.dot1xconfiguration_);
}

TEST_F(TestDeviceClient, GetDPAddresses)
{
  const std::vector<NetworkHost> dpaddresses =
  {
    NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
    NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
  };
  dummydeviceservice_->SetGetDPAddresses(dpaddresses);

  device::GetDPAddressesResponse getdpaddressesresponse = client_->GetDPAddressesFuture().get();
  ASSERT_GE(getdpaddressesresponse.latency_, 0);
  ASSERT_FALSE(getdpaddressesresponse.Error());
  ASSERT_EQ(dpaddresses, getdpaddressesresponse.dpaddress_);
}

TEST_F(TestDeviceClient, GetDynamicDNS)
{
  const DynamicDNSInformation dynamicdnsinformation(DYNAMICDNSTYPE_CLIENTUPDATES, std::string("name"), Duration(0, 1, 2, 3, 4, 5));
  dummydeviceservice_->SetGetDynamicDNS(dynamicdnsinformation);

  device::GetDynamicDNSResponse getdynamicdnsresponse = client_->GetDynamicDNSFuture().get();
  ASSERT_GE(getdynamicdnsresponse.latency_, 0);
  ASSERT_FALSE(getdynamicdnsresponse.Error());
  ASSERT_TRUE(getdynamicdnsresponse.dynamicdnsinformation_.is_initialized());
  ASSERT_EQ(dynamicdnsinformation, *getdynamicdnsresponse.dynamicdnsinformation_);
}

TEST_F(TestDeviceClient, GetEndpointReference)
{
  const std::string guid("endpointreference");
  dummydeviceservice_->SetGetEndpointReference(guid);

  device::GetEndpointReferenceResponse getendpointreferenceresponse = client_->GetEndpointReferenceFuture().get();
  ASSERT_GE(getendpointreferenceresponse.latency_, 0);
  ASSERT_FALSE(getendpointreferenceresponse.Error());
  ASSERT_TRUE(getendpointreferenceresponse.guid_.is_initialized());
  ASSERT_EQ(guid, *getendpointreferenceresponse.guid_);
}

TEST_F(TestDeviceClient, GetHostname)
{
  const HostnameInformation hostnameinformation(true, std::string("name"));
  dummydeviceservice_->SetGetHostname(hostnameinformation);

  device::GetHostnameResponse gethostnameresponse = client_->GetHostnameFuture().get();
  ASSERT_GE(gethostnameresponse.latency_, 0);
  ASSERT_FALSE(gethostnameresponse.Error());
  ASSERT_TRUE(gethostnameresponse.hostnameinformation_.is_initialized());
  ASSERT_EQ(hostnameinformation, *gethostnameresponse.hostnameinformation_);
}

TEST_F(TestDeviceClient, GetIPAddressFilter)
{
  const IPAddressFilter ipaddressfilter(IPADDRESSFILTERTYPE_ALLOW,
                                        {
                                          PrefixedIPv4Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv4Address(std::string("6.6.6.6"), 6)
                                        },
                                        {
                                          PrefixedIPv6Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv6Address(std::string("6.6.6.6"), 6)
                                        });
  dummydeviceservice_->SetGetIPAddressFilter(ipaddressfilter);

  device::GetIPAddressFilterResponse getipaddressfilterresponse = client_->GetIPAddressFilterFuture().get();
  ASSERT_GE(getipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(getipaddressfilterresponse.Error());
  ASSERT_TRUE(getipaddressfilterresponse.ipaddressfilter_.is_initialized());
  ASSERT_EQ(ipaddressfilter, *getipaddressfilterresponse.ipaddressfilter_);
}

TEST_F(TestDeviceClient, GetNetworkDefaultGateway)
{
  const NetworkGateway networkgateway({ "5.5.5.5", "6.6.6.6" }, { "FE80:0000:0000:0000:0202:B3FF:FE1E:8329", "FE80::0202:B3FF:FE1E:8329" });
  dummydeviceservice_->SetGetNetworkDefaultGateway(networkgateway);

  device::GetNetworkDefaultGatewayResponse getnetworkdefaultgateway = client_->GetNetworkDefaultGatewayFuture().get();
  ASSERT_GE(getnetworkdefaultgateway.latency_, 0);
  ASSERT_FALSE(getnetworkdefaultgateway.Error());
  ASSERT_TRUE(getnetworkdefaultgateway.networkgateway_.is_initialized());
  ASSERT_EQ(networkgateway, *getnetworkdefaultgateway.networkgateway_);
}

TEST_F(TestDeviceClient, GetNetworkInterfaces)
{
  const std::vector<NetworkInterface> networkinterfaces = 
  {
    NetworkInterface(std::string("token1"), true, NetworkInterfaceInfo(std::string("name1"), std::string("hwaddress1"), 5), NetworkInterfaceLink(NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), 5), IPv4NetworkInterface(true, IPv4Configuration({ PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, PrefixedIPv4Address(std::string("7.7.7.7"), 7), PrefixedIPv4Address(std::string("8.8.8.8"), 8), true)), IPv6NetworkInterface(true, IPv6Configuration(true, IPV6DHCPCONFIGURATION_STATEFUL, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("7.7.7.7"), 7), PrefixedIPv6Address(std::string("8.8.8.8"), 8) }, { PrefixedIPv6Address(std::string("9.9.9.9"), 9), PrefixedIPv6Address(std::string("10.10.10.10"), 10) }, { PrefixedIPv6Address(std::string("11.11.11.11"), 11), PrefixedIPv6Address(std::string("12.12.12.12"), 12) })), Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x")))),
    NetworkInterface(std::string("token2"), true, NetworkInterfaceInfo(std::string("name2"), std::string("hwaddress2"), 5), NetworkInterfaceLink(NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), 5), IPv4NetworkInterface(true, IPv4Configuration({ PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, PrefixedIPv4Address(std::string("7.7.7.7"), 7), PrefixedIPv4Address(std::string("8.8.8.8"), 8), true)), IPv6NetworkInterface(true, IPv6Configuration(true, IPV6DHCPCONFIGURATION_STATEFUL, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("7.7.7.7"), 7), PrefixedIPv6Address(std::string("8.8.8.8"), 8) }, { PrefixedIPv6Address(std::string("9.9.9.9"), 9), PrefixedIPv6Address(std::string("10.10.10.10"), 10) }, { PrefixedIPv6Address(std::string("11.11.11.11"), 11), PrefixedIPv6Address(std::string("12.12.12.12"), 12) })), Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x"))))
  };
  dummydeviceservice_->SetGetNetworkInterfaces(networkinterfaces);

  device::GetNetworkInterfacesResponse getnetworkinterfacesresponse = client_->GetNetworkInterfacesFuture().get();
  ASSERT_GE(getnetworkinterfacesresponse.latency_, 0);
  ASSERT_FALSE(getnetworkinterfacesresponse.Error());
  ASSERT_EQ(networkinterfaces, getnetworkinterfacesresponse.networkinterfaces_);
}

TEST_F(TestDeviceClient, GetNetworkProtocols)
{
  const std::vector<NetworkProtocol> networkprotocols = 
  {
    NetworkProtocol(NETWORKPROTOCOLTYPE_HTTP, true, { 5, 6 }),
    NetworkProtocol(NETWORKPROTOCOLTYPE_HTTPS, true, { 5, 6 })
  };
  dummydeviceservice_->SetGetNetworkProtocols(networkprotocols);

  device::GetNetworkProtocolsResponse getnetworkprotocolsresponse = client_->GetNetworkProtocolsFuture().get();
  ASSERT_GE(getnetworkprotocolsresponse.latency_, 0);
  ASSERT_FALSE(getnetworkprotocolsresponse.Error());
  ASSERT_EQ(networkprotocols, getnetworkprotocolsresponse.networkprotocols_);
}

TEST_F(TestDeviceClient, GetNTP)
{
  const NTPInformation ntpinformation(true,
                                      {
                                        NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
                                        NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
                                      },
                                      {
                                        NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("7.7.7.7"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
                                        NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("8.8.8.8"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
                                      });
  dummydeviceservice_->SetGetNTP(ntpinformation);

  device::GetNTPResponse getntpresponse = client_->GetNTPFuture().get();
  ASSERT_GE(getntpresponse.latency_, 0);
  ASSERT_FALSE(getntpresponse.Error());
  ASSERT_TRUE(getntpresponse.ntpinformation_.is_initialized());
  ASSERT_EQ(ntpinformation, *getntpresponse.ntpinformation_);
}

TEST_F(TestDeviceClient, GetPkcs10Request)
{
  const std::string certificateid("certificateid");
  const boost::optional<std::string> subject(std::string("subject"));
  const boost::optional<BinaryData> attributes(BinaryData({ 'a', 'b', 'c' }, std::string("contenttype1")));
  boost::optional<BinaryData> pkcs10reqest(BinaryData({ 'a', 'b', 'c' }, std::string("contenttype2")));
  dummydeviceservice_->SetGetPkcs10Request(*pkcs10reqest);

  device::GetPkcs10RequestResponse getpkcs10requestresponse = client_->GetPkcs10RequestFuture(certificateid, subject, attributes).get();
  ASSERT_GE(getpkcs10requestresponse.latency_, 0);
  ASSERT_FALSE(getpkcs10requestresponse.Error());
  ASSERT_EQ(certificateid, getpkcs10requestresponse.certificateid_);
  ASSERT_EQ(subject, getpkcs10requestresponse.subject_);
  ASSERT_EQ(attributes, getpkcs10requestresponse.attributes_);
  ASSERT_EQ(pkcs10reqest, getpkcs10requestresponse.pkcs10request_);
}

TEST_F(TestDeviceClient, GetRelayOutputs)
{
  const std::vector<RelayOutput> relayoutputs =
  {
    RelayOutput(std::string("token1"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN)),
    RelayOutput(std::string("token2"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN))
  };
  dummydeviceservice_->SetGetRelayOutputs(relayoutputs);

  device::GetRelayOutputsResponse getrelayoutputsresponse = client_->GetRelayOutputsFuture().get();
  ASSERT_GE(getrelayoutputsresponse.latency_, 0);
  ASSERT_FALSE(getrelayoutputsresponse.Error());
  ASSERT_EQ(relayoutputs, getrelayoutputsresponse.relayoutputs_);
}

TEST_F(TestDeviceClient, GetRemoteDiscoveryMode)
{
  const DISCOVERYMODE remotediscoverymode = DISCOVERYMODE_NONDISCOVERABLE;
  dummydeviceservice_->SetGetRemoteDiscoveryMode(remotediscoverymode);

  device::GetRemoteDiscoveryModeResponse getremotediscoverymoderesponse = client_->GetRemoteDiscoveryModeFuture().get();
  ASSERT_GE(getremotediscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(getremotediscoverymoderesponse.Error());
  ASSERT_TRUE(getremotediscoverymoderesponse.remotediscoverymode_.is_initialized());
  ASSERT_EQ(remotediscoverymode, *getremotediscoverymoderesponse.remotediscoverymode_);
}

TEST_F(TestDeviceClient, GetRemoteUser)
{
  const RemoteUser remoteuser(std::string("username"), std::string("password"), true);
  dummydeviceservice_->SetGetRemoteUser(remoteuser);

  device::GetRemoteUserResponse getremoteuserresponse = client_->GetRemoteUserFuture().get();
  ASSERT_GE(getremoteuserresponse.latency_, 0);
  ASSERT_FALSE(getremoteuserresponse.Error());
  ASSERT_TRUE(getremoteuserresponse.remoteuser_.is_initialized());
  ASSERT_EQ(remoteuser, *getremoteuserresponse.remoteuser_);
}

TEST_F(TestDeviceClient, GetScopes)
{
  const std::vector<Scope> scopes =
  {
    Scope(SCOPEDEFINITION_FIXED, std::string("test1")),
    Scope(SCOPEDEFINITION_FIXED, std::string("test2")),
    Scope(SCOPEDEFINITION_FIXED, std::string("test3"))
  };
  dummydeviceservice_->SetGetScopes(scopes);

  device::GetScopesResponse getscopesresponse = client_->GetScopesFuture().get();
  ASSERT_GE(getscopesresponse.latency_, 0);
  ASSERT_FALSE(getscopesresponse.Error());
  ASSERT_EQ(scopes, getscopesresponse.scopes_);
}

TEST_F(TestDeviceClient, GetServiceCapabilities)
{
  const device::DeviceServiceCapabilities capabilities(device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true), device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5), device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true), device::MiscCapabilities(StringAttrList(std::vector<std::string>({ "test1", "test2", "test3" }))));
  dummydeviceservice_->SetGetServiceCapabilities(capabilities);

  device::GetServiceCapabilitiesResponse getservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getservicecapabilitiesresponse.Error());
  ASSERT_EQ(capabilities, *getservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestDeviceClient, GetServices)
{
  const std::vector<device::Service> services =
  {
    device::Service("namespace1", "xaddr1", boost::none, OnvifVersion(5, 5)),
    device::Service("namespace2", "xaddr2", boost::none, OnvifVersion(6, 6)),
    device::Service("namespace3", "xaddr3", boost::none, OnvifVersion(7, 7))
  };
  dummydeviceservice_->SetGetServices(services);

  device::GetServicesResponse getservicesresponse = client_->GetServicesFuture(true).get();
  ASSERT_GE(getservicesresponse.latency_, 0);
  ASSERT_FALSE(getservicesresponse.Error());
  ASSERT_EQ(services, getservicesresponse.services_);
}

TEST_F(TestDeviceClient, GetStorageConfiguration)
{
  const std::string token("token");
  const device::StorageConfiguration storageconfiguration(std::string("token1"), device::StorageConfigurationData(std::string("localpath1"), std::string("storageuri1"), UserCredential(std::string("username1"), std::string("password1")), std::string("type1")));
  dummydeviceservice_->SetGetStorageConfiguration(storageconfiguration);

  device::GetStorageConfigurationResponse getstorageconfigurationresponse = client_->GetStorageConfigurationFuture(token).get();
  ASSERT_GE(getstorageconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getstorageconfigurationresponse.Error());
  ASSERT_EQ(token, getstorageconfigurationresponse.token_);
  ASSERT_TRUE(getstorageconfigurationresponse.storageconfiguration_.is_initialized());
  ASSERT_EQ(storageconfiguration, *getstorageconfigurationresponse.storageconfiguration_);
}

TEST_F(TestDeviceClient, GetStorageConfigurations)
{
  const std::vector<device::StorageConfiguration> storageconfigurations =
  {
    device::StorageConfiguration(std::string("token1"), device::StorageConfigurationData(std::string("localpath1"), std::string("storageuri1"), UserCredential(std::string("username1"), std::string("password1")), std::string("type1"))),
    device::StorageConfiguration(std::string("token2"), device::StorageConfigurationData(std::string("localpath2"), std::string("storageuri2"), UserCredential(std::string("username2"), std::string("password2")), std::string("type2")))
  };
  dummydeviceservice_->SetGetStorageConfigurations(storageconfigurations);

  device::GetStorageConfigurationsResponse getstorageconfigurationsresponse = client_->GetStorageConfigurationsFuture().get();
  ASSERT_GE(getstorageconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getstorageconfigurationsresponse.Error());
  ASSERT_EQ(storageconfigurations, getstorageconfigurationsresponse.storageconfigurations_);
}

TEST_F(TestDeviceClient, GetSystemBackup)
{
  const std::vector<BackupFile> backupfiles =
  {
    BackupFile(std::string("name1"), AttachmentData(Include(std::string("href1"), std::string("data1")), std::string("contenttype1"))),
    BackupFile(std::string("name2"), AttachmentData(Include(std::string("href2"), std::string("data2")), std::string("contenttype2")))
  };
  dummydeviceservice_->SetGetSystemBackup(backupfiles);

  device::GetSystemBackupResponse getsystembackupresponse = client_->GetSystemBackupFuture().get();
  ASSERT_GE(getsystembackupresponse.latency_, 0);
  ASSERT_FALSE(getsystembackupresponse.Error());
  ASSERT_EQ(backupfiles, getsystembackupresponse.backupfiles_);
}

TEST_F(TestDeviceClient, GetSystemDateAndTime)
{
  const SystemDateTime systemdatetime =
  {
    DATETIME_MANUAL,
    true,
    TimeZone(std::string("timezone")),
    DateTime(boost::posix_time::second_clock::universal_time()),
    DateTime(boost::posix_time::second_clock::universal_time())
  };
  dummydeviceservice_->SetGetSystemDateTime(systemdatetime);

  device::GetSystemDateAndTimeResponse getsystemdateandtimeresponse = client_->GetSystemDateAndTimeFuture().get();
  ASSERT_GE(getsystemdateandtimeresponse.latency_, 0);
  ASSERT_FALSE(getsystemdateandtimeresponse.Error());
  ASSERT_TRUE(getsystemdateandtimeresponse.systemdatetime_.is_initialized());
  ASSERT_EQ(systemdatetime, *getsystemdateandtimeresponse.systemdatetime_);
}

TEST_F(TestDeviceClient, GetSystemLog)
{
  const SYSTEMLOGTYPE logtype = SYSTEMLOGLEVEL_SYSTEM;
  const SystemLog systemlog(AttachmentData(Include(std::string("href"), std::string("data")), std::string("contenttype")), std::string("string"));
  dummydeviceservice_->SetGetSystemLog(systemlog);

  device::GetSystemLogResponse getsystemlogresponse = client_->GetSystemLogFuture(logtype).get();
  ASSERT_GE(getsystemlogresponse.latency_, 0);
  ASSERT_FALSE(getsystemlogresponse.Error());
  ASSERT_EQ(logtype, getsystemlogresponse.LOGLEVEL_);
  ASSERT_TRUE(getsystemlogresponse.systemlog_.is_initialized());
  ASSERT_EQ(systemlog, *getsystemlogresponse.systemlog_);
}

TEST_F(TestDeviceClient, GetSystemSupportInformation)
{
  const SupportInformation supportinformation(BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), std::string("string"));
  dummydeviceservice_->SetGetSystemSupportInformation(supportinformation);

  device::GetSystemSupportInformationResponse getsystemsupportinformationresponse = client_->GetSystemSupportInformationFuture().get();
  ASSERT_GE(getsystemsupportinformationresponse.latency_, 0);
  ASSERT_FALSE(getsystemsupportinformationresponse.Error());
  ASSERT_TRUE(getsystemsupportinformationresponse.supportinformation_.is_initialized());
  ASSERT_EQ(supportinformation, *getsystemsupportinformationresponse.supportinformation_);
}

TEST_F(TestDeviceClient, GetSystemUris)
{
  boost::optional<SystemLogUriList> systemloguris(SystemLogUriList({ SystemLogUri(SYSTEMLOGLEVEL_ACCESS, std::string("uri1")), SystemLogUri(SYSTEMLOGLEVEL_SYSTEM, std::string("uri2")) }));
  boost::optional<std::string> supportinfouri(std::string("SupportInfoUri"));
  boost::optional<std::string> systembackupuri(std::string("SystemBackupUri"));
  dummydeviceservice_->SetGetSystemUris(*systemloguris, *supportinfouri, *systembackupuri);

  device::GetSystemUrisResponse getsystemurisresponse = client_->GetSystemUrisFuture().get();
  ASSERT_GE(getsystemurisresponse.latency_, 0);
  ASSERT_FALSE(getsystemurisresponse.Error());
  ASSERT_EQ(systemloguris, getsystemurisresponse.systemloguris_);
  ASSERT_EQ(supportinfouri, getsystemurisresponse.supportinfouri_);
  ASSERT_EQ(systembackupuri, getsystemurisresponse.systembackupuri_);
}

TEST_F(TestDeviceClient, GetUsers)
{
  const std::vector<User> users =
  {
    User(std::string("username1"), std::string("password1"), USERLEVEL_ADMINISTRATOR),
    User(std::string("username2"), std::string("password2"), USERLEVEL_ANONYMOUS),
    User(std::string("username3"), std::string("password3"), USERLEVEL_EXTENDED),
    User(std::string("username4"), std::string("password4"), USERLEVEL_OPERATOR)
  };
  dummydeviceservice_->SetGetUsers(users);

  device::GetUsersResponse getusersresponse = client_->GetUsersFuture().get();
  ASSERT_GE(getusersresponse.latency_, 0);
  ASSERT_FALSE(getusersresponse.Error());
  ASSERT_EQ(users, getusersresponse.users_);
}

TEST_F(TestDeviceClient, GetWsdlUrl)
{
  const std::string wsdlurl("wsdlurl");
  dummydeviceservice_->SetGetWsdlUrl(wsdlurl);

  device::GetWsdlUrlResponse getwsdlurlresponse = client_->GetWsdlUrlFuture().get();
  ASSERT_GE(getwsdlurlresponse.latency_, 0);
  ASSERT_FALSE(getwsdlurlresponse.Error());
  ASSERT_TRUE(getwsdlurlresponse.wsdlurl_.is_initialized());
  ASSERT_EQ(wsdlurl, *getwsdlurlresponse.wsdlurl_);
}

TEST_F(TestDeviceClient, GetZeroConfiguration)
{
  const NetworkZeroConfiguration zeroconfiguration(std::string("interfacetoken1"), true, { std::string("5.5.5.5"), std::string("6.6.6.6") }, { NetworkZeroConfiguration(std::string("interfacetoken2"), true,{ std::string("6.6.6.6"), std::string("7.7.7.7") },{}) });
  dummydeviceservice_->SetGetZeroConfiguration(zeroconfiguration);

  device::GetZeroConfigurationResponse getzeroconfigurationresponse = client_->GetZeroConfigurationFuture().get();
  ASSERT_GE(getzeroconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getzeroconfigurationresponse.Error());
  ASSERT_TRUE(getzeroconfigurationresponse.zeroconfiguration_.is_initialized());
  ASSERT_EQ(zeroconfiguration, *getzeroconfigurationresponse.zeroconfiguration_);
}

TEST_F(TestDeviceClient, LoadCACertificates)
{
  const std::vector<Certificate> cacertificates =
  {
    Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))),
    Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")))
  };

  device::LoadCACertificatesResponse loadcacertificatesresponse = client_->LoadCACertificatesFuture(cacertificates).get();
  ASSERT_GE(loadcacertificatesresponse.latency_, 0);
  ASSERT_FALSE(loadcacertificatesresponse.Error());
  ASSERT_EQ(cacertificates, loadcacertificatesresponse.cacertificate_);
}

TEST_F(TestDeviceClient, LoadCertificates)
{
  const std::vector<Certificate> nvtcertificates =
  {
    Certificate(std::string("token1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype"))),
    Certificate(std::string("token2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")))
  };

  device::LoadCertificatesResponse loadcertificatesresponse = client_->LoadCertificatesFuture(nvtcertificates).get();
  ASSERT_GE(loadcertificatesresponse.latency_, 0);
  ASSERT_FALSE(loadcertificatesresponse.Error());
  ASSERT_EQ(nvtcertificates, loadcertificatesresponse.nvtcertificate_);
}

TEST_F(TestDeviceClient, LoadCertificateWithPrivateKey)
{
  const std::vector<CertificateWithPrivateKey> certificatewithprivatekey =
  {
    CertificateWithPrivateKey(std::string("certificateid1"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), BinaryData({ 'd', 'e', 'f' }, std::string("contenttype"))),
    CertificateWithPrivateKey(std::string("certificateid2"), BinaryData({ 'a', 'b', 'c' }, std::string("contenttype")), BinaryData({ 'd', 'e', 'f' }, std::string("contenttype")))
  };

  device::LoadCertificateWithPrivateKeyResponse loadcertificatewithprivatekeyresponse = client_->LoadCertificateWithPrivateKeyFuture(certificatewithprivatekey).get();
  ASSERT_GE(loadcertificatewithprivatekeyresponse.latency_, 0);
  ASSERT_FALSE(loadcertificatewithprivatekeyresponse.Error());
  ASSERT_EQ(certificatewithprivatekey, loadcertificatewithprivatekeyresponse.certificatewithprivatekey_);
}

TEST_F(TestDeviceClient, RemoveIPAddressFilter)
{
  const IPAddressFilter ipaddressfilter(IPADDRESSFILTERTYPE_ALLOW,
                                        {
                                          PrefixedIPv4Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv4Address(std::string("6.6.6.6"), 6)
                                        },
                                        {
                                          PrefixedIPv6Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv6Address(std::string("6.6.6.6"), 6)
                                        });

  device::RemoveIPAddressFilterResponse removeipaddressfilterresponse = client_->RemoveIPAddressFilterFuture(ipaddressfilter).get();
  ASSERT_GE(removeipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(removeipaddressfilterresponse.Error());
  ASSERT_EQ(ipaddressfilter, removeipaddressfilterresponse.ipaddressfilter_);
}

TEST_F(TestDeviceClient, RemoveScopes)
{
  const std::vector<std::string> scopes =
  {
    std::string("test1"),
    std::string("test2")
  };

  device::RemoveScopesResponse removescopesresponse = client_->RemoveScopesFuture(scopes).get();
  ASSERT_GE(removescopesresponse.latency_, 0);
  ASSERT_FALSE(removescopesresponse.Error());
  ASSERT_EQ(scopes, removescopesresponse.scopes_);
}

TEST_F(TestDeviceClient, RestoreSystem)
{
  const std::vector<BackupFile> backupfiles =
  {
    BackupFile(std::string("name1"), AttachmentData(Include(std::string("href1"), std::string("data1")), std::string("contenttype1"))),
    BackupFile(std::string("name2"), AttachmentData(Include(std::string("href2"), std::string("data2")), std::string("contenttype2")))
  };

  device::RestoreSystemResponse restoresystemresponse = client_->RestoreSystemFuture(backupfiles).get();
  ASSERT_GE(restoresystemresponse.latency_, 0);
  ASSERT_FALSE(restoresystemresponse.Error());
  ASSERT_EQ(backupfiles, restoresystemresponse.backupfiles_);
}

TEST_F(TestDeviceClient, ScanAvailableDot11Networks)
{
  const std::string interfacetoken("interfacetoken");
  const std::vector<Dot11AvailableNetworks> networks =
  {
    Dot11AvailableNetworks(std::string("ssid1"), std::string("bssid1"),{ DOT11AUTHANDMANAGEMENTSUITE_DOT1X, DOT11AUTHANDMANAGEMENTSUITE_PSK }, DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD),
    Dot11AvailableNetworks(std::string("ssid2"), std::string("bssid2"),{ DOT11AUTHANDMANAGEMENTSUITE_DOT1X, DOT11AUTHANDMANAGEMENTSUITE_PSK }, DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD)
  };
  dummydeviceservice_->SetScanAvailableDot11Networks(networks);

  device::ScanAvailableDot11NetworksResponse scanavailabledot11networksresponse = client_->ScanAvailableDot11NetworksFuture(interfacetoken).get();
  ASSERT_GE(scanavailabledot11networksresponse.latency_, 0);
  ASSERT_FALSE(scanavailabledot11networksresponse.Error());
  ASSERT_EQ(interfacetoken, scanavailabledot11networksresponse.interfacetoken_);
  ASSERT_EQ(networks, scanavailabledot11networksresponse.networks_);
}

TEST_F(TestDeviceClient, SendAuxiliaryCommand)
{
  const std::string auxiliarycommand("AuxiliaryCommand");
  const std::string auxiliarycommandresponse("AuxiliaryCommandResponse");
  dummydeviceservice_->SetSendAuxiliaryCommand(auxiliarycommandresponse);

  device::SendAuxiliaryCommandResponse sendauxiliarycommandresponse = client_->SendAuxiliaryCommandFuture(auxiliarycommand).get();
  ASSERT_GE(sendauxiliarycommandresponse.latency_, 0);
  ASSERT_FALSE(sendauxiliarycommandresponse.Error());
  ASSERT_EQ(auxiliarycommand, sendauxiliarycommandresponse.auxiliarycommand_);
  ASSERT_EQ(auxiliarycommandresponse, sendauxiliarycommandresponse.auxiliarycommandresponse_);
}

TEST_F(TestDeviceClient, SetAccessPolicy)
{
  const BinaryData policyfile({ 'a', 'b', 'c' }, std::string("contenttype"));

  device::SetAccessPolicyResponse setaccesspolicyresponse = client_->SetAccessPolicyFuture(policyfile).get();
  ASSERT_GE(setaccesspolicyresponse.latency_, 0);
  ASSERT_FALSE(setaccesspolicyresponse.Error());
  ASSERT_EQ(policyfile, setaccesspolicyresponse.policyfile_);
}

TEST_F(TestDeviceClient, SetCertificatesStatus)
{
  const std::vector<CertificateStatus> certificatestatus =
  {
    CertificateStatus(std::string("token1"), true),
    CertificateStatus(std::string("token2"), true)
  };

  device::SetCertificatesStatusResponse setcertificatesstatusresponse = client_->SetCertificatesStatusFuture(certificatestatus).get();
  ASSERT_GE(setcertificatesstatusresponse.latency_, 0);
  ASSERT_FALSE(setcertificatesstatusresponse.Error());
  ASSERT_EQ(certificatestatus, setcertificatesstatusresponse.certificatestatus_);
}

TEST_F(TestDeviceClient, SetClientCertificateMode)
{
  const bool enabled = true;

  device::SetClientCertificateModeResponse setclientcertificatemoderesponse = client_->SetClientCertificateModeFuture(enabled).get();
  ASSERT_GE(setclientcertificatemoderesponse.latency_, 0);
  ASSERT_FALSE(setclientcertificatemoderesponse.Error());
  ASSERT_EQ(enabled, setclientcertificatemoderesponse.enabled_);
}

TEST_F(TestDeviceClient, SetDiscoveryMode)
{
  const DISCOVERYMODE discoverymode = DISCOVERYMODE_NONDISCOVERABLE;

  device::SetDiscoveryModeResponse setdiscoverymoderesponse = client_->SetDiscoveryModeFuture(discoverymode).get();
  ASSERT_GE(setdiscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(setdiscoverymoderesponse.Error());
  ASSERT_EQ(discoverymode, setdiscoverymoderesponse.discoverymode_);
}

TEST_F(TestDeviceClient, SetDNS)
{
  const bool fromdhcp = true;
  const std::vector<std::string> searchdomain =
  {
    std::string("searchdomain1"),
    std::string("searchdomain2")
  };
  const std::vector<IPAddress> dnsmanual =
  {
    IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none),
    IPAddress(IPTYPE_IPV4, std::string("6.6.6.6"), boost::none)
  };

  device::SetDNSResponse setdnsresponse = client_->SetDNSFuture(fromdhcp, searchdomain, dnsmanual).get();
  ASSERT_GE(setdnsresponse.latency_, 0);
  ASSERT_FALSE(setdnsresponse.Error());
  ASSERT_EQ(fromdhcp, setdnsresponse.fromdhcp_);
  ASSERT_EQ(searchdomain, setdnsresponse.searchdomain_);
  ASSERT_EQ(dnsmanual, setdnsresponse.dnsmanual_);
}

TEST_F(TestDeviceClient, SetDot1XConfiguration)
{
  const Dot1XConfiguration dot1xconfiguration(std::string("Dot1XConfigurationToken"), std::string("identity"), std::string("anonymousid"), 5, std::string("certificateid"), EAPMethodConfiguration(TLSConfiguration(std::string("CertificateID")), std::string("password")));

  device::SetDot1XConfigurationResponse setdot1xconfigurationresponse = client_->SetDot1XConfigurationFuture(dot1xconfiguration).get();
  ASSERT_GE(setdot1xconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setdot1xconfigurationresponse.Error());
  ASSERT_EQ(dot1xconfiguration, setdot1xconfigurationresponse.dot1xconfiguration_);
}

TEST_F(TestDeviceClient, SetDPAddresses)
{
  const std::vector<NetworkHost> dpaddress =
  {
    NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
    NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
  };

  device::SetDPAddressesResponse setdpaddressesresponse = client_->SetDPAddressesFuture(dpaddress).get();
  ASSERT_GE(setdpaddressesresponse.latency_, 0);
  ASSERT_FALSE(setdpaddressesresponse.Error());
  ASSERT_EQ(dpaddress, setdpaddressesresponse.dpaddress_);
}

TEST_F(TestDeviceClient, SetDynamicDNS)
{
  const DYNAMICDNSTYPE type = DYNAMICDNSTYPE_CLIENTUPDATES;
  const std::string name("name");
  const Duration ttl(0, 1, 2, 3, 4, 5);

  device::SetDynamicDNSResponse setdynamicdnsresponse = client_->SetDynamicDNSFuture(type, name, ttl).get();
  ASSERT_GE(setdynamicdnsresponse.latency_, 0);
  ASSERT_FALSE(setdynamicdnsresponse.Error());
  ASSERT_EQ(type, setdynamicdnsresponse.type_);
  ASSERT_TRUE(setdynamicdnsresponse.name_.is_initialized());
  ASSERT_EQ(name, *setdynamicdnsresponse.name_);
  ASSERT_TRUE(setdynamicdnsresponse.ttl_.is_initialized());
  ASSERT_EQ(ttl, *setdynamicdnsresponse.ttl_);
}

TEST_F(TestDeviceClient, SetHostname)
{
  const std::string name("name");

  device::SetHostnameResponse sethostnameresponse = client_->SetHostnameFuture(name).get();
  ASSERT_GE(sethostnameresponse.latency_, 0);
  ASSERT_FALSE(sethostnameresponse.Error());
  ASSERT_EQ(name, sethostnameresponse.name_);
}

TEST_F(TestDeviceClient, SetHostnameFromDHCP)
{
  const bool fromdhcp = true;

  device::SetHostnameFromDHCPResponse sethostnamefromdhcpresponse = client_->SetHostnameFromDHCPFuture(fromdhcp).get();
  ASSERT_GE(sethostnamefromdhcpresponse.latency_, 0);
  ASSERT_FALSE(sethostnamefromdhcpresponse.Error());
  ASSERT_EQ(fromdhcp, sethostnamefromdhcpresponse.fromdhcp_);
}

TEST_F(TestDeviceClient, SetIPAddressFilter)
{
  const IPAddressFilter ipaddressfilter(IPADDRESSFILTERTYPE_ALLOW,
                                        {
                                          PrefixedIPv4Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv4Address(std::string("6.6.6.6"), 6)
                                        },
                                        {
                                          PrefixedIPv6Address(std::string("5.5.5.5"), 5),
                                          PrefixedIPv6Address(std::string("6.6.6.6"), 6)
                                        });
  
  device::SetIPAddressFilterResponse setipaddressfilterresponse = client_->SetIPAddressFilterFuture(ipaddressfilter).get();
  ASSERT_GE(setipaddressfilterresponse.latency_, 0);
  ASSERT_FALSE(setipaddressfilterresponse.Error());
  ASSERT_EQ(ipaddressfilter, setipaddressfilterresponse.ipaddressfilter_);
}

TEST_F(TestDeviceClient, SetNetworkDefaultGateway)
{
  const std::vector<std::string> ipv4address =
  {
    std::string("5.5.5.5"),
    std::string("6.6.6.6")
  };
  const std::vector<std::string> ipv6address =
  {
    std::string("7.7.7.7"),
    std::string("8.8.8.8")
  };
  device::SetNetworkDefaultGatewayResponse setnetworkdefaultgatewayresponse = client_->SetNetworkDefaultGatewayFuture(ipv4address, ipv6address).get();
  ASSERT_GE(setnetworkdefaultgatewayresponse.latency_, 0);
  ASSERT_FALSE(setnetworkdefaultgatewayresponse.Error());
  ASSERT_EQ(ipv4address, setnetworkdefaultgatewayresponse.ipv4address_);
  ASSERT_EQ(ipv6address, setnetworkdefaultgatewayresponse.ipv6address_);
}

TEST_F(TestDeviceClient, SetNetworkInterfaces)
{
  const std::string& interfacetoken("token");
  const NetworkInterfaceSetConfiguration networkinterface(true,
                                                          NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF),
                                                          5,
                                                          IPv4NetworkInterfaceSetConfiguration(true,
                                                                                               { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) },
                                                                                               true),
                                                          IPv6NetworkInterfaceSetConfiguration(true,
                                                                                               true,
                                                                                               { PrefixedIPv6Address(std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), 5), PrefixedIPv6Address(std::string("FE80::0202:B3FF:FE1E:8329"), 6) },
                                                                                               IPV6DHCPCONFIGURATION_STATEFUL),
                                                          { Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x"))) });
  const bool rebootneeded = false;
  dummydeviceservice_->SetSetNetworkInterfaces(rebootneeded);

  device::SetNetworkInterfacesResponse setnetworkinterfacesresponse = client_->SetNetworkInterfacesFuture(interfacetoken, networkinterface).get();
  ASSERT_GE(setnetworkinterfacesresponse.latency_, 0);
  ASSERT_FALSE(setnetworkinterfacesresponse.Error());
  ASSERT_EQ(interfacetoken, setnetworkinterfacesresponse.interfacetoken_);
  ASSERT_EQ(networkinterface, setnetworkinterfacesresponse.networkinterface_);
  ASSERT_TRUE(setnetworkinterfacesresponse.rebootneeded_.is_initialized());
  ASSERT_EQ(rebootneeded, *setnetworkinterfacesresponse.rebootneeded_);
}

TEST_F(TestDeviceClient, SetNetworkProtocols)
{
  const std::vector<NetworkProtocol> networkprotocols =
  {
    NetworkProtocol(NETWORKPROTOCOLTYPE_HTTPS, true, { 5, 6}),
    NetworkProtocol(NETWORKPROTOCOLTYPE_RTSP, true, { 5, 6 })
  };

  device::SetNetworkProtocolsResponse setnetworkprotocolsresponse = client_->SetNetworkProtocolsFuture(networkprotocols).get();
  ASSERT_GE(setnetworkprotocolsresponse.latency_, 0);
  ASSERT_FALSE(setnetworkprotocolsresponse.Error());
  ASSERT_EQ(networkprotocols, setnetworkprotocolsresponse.networkprotocols_);
}

TEST_F(TestDeviceClient, SetNTP)
{
  const bool fromdhcp = true;
  const std::vector<NetworkHost> ntpmanual({
                                             NetworkHost(NETWORKHOSTTYPE_IPV4, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname1")),
                                             NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("6.6.6.6"), std::string("FE80:0000:0000:0010:0202:B3FF:FE1E:8329"), std::string("dnsname2"))
                                           });

  device::SetNTPResponse setntpresponse = client_->SetNTPFuture(fromdhcp, ntpmanual).get();
  ASSERT_GE(setntpresponse.latency_, 0);
  ASSERT_FALSE(setntpresponse.Error());
  ASSERT_EQ(fromdhcp, setntpresponse.fromdhcp_);
  ASSERT_EQ(ntpmanual, setntpresponse.ntpmanual_);
}

TEST_F(TestDeviceClient, SetRelayOutputSettings)
{
  const std::string relayoutputtoken("relayoutputtoken");
  RelayOutputSettings properties(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN);

  device::SetRelayOutputSettingsResponse setrelayoutputsettingsresponse = client_->SetRelayOutputSettingsFuture(relayoutputtoken, properties).get();
  ASSERT_GE(setrelayoutputsettingsresponse.latency_, 0);
  ASSERT_FALSE(setrelayoutputsettingsresponse.Error());
  ASSERT_EQ(relayoutputtoken, setrelayoutputsettingsresponse.relayoutputtoken_);
  ASSERT_EQ(properties, setrelayoutputsettingsresponse.properties_);
}

TEST_F(TestDeviceClient, SetRemoteDiscoveryMode)
{
  const DISCOVERYMODE remotediscoverymode = DISCOVERYMODE_NONDISCOVERABLE;

  device::SetRemoteDiscoveryModeResponse setremotediscoverymoderesponse = client_->SetRemoteDiscoveryModeFuture(remotediscoverymode).get();
  ASSERT_GE(setremotediscoverymoderesponse.latency_, 0);
  ASSERT_FALSE(setremotediscoverymoderesponse.Error());
  ASSERT_EQ(remotediscoverymode, setremotediscoverymoderesponse.remotediscoverymode_);
}

TEST_F(TestDeviceClient, SetSystemDateAndTime)
{
  const DATETIMETYPE datetimetype = DATETIME_MANUAL;
  const bool daylightsavings = true;
  const TimeZone timezone(std::string("tz"));
  const DateTime utcdatetime(Time(1, 1, 1), Date(1, 1, 1));

  device::SetSystemDateAndTimeResponse setsystemdateandtimeresponse = client_->SetSystemDateAndTimeFuture(datetimetype, daylightsavings, timezone, utcdatetime).get();
  ASSERT_GE(setsystemdateandtimeresponse.latency_, 0);
  ASSERT_FALSE(setsystemdateandtimeresponse.Error());
  ASSERT_EQ(datetimetype, setsystemdateandtimeresponse.datetimetype_);
  ASSERT_EQ(daylightsavings, setsystemdateandtimeresponse.daylightsavings_);
  ASSERT_TRUE(setsystemdateandtimeresponse.timezone_.is_initialized());
  ASSERT_EQ(timezone, *setsystemdateandtimeresponse.timezone_);
  ASSERT_TRUE(setsystemdateandtimeresponse.utcdatetime_.is_initialized());
  ASSERT_EQ(utcdatetime, *setsystemdateandtimeresponse.utcdatetime_);
}

TEST_F(TestDeviceClient, SetSystemFactoryDefault)
{
  const FACTORYDEFAULTTYPE factorydefault = FACTORYDEFAULTTYPE_SOFT;

  device::SetSystemFactoryDefaultResponse setsystemfactorydefaultresponse = client_->SetSystemFactoryDefaultFuture(factorydefault).get();
  ASSERT_GE(setsystemfactorydefaultresponse.latency_, 0);
  ASSERT_FALSE(setsystemfactorydefaultresponse.Error());
  ASSERT_EQ(factorydefault, setsystemfactorydefaultresponse.factorydefault_);
}

TEST_F(TestDeviceClient, SetUser)
{
  const std::vector<User> users =
  {
    User(std::string("username1"), std::string("password1"), USERLEVEL_ADMINISTRATOR),
    User(std::string("username2"), std::string("password2"), USERLEVEL_ANONYMOUS),
    User(std::string("username3"), std::string("password3"), USERLEVEL_EXTENDED),
    User(std::string("username4"), std::string("password4"), USERLEVEL_OPERATOR)
  };

  device::SetUserResponse setuserresponse = client_->SetUserFuture(users).get();
  ASSERT_GE(setuserresponse.latency_, 0);
  ASSERT_FALSE(setuserresponse.Error());
  ASSERT_EQ(users, setuserresponse.users_);
}

TEST_F(TestDeviceClient, SetZeroConfiguration)
{
  const std::string interfacetoken("interfacetoken");
  const bool enabled = true;

  device::SetZeroConfigurationResponse setzeroconfigurationresponse = client_->SetZeroConfigurationFuture(interfacetoken, enabled).get();
  ASSERT_GE(setzeroconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setzeroconfigurationresponse.Error());
  ASSERT_EQ(interfacetoken, setzeroconfigurationresponse.interfacetoken_);
  ASSERT_EQ(enabled, setzeroconfigurationresponse.enabled_);
}

TEST_F(TestDeviceClient, SystemReboot)
{
  const std::string message("reboot");
  dummydeviceservice_->SetSystemReboot(message);

  device::SystemRebootResponse systemrebootresponse = client_->SystemRebootFuture().get();
  ASSERT_GE(systemrebootresponse.latency_, 0);
  ASSERT_FALSE(systemrebootresponse.Error());
  ASSERT_TRUE(systemrebootresponse.rebootmessage_.is_initialized());
  ASSERT_EQ(message, *systemrebootresponse.rebootmessage_);
}

}

}
