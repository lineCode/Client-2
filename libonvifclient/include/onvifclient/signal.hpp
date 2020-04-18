// signal.hpp
//

#ifndef ID0T8B0L8GRAME9TBJMSO6EUS7PHZ4KAM6
#define ID0T8B0L8GRAME9TBJMSO6EUS7PHZ4KAM6

///// Includes /////

#include <boost/thread/future.hpp>
#include <boost/signals2.hpp>
#include <curl/curl.h>
#include <functional>
#include <map>
#include <mutex>
#include <tuple>
#include <utility>

#include "connection.hpp"
#include "connectionblock.hpp"
#include "scopedconnection.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

template<typename Operation, typename Client, typename Response, typename... Args>
class Signal
{
 public:

  template<typename Args1, typename... Params>
  Response GetResponse(Args1 args, Params... params)
  {
    static constexpr auto size = std::tuple_size<Args1>::value;
    return GetResponse(std::make_index_sequence<size>(), args, params...);
  }

  template<size_t... I, typename Args1, typename... Params>
  Response GetResponse(std::index_sequence<I...>, Args1 args, Params... params)
  {
    return Response(std::get<I>(args)..., params...);
  }

  Signal(const boost::shared_ptr<Client>& client, const Operation operation, const bool authentication, const std::string& action, const bool event) :
    client_(client),
    operation_(operation),
    authentication_(authentication),
    action_(action),
    event_(event)
  {

  }

  ~Signal()
  {
    Destroy();

  }

  void Destroy()
  {
    for (auto& promise : promises_)
    {
      const Response response = GetResponse(std::tuple_cat(std::make_tuple(client_, boost::asio::ip::address(), 0LL, std::string("Cancelled")), GetArgs(promise.first)));
      promise.second.set_value(response);
      signal_(response);
    }
    promises_.clear();
    
    for (auto& callback : callbacks_)
    {
      const Response response = GetResponse(std::tuple_cat(std::make_tuple(client_, boost::asio::ip::address(), 0LL, std::string("Cancelled")), GetArgs(callback.first)));
      callback.second.first->mutex_.lock();
      if (callback.second.first->connected_)
      {
        callback.second.second(response);
        callback.second.first->connected_ = false;
      }
      callback.second.first->mutex_.unlock();
      signal_(response);
    }
    callbacks_.clear();
    
    signal_.disconnect_all_slots();
    
    args_.clear();
  }

  void Create(const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, Args... args)
  {
    CURL* handle = client_->SendCommand(operation_, authentication_, event_, action_, referenceparameters, to, body, mtomdata);
    if (handle == nullptr)
    {
      signal_(GetResponse(std::make_tuple(client_, boost::asio::ip::address(), 0, std::string("Client::SendCommand() failed")), args...));

    }
    else
    {
      args_[handle] = std::make_tuple(args...);

    }
  }

  void Create(const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, Args... args)
  {
    Create(std::vector<Element>(), std::string(), body, mtomdata, args...);
  }

  void Create(const std::string& body, Args... args)
  {
    Create(std::vector<Element>(), std::string(), body, std::map<std::string, std::vector<char> >(), std::forward<Args>(args)...);
  }

  boost::unique_future<Response> CreateFuture(const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, Args... args)
  {
    boost::promise<Response> promise;
    boost::unique_future<Response> future = promise.get_future();
    CURL* handle = client_->SendCommand(operation_, authentication_, event_, action_, referenceparameters, to, body, mtomdata);
    if (handle == nullptr)
    {
      promise.set_value(Response(client_, boost::asio::ip::address(), 0, std::string("Client::SendCommand() failed"), args...));

    }
    else
    {
      promises_.emplace(std::make_pair(handle, std::move(promise)));
      args_[handle] = std::make_tuple(args...);
    }
    return future;
  }

  boost::unique_future<Response> CreateFuture(const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, Args... args)
  {
    return CreateFuture(std::vector<Element>(), std::string(), body, mtomdata, args...);
  }

  boost::unique_future<Response> CreateFuture(const std::string& body, Args... args)
  {
    return CreateFuture(std::vector<Element>(), std::string(), body, std::map<std::string, std::vector<char> >(), std::forward<Args>(args)...);
  }
  
  Connection CreateCallback(const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, boost::function<void(const Response&)> callback, Args... args)
  {
    if (!callback)
    {
    
      return Connection();
    }
    
    CURL* handle = client_->SendCommand(operation_, authentication_, event_, action_, referenceparameters, to, body, mtomdata);
    if (handle == nullptr)
    {
      callback(Response(client_, boost::asio::ip::address(), 0, std::string("Client::SendCommand() failed"), args...));
      return Connection();
    }
    
    std::lock_guard<std::recursive_mutex> lock(*client_->GetMutex());
    boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
    callbacks_[handle] = std::make_pair(connectionblock, callback);
    args_[handle] = std::make_tuple(args...);
    return Connection(connectionblock);
  }

  Connection CreateCallback(const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata, boost::function<void(const Response&)> callback, Args... args)
  {
    return CreateCallback(std::vector<Element>(), std::string(), body, mtomdata, callback, args...);
  }

  Connection CreateCallback(const std::string& body, boost::function<void(const Response&)> callback, Args... args)
  {
    return CreateCallback(std::vector<Element>(), std::string(), body, std::map<std::string, std::vector<char> >(), callback, std::forward<Args>(args)...);
  }
  
  template<typename... Parameters>
  void Emit(CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, Parameters... parameters)
  {
    const Response response = GetResponse(std::tuple_cat(std::make_tuple(client_->shared_from_this(), localendpoint, latency, message), GetArgs(handle)), parameters...);

    // Clear the parameters
    args_.erase(args_.find(handle));

    signal_(response);

    // Futures
    typename std::map< CURL*, boost::promise< Response > >::iterator promise = promises_.find(handle);
    if (promise != promises_.end())
    {
      promise->second.set_value(response);
      promises_.erase(promise);
    }
    
    // Callbacks
    std::lock_guard<std::recursive_mutex> lock(*client_->GetMutex());
    typename std::map< CURL*, std::pair< boost::shared_ptr<ConnectionBlock>, boost::function<void(const Response&)> > >::iterator callback = callbacks_.find(handle);
    if (callback != callbacks_.end())
    {
      // Check whether the receiver is still connected
      callback->second.first->mutex_.lock();
      if (callback->second.first->connected_)
      {
        callback->second.second(response);
        callback->second.first->connected_ = false;
      }
      callback->second.first->mutex_.unlock();
      callbacks_.erase(callback);
    }
  }

  boost::signals2::signal<void(const Response&)>& GetSignal() { return signal_; }

 private:

  std::tuple<Args...> GetArgs(CURL* handle) const
  {
    return args_.find(handle)->second;
  }
  
  boost::shared_ptr<Client> client_;
  const Operation operation_;
  const bool authentication_;
  const std::string action_;
  const bool event_; // Flag to indicate whether it is an event style request(including some extra headers)

  boost::signals2::signal<void(const Response&)> signal_;
  std::map< CURL*, boost::promise< Response > > promises_;
  std::map< CURL*, std::pair< boost::shared_ptr<ConnectionBlock>, boost::function<void(const Response&)> > > callbacks_;

  std::map< CURL*, std::tuple<Args...> > args_; // Represents the original arguments passed through to call the wsdl operation

};

}

#endif
