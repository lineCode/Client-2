// testwsdiscoverserver.cpp
//

///// Includes /////

#include "testwsdiscoverserver.hpp"

#include <boost/make_shared.hpp>
#include <future>
#include <utility/utility.hpp>
#include <memory>
#include <mutex>
#include <wsdiscover/wsdiscoverclient.hpp>

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 3703;
const int WAITSECONDS = 3; // Time amount of time in seconds to wait for a response from the device before failing
const std::vector<std::string> SCOPES = { std::string("onvif://www.onvif.org/type/type"), std::string("onvif://www.onvif.org/location/location"), std::string("onvif://www.onvif.org/hardware/hardware"), std::string("onvif://www.onvif.org/name/name") };
const std::vector<std::string> XADDRS = { "http://127.0.0.1/onvif/deviceclient_service", "http://127.0.0.1/onvif/deviceclient_service" };

///// Methods /////

TestWsDiscoverServer::TestWsDiscoverServer() :
  ioservicepool_(4, 4, []() {}, []() {})
{
  discoverserver_ = boost::make_shared<WsDiscoverServer>(ioservicepool_.GetIoService());

}

TestWsDiscoverServer::~TestWsDiscoverServer()
{
  ioservicepool_.Destroy();

}

void TestWsDiscoverServer::SetUp()
{
  ASSERT_FALSE(discoverserver_->Init(boost::asio::ip::address::from_string("0.0.0.0"), PORT, true, boost::uuids::random_generator()(), 0, SCOPES, XADDRS));
}

void TestWsDiscoverServer::TearDown()
{
  ASSERT_FALSE(discoverserver_->Destroy());
}

///// Tests /////

TEST_F(TestWsDiscoverServer, Discover)
{
  boost::shared_ptr<WsDiscoverClient> client = boost::make_shared<WsDiscoverClient>(ioservicepool_.GetIoService(), PORT);
  ASSERT_EQ(0, client->Init());

  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client->hello_.connect([&promise](const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
  {
    std::for_each(SCOPES.begin(), SCOPES.end(), [&scopes](const std::string& scope){ ASSERT_TRUE(utility::Contains(scopes, scope)); });
    ASSERT_TRUE(utility::Contains(addresses, XADDRS));
    ASSERT_NO_THROW(promise.set_value());
  });
  
  ASSERT_FALSE(client->Broadcast());
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(500)));
  ASSERT_NO_THROW(future.get());
  ASSERT_FALSE(client->Destroy());
}

}

}

}
