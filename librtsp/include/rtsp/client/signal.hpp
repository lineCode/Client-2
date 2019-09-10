// signal.hpp
//

#ifndef IDKYQWLPV6APRODUS3JPUHNZ6GS9HCE8B5
#define IDKYQWLPV6APRODUS3JPUHNZ6GS9HCE8B5

///// Includes /////

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <future>
#include <map>
#include <mutex>
#include <stdint.h>
#include <utility>

#include "client.hpp"
#include "connection.hpp"
#include "connectionblock.hpp"
#include "rtsp/rtsprequest.hpp"
#include "scopedconnection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace client
{

///// Classes /////

template<class Client, class ResponseType>
class Signal
{
 public:

  typedef boost::function<void(const boost::system::error_code&, const ResponseType&)> CallbackType;

  Signal(Client* client) :
    client_(client)
  {

  }

  ~Signal()
  {
    Destroy();

  }

  void Destroy()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    for (auto& promise : promises_)
    {
      promise.second.set_exception(std::make_exception_ptr(boost::system::system_error(boost::system::errc::make_error_code(boost::system::errc::connection_aborted))));

    }
    promises_.clear();

    while (callbacks_.size())
    {
      auto i = callbacks_.begin();
      auto callback = std::move(*i);
      callbacks_.erase(i);

      // Check whether the receiver is still connected
      callback.second.first->mutex_.lock();
      if (callback.second.first->connected_)
      {
        callback.second.second(boost::system::errc::make_error_code(boost::system::errc::connection_aborted), ResponseType(0));
        callback.second.first->connected_ = false;
      }
      callback.second.first->mutex_.unlock();
    }
  }
  
  std::future<ResponseType> CreateFuture(const std::pair<uint64_t, const RtspRequest>& request)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    std::promise<ResponseType> promise;
    if (client_->Request(request))
    {
      promise.set_exception(std::make_exception_ptr(boost::system::system_error(boost::system::errc::make_error_code(boost::system::errc::protocol_error))));
      return promise.get_future();
    }
    else
    {
      auto i = promises_.emplace(std::make_pair(request.first, std::move(promise)));
      return i.first->second.get_future();
    }
  }

  Connection CreateCallback(const std::pair<uint64_t, const RtspRequest>& request, CallbackType callback)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if (client_->Request(request))
    {
      callback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), ResponseType(0));
      return Connection();
    }
    else
    {
      boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
      callbacks_[request.first] = std::make_pair(connectionblock, callback);
      return Connection(connectionblock);
    }
  }

  template<typename... Parameters>
  void Emit(uint64_t id, uint64_t latency, const boost::system::error_code& error, Parameters... parameters)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    // Future
    auto promise = promises_.find(id);
    if (promise != promises_.end())
    {
      if (error)
      {
        promise->second.set_exception(std::make_exception_ptr(boost::system::system_error(error)));

      }
      else
      {
        promise->second.set_value(ResponseType(latency, parameters...));

      }
      promises_.erase(promise);
    }

    // Callback
    typename std::map< uint64_t, std::pair< boost::shared_ptr<ConnectionBlock>, CallbackType> >::iterator callback = callbacks_.find(id);
    if (callback != callbacks_.end())
    {
      // Check whether the receiver is still connected
      callback->second.first->mutex_.lock();
      if (callback->second.first->connected_)
      {
        callback->second.second(error, ResponseType(latency, parameters...));
        callback->second.first->connected_ = false;
      }
      callback->second.first->mutex_.unlock();
      callbacks_.erase(callback);
    }
  }

 private:

  Client* client_;

  std::recursive_mutex mutex_;

  std::map< uint64_t, std::promise<ResponseType> > promises_;
  std::map< uint64_t, std::pair< boost::shared_ptr<ConnectionBlock>, CallbackType> > callbacks_;

};

}

}

#endif
