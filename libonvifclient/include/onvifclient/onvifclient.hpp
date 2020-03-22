// clientsdk.hpp
//

#ifndef IDQL5JX6FGP4TB09H0L9U47SPSD4V18IGQ
#define IDQL5JX6FGP4TB09H0L9U47SPSD4V18IGQ

///// Includes /////

#include <boost/asio.hpp>
#include <memory>
#include <utility/utility.hpp>

#include "analyticsclient.hpp"
#include "deviceclient.hpp"
#include "deviceioclient.hpp"
#include "eventclient.hpp"
#include "imagingclient.hpp"
#include "mediaclient.hpp"
#include "ptzclient.hpp"
#include "receiverclient.hpp"
#include "recordingclient.hpp"
#include "replayclient.hpp"
#include "searchclient.hpp"

///// Namespaces /////

namespace onvif
{

///// Prototypes /////

int Init();
int Destroy();

///// Classes /////

template<class Client>
class ClientThread
{
 public:

  ClientThread(const boost::shared_ptr<std::recursive_mutex>& mutex) :
    client_(boost::make_shared<Client>(mutex)),
    running_(false)
  {

  }

  virtual ~ClientThread()
  {
    Destroy();

  }

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password)
  {
    return Init(proxyparams, address, username, password, 0, false, false);
  }

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, unsigned int maxconcurrentrequests, bool forcehttpauthentication, const bool forbidreuse)
  {
    Destroy();

    const int ret = client_->Init(proxyparams, address, username, password, maxconcurrentrequests, forcehttpauthentication, forbidreuse);
    if (ret)
    {

      return ret;
    }

    running_ = true;
    thread_ = std::thread(&ClientThread::Run, this);

    return 0;
  }

  virtual void Destroy()
  {
    if (thread_.joinable())
    {
      running_ = false;
      thread_.join();
    }
    client_->Destroy();
  }

  boost::shared_ptr<Client>& operator->() { return client_; }

private:

  void Run()
  {
    while (running_)
    {
      client_->Update();
      utility::Sleep(std::chrono::milliseconds(10));
    }
  }

  boost::shared_ptr<Client> client_;

  std::thread thread_;
  bool running_;

};

}

#endif