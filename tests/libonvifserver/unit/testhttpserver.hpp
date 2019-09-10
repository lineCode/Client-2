// testhttpserver.hpp
//

#ifndef IDTXTET0TJTKXUH9UDKYWHP803NURLMRBX
#define IDTXTET0TJTKXUH9UDKYWHP803NURLMRBX

///// Includes /////

#include <curl/curl.h>
#include <gtest/gtest.h>
#include <onvifserver/httpserver.hpp>
#include <thread>

#include "dummyservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Classes /////

class TestHttpServer : public testing::Test
{
 public:

  TestHttpServer();
  virtual ~TestHttpServer();

 protected:

  static void SetUpTestCase();
  static void TearDownTestCase();

  virtual void SetUp();
  virtual void TearDown();

  HttpServer httpserver_;
  std::unique_ptr<DummyService> dummyservice_;

 private:

  void Run();

  std::thread httpserverthread_;
  bool run_;

};

}

}

}

#endif
