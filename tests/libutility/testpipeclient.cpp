// testpipeclient.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "testpipeclient.hpp"

#include <gtest/gtest.h>
#include <utility/utility.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Methods /////

TestPipeClient::TestPipeClient() :
  running_(false)
{

}

TestPipeClient::~TestPipeClient()
{
  Destroy();

}

int TestPipeClient::Init(const std::string& uuid)
{
  Destroy();

  const int ret = PipeClient::Init(uuid);
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

void TestPipeClient::Destroy()
{
  if (thread_.joinable())
  {
    running_ = false;
    thread_.join();
  }

  PipeClient::Destroy();
}

void TestPipeClient::Message(const std::string& message)
{
  if (messagecallback_)
  {
    messagecallback_(message);

  }
}

}

}
