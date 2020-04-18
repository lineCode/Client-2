// testclient.hpp
//

#ifndef IDY6XNOH4BEMEE6W5VM1MZ17XMRWDYY4ST
#define IDY6XNOH4BEMEE6W5VM1MZ17XMRWDYY4ST

///// Includes /////

#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>
#include <onvifserver/onvifserver.hpp>
#include <thread>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const int WAITSECONDS = 1; // Time amount of time in seconds to wait for a response from the device before failing

///// Classes /////

template<class T>
class TestClient : public testing::Test
{
 public:

  TestClient() :
    httpserver_("realm@host.com"),
    running_(false)
  {
    
  }

  virtual ~TestClient()
  {
    
  }

  void Init(unsigned short port, const std::string& uri)
  {
    ASSERT_FALSE(running_);
    running_ = true;
  
    // Initialise the http server
    ASSERT_FALSE(server::Init());
    ASSERT_FALSE(httpserver_.Init("0.0.0.0", port));

    // Initialise the client
    ASSERT_FALSE(onvif::Init());
    client_ = boost::make_shared<T>(boost::make_shared<std::recursive_mutex>());
    ASSERT_FALSE(client_->Init(sock::ProxyParams(), std::string("http://localhost:") + std::to_string(port) + uri, std::string(), std::string(), 0, false, false));
    thread_ = std::thread(boost::bind(&TestClient::Run, this));
  }

  void Destroy()
  {
    ASSERT_TRUE(running_);
    running_ = false;
    ASSERT_TRUE(thread_.joinable());
    thread_.join();
    ASSERT_FALSE(onvif::Destroy());

    // Destroy the http server
    ASSERT_FALSE(httpserver_.Destroy());
    ASSERT_FALSE(server::Destroy());
  }

  void Run()
  {
    while(running_)
    {
      utility::Sleep(std::chrono::milliseconds(10));
      ASSERT_FALSE(httpserver_.Update());
      ASSERT_FALSE(client_->Update());
    }
  }

 protected:
    
  server::HttpServer httpserver_;
  boost::shared_ptr<T> client_;

 private:

  std::thread thread_;
  bool running_;

};

}

}

#endif
