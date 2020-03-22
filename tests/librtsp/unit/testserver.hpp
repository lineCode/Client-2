// testserver.hpp
//

#ifndef ID0JBLEWNTE53SDZZLDKENLH2WEAEFAH79
#define ID0JBLEWNTE53SDZZLDKENLH2WEAEFAH79

///// Includes /////

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <mutex>
#include <rtsp/client/client.hpp>
#include <gtest/gtest.h>
#include <rtsp/server/server.hpp>
#include <thread>

#include "rtspservertest.hpp"
#include "streamtest.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Classes /////

class TestServer : public testing::Test
{
 public:

  TestServer();
  virtual ~TestServer();

 protected:

  static void SetUpTestCase();
  static void TearDownTestCase();

  virtual void SetUp();
  virtual void TearDown();

  std::recursive_mutex mutex_;

  static const std::string url_;

  static utility::IoServicePool ioservicepool_;

  static boost::shared_ptr< Client<TestServer> > client_;
  static boost::shared_ptr<StreamTest> stream_;
  static boost::shared_ptr<Server> server_;

};

}

}

}

#endif
