// pipeserver.cpp
//

///// Includes /////

#include <atomic>
#include <functional>
#include <thread>
#include <utility/pipeserver.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Classes /////

class TestPipeServer : public PipeServer
{
 public:

  TestPipeServer();
  ~TestPipeServer();

  virtual int Init(const std::string& uuid) override;
  virtual void Destroy() override;

  virtual void PipeConnected() override;
  virtual void Message(const std::string& message) override;

  inline void SetPipeConnectedCallback(const std::function<void()>& pipeconnectedcallback) { pipeconnectedcallback_ = pipeconnectedcallback; }

  inline void SetMessageCallback(const std::function<void(const std::string&)>& messagecallback) { messagecallback_ = messagecallback; }

 private:

  std::thread thread_;
  std::atomic<bool> running_;

  std::function<void()> pipeconnectedcallback_;

  std::function<void(const std::string&)> messagecallback_;

};

}

}
