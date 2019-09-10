// testpipe.cpp
//

///// Includes /////

#include <array>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <gtest/gtest.h>
#include <random>
#include <utility/utility.hpp>

#include "testpipeclient.hpp"
#include "testpipeserver.hpp"

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Globals /////

const size_t NUM_PIPES = 2;

///// Classes /////

TEST(Pipe, Test)
{
  const int MESSAGE_COUNT = 200;

  // Initialise server
  const std::string uuid = boost::uuids::to_string(boost::uuids::random_generator()());
  TestPipeServer pipeserver;
  ASSERT_FALSE(pipeserver.Init(uuid));

  for (int i = 0; i < 5; ++i)
  {
    // Connect clients
    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    std::atomic<int> count = 0;
    pipeserver.SetPipeConnectedCallback([&promise, &count]()
    {
      ++count;
      if (count >= NUM_PIPES)
      {
        promise.set_value();

      }
    });
    std::array<TestPipeClient, NUM_PIPES> pipeclients;
    for (TestPipeClient& pipeclient : pipeclients)
    {
      ASSERT_FALSE(pipeclient.Init(uuid));

    }
    future.get();
    pipeserver.SetPipeConnectedCallback(std::function<void()>());

    // Send data from server and collect in the clients
    std::array<std::promise<void>, NUM_PIPES> pipepromises;
    std::array<std::future<void>, NUM_PIPES> pipefutures;
    std::array<std::vector<std::string>, NUM_PIPES> pipemessages;
    for (int j = 0; j < pipeclients.size(); ++j)
    {
      TestPipeClient& pipeclient = pipeclients[j];
      pipefutures[j] = pipepromises[j].get_future();
      pipemessages[j].reserve(MESSAGE_COUNT);
      pipeclient.SetMessageCallback([&pipepromises, &pipemessages, j, count = MESSAGE_COUNT](const std::string& message) mutable
      {
        pipemessages[j].push_back(message);
        --count;
        if (count == 0)
        {
          pipepromises[j].set_value();

        }
      });
    }

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<unsigned int> dist(1, 128);

    std::vector<std::string> sentmessages;
    sentmessages.reserve(MESSAGE_COUNT);
    for (int j = 0; j < MESSAGE_COUNT; ++j)
    {
      const std::string text = utility::GenerateRandomString(dist(gen));
      ASSERT_FALSE(pipeserver.SendText(text));
      sentmessages.push_back(text);
    }

    for (std::future<void>& pipefuture : pipefutures)
    {
      pipefuture.get();

    }
    for (TestPipeClient& pipeclient : pipeclients)
    {
      pipeclient.SetMessageCallback(std::function<void(const std::string&)>());

    }

    for (int j = 0; j < pipemessages.size(); ++j)
    {
      ASSERT_EQ(pipemessages[j], sentmessages);

    }

    // Send data from the client to the server
    promise = std::promise<void>();
    future = promise.get_future();
    std::vector<std::string> messages;
    messages.reserve(NUM_PIPES * MESSAGE_COUNT);
    pipeserver.SetMessageCallback([&promise, &messages, count = NUM_PIPES * MESSAGE_COUNT](const std::string& message) mutable
    {
      messages.push_back(message);
      --count;
      if (count == 0)
      {
        promise.set_value();

      }
    });

    sentmessages.clear();
    for (TestPipeClient& pipeclient : pipeclients)
    {
      for (int j = 0; j < MESSAGE_COUNT; ++j)
      {
        const std::string text = utility::GenerateRandomString(dist(gen));
        ASSERT_FALSE(pipeclient.SendText(text));
        sentmessages.push_back(text);
      }
    }

    future.get();
    pipeserver.SetMessageCallback(std::function<void(const std::string&)>());

    // Clear up...
    for (TestPipeClient& pipeclient : pipeclients)
    {
      pipeclient.Destroy();

    }
  }

  pipeserver.Destroy();
}

}

}
