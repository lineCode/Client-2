// testhttpserver.cpp
//

///// Includes /////

#include "testhttpserver.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 7000;
const std::string URI = "/test";

///// Functions /////

static size_t EmptyCallback(void* data, size_t size, size_t nmemb, void* string)
{
  static_cast<std::string*>(string)->append(static_cast<char*>(data));
  return (size*nmemb);
}

///// Methods /////

TestHttpServer::TestHttpServer() :
  httpserver_("realm@host.com"),
  dummyservice_(std::make_unique<DummyService>()),
  run_(false)
{

}

TestHttpServer::~TestHttpServer()
{

}

void TestHttpServer::SetUpTestCase()
{

}

void TestHttpServer::TearDownTestCase()
{

}

void TestHttpServer::SetUp()
{
  ASSERT_FALSE(curl_global_init(CURL_GLOBAL_ALL));
  ASSERT_FALSE(httpserver_.Init("0.0.0.0", PORT));
  httpserver_.AddServer(URI, dummyservice_.get());
  httpserverthread_ = std::thread(std::bind(&TestHttpServer::Run, this));
}

void TestHttpServer::TearDown()
{
  run_ = false;
  httpserverthread_.join();

  curl_global_cleanup();
  ASSERT_FALSE(httpserver_.Destroy());
}

void TestHttpServer::Run()
{
  run_ = true;
  while (run_)
  {
    ASSERT_FALSE(httpserver_.Update());
    utility::Sleep(std::chrono::milliseconds(10));
  }
}

///// Tests /////

TEST_F(TestHttpServer, Get)
{
  const std::string response = utility::GenerateRandomString(20);
  dummyservice_->SetRestricted(false);
  dummyservice_->SetResponse(response);

  CURL* handle = curl_easy_init();
  ASSERT_TRUE(handle);
  curl_easy_setopt(handle, CURLOPT_URL, ("http://127.0.0.1:" + std::to_string(PORT) + URI).c_str());
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, EmptyCallback));
  std::string text;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  ASSERT_EQ(response, text);
  curl_easy_cleanup(handle);
}

TEST_F(TestHttpServer, Digest)
{
  const std::string username("username");
  const std::string password("password");

  const std::string response = utility::GenerateRandomString(20);
  dummyservice_->SetRestricted(true);
  dummyservice_->SetResponse(response);
  dummyservice_->SetUsername(username);
  dummyservice_->SetPassword(password);

  CURL* handle = curl_easy_init();
  ASSERT_TRUE(handle);
  curl_easy_setopt(handle, CURLOPT_URL, ("http://127.0.0.1:" + std::to_string(PORT) + URI).c_str());
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, EmptyCallback));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str()));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_PASSWORD, password.c_str()));
  std::string text;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  ASSERT_EQ(response, text);
  curl_easy_cleanup(handle);
}

TEST_F(TestHttpServer, DigestBadUsername)
{
  const std::string username("username");
  const std::string password("password");

  const std::string response = utility::GenerateRandomString(20);
  dummyservice_->SetRestricted(true);
  dummyservice_->SetResponse(response);
  dummyservice_->SetUsername(username);
  dummyservice_->SetPassword(password);

  CURL* handle = curl_easy_init();
  ASSERT_TRUE(handle);
  curl_easy_setopt(handle, CURLOPT_URL, ("http://127.0.0.1:" + std::to_string(PORT) + URI).c_str());
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, EmptyCallback));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_USERNAME, "badusername"));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_PASSWORD, password.c_str()));
  std::string text;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  long responsecode = 0;
  curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &responsecode);
  ASSERT_EQ(401, responsecode);
  curl_easy_cleanup(handle);
}

TEST_F(TestHttpServer, DigestBadPassword)
{
  const std::string username("username");
  const std::string password("password");

  const std::string response = utility::GenerateRandomString(20);
  dummyservice_->SetRestricted(true);
  dummyservice_->SetResponse(response);
  dummyservice_->SetUsername(username);
  dummyservice_->SetPassword(password);

  CURL* handle = curl_easy_init();
  ASSERT_TRUE(handle);
  curl_easy_setopt(handle, CURLOPT_URL, ("http://127.0.0.1:" + std::to_string(PORT) + URI).c_str());
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, EmptyCallback));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str()));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_PASSWORD, "badpassword"));
  std::string text;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  long responsecode = 0;
  curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &responsecode);
  ASSERT_EQ(401, responsecode);
  curl_easy_cleanup(handle);
}

/*TEST_F(TestHttpServer, DigestStale) // This test fails with the current versino of libcurl... It works with chrome though...
{
  httpserver_.SetNonceTimeout(std::chrono::seconds(1));
  httpserver_.SetPrevNonceTimeout(std::chrono::seconds(20));

  const std::string username("username");
  const std::string password("password");

  const std::string response = utility::GenerateRandomString(20);
  dummyserver_->SetRestricted(true);
  dummyserver_->SetResponse(response);
  dummyserver_->SetUsername(username);
  dummyserver_->SetPassword(password);

  CURL* handle = curl_easy_init();
  ASSERT_TRUE(handle);

  curl_easy_setopt(handle, CURLOPT_URL, ("http://127.0.0.1:" + std::to_string(PORT) + URI).c_str());
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, EmptyCallback));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str()));
  ASSERT_FALSE(curl_easy_setopt(handle, CURLOPT_PASSWORD, password.c_str()));
  ASSERT_TRUE(handle);

  std::string text;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  ASSERT_EQ(response, text.get());

  std::this_thread::sleep_for(std::chrono::seconds(5));

  text.clear();
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &text);
  ASSERT_EQ(CURLE_OK, curl_easy_perform(handle));
  ASSERT_EQ(response, text.get());

  curl_easy_cleanup(handle);
}*/

}

}

}
