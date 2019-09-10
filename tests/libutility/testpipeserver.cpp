// testpipeserver.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "testpipeserver.hpp"

#include <gtest/gtest.h>
#include <utility/utility.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Methods /////

TestPipeServer::TestPipeServer() :
  running_(false)
{

}

TestPipeServer::~TestPipeServer()
{
  Destroy();

}

int TestPipeServer::Init(const std::string& uuid)
{
  Destroy();

  const int ret = PipeServer::Init(uuid);
  if (ret)
  {

    return ret;
  }

  running_ = true;
  thread_ = std::thread([this]()
  {
    while (running_)
    {
      ASSERT_FALSE(Update());
      utility::Sleep(std::chrono::milliseconds(1));
    }
  });

  return 0;
}

void TestPipeServer::Destroy()
{
  if (thread_.joinable())
  {
    running_ = false;
    thread_.join();
  }

  PipeServer::Destroy();
}

void TestPipeServer::PipeConnected()
{
  if (pipeconnectedcallback_)
  {
    pipeconnectedcallback_();

  }
}

void TestPipeServer::Message(const std::string& message)
{
  if (messagecallback_)
  {
    messagecallback_(message);

  }
}

}

}
