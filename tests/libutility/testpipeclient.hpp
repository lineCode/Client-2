// testpipeclient.cpp
//

///// Includes /////

#include <atomic>
#include <functional>
#include <thread>
#include <utility/pipeclient.hpp>
#include <vector>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Methods /////

class TestPipeClient : public PipeClient
{
 public:

  TestPipeClient();
  ~TestPipeClient();

  virtual int Init(const std::string& uuid) override;
  virtual void Destroy() override;

  virtual void Message(const std::string& message) override;

  inline void SetMessageCallback(const std::function<void(const std::string&)>& messagecallback) { messagecallback_ = messagecallback; }

 private:

  std::thread thread_;
  std::atomic<bool> running_;

  std::function<void(const std::string&)> messagecallback_;

};

}

}
