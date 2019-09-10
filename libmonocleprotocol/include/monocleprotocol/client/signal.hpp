// signal.hpp
//

#ifndef IDF69P3OAV5U123MFJAIZ6ETGJT0STS5UA
#define IDF69P3OAV5U123MFJAIZ6ETGJT0STS5UA

///// Includes /////

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/future.hpp>
#include <memory>
#include <vector>

#include "monocleprotocol/client/connection.hpp"

///// Namespaces /////

namespace monocle
{

namespace client
{

///// Declarations /////

class Client;

///// Structures /////

template<class T>
struct REQUEST
{
  REQUEST(boost::asio::io_service& io, const uint16_t sequence, std::chrono::steady_clock::time_point time, const int32_t timeout) :
    sequence_(sequence),
    time_(time),
    timeout_(io, boost::posix_time::time_duration(0, 0, timeout))
  {

  }

  REQUEST(boost::asio::io_service& io, const uint16_t sequence, std::chrono::steady_clock::time_point time, const int32_t timeout, const std::function<void(const std::chrono::steady_clock::duration, const T&)>& callback, boost::shared_ptr<ConnectionBlock>& connectionblock) :
    sequence_(sequence),
    time_(time),
    timeout_(io, boost::posix_time::time_duration(0, 0, timeout)),
    callback_(callback),
    connectionblock_(connectionblock)
  {

  }

  uint16_t sequence_;
  std::chrono::steady_clock::time_point time_;
  boost::asio::deadline_timer timeout_;
  boost::promise<T> promise_;
  std::function<void(const std::chrono::steady_clock::duration, const T&)> callback_;
  boost::shared_ptr<ConnectionBlock> connectionblock_;

};

///// Classes /////

template<class C, class T>
class Signal
{
 public:

  Signal(const int32_t timeout, C* client) :
    timeout_(timeout),
    client_(client)
  {

  }

  ~Signal()
  {
    Destroy();

  }

  void Destroy()
  {
    std::lock_guard<std::recursive_mutex> lock(client_->mutex_);
    while (requests_.size())
    {
      auto i = requests_.end() - 1;
      auto request = std::move(*i);
      requests_.erase(i);

      T response(Error(ErrorCode::OperationCancelled, "Operation cancelled"));
      request->promise_.set_value(response);
      if (request->callback_)
      {
        // Check whether the receiver is still connected
        request->connectionblock_->mutex_.lock();
        if (request->connectionblock_->connected_)
        {
          request->callback_(std::chrono::steady_clock::now() - request->time_, response);
          request->connectionblock_->connected_ = false;
        }
        request->connectionblock_->mutex_.unlock();
      }
    }
  }

  boost::unique_future<T> CreateFuture(const uint16_t sequence)
  {
    requests_.emplace_back(std::make_unique< REQUEST<T> >(client_->io_, sequence, std::chrono::steady_clock::now(), timeout_));
    requests_.back()->timeout_.async_wait(boost::bind(&Signal::Timeout, this, boost::asio::placeholders::error, sequence));
    return requests_.back()->promise_.get_future();
  }

  Connection CreateCallback(const uint16_t sequence, const std::function<void(const std::chrono::steady_clock::duration, const T&)>& callback)
  {
    boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
    requests_.emplace_back(std::make_unique< REQUEST<T> >(client_->io_, sequence, std::chrono::steady_clock::now(), timeout_, callback, connectionblock));
    requests_.back()->timeout_.async_wait(boost::bind(&Signal::Timeout, this, boost::asio::placeholders::error, sequence));
    return Connection(connectionblock);
  }

  void Response(const uint16_t sequence, const T& response)
  {
    auto i = std::find_if(requests_.begin(), requests_.end(), [sequence](const std::unique_ptr< REQUEST<T> >& request) { return (request->sequence_ == sequence); });
    if (i == requests_.end())
    {

      return;
    }
    auto request = std::move(*i);
    requests_.erase(i);
    request->promise_.set_value(response);
    if (request->callback_)
    {
      // Check whether the receiver is still connected
      request->connectionblock_->mutex_.lock();
      if (request->connectionblock_->connected_)
      {
        request->callback_(std::chrono::steady_clock::now() - request->time_, response);
        request->connectionblock_->connected_ = false;
      }
      request->connectionblock_->mutex_.unlock();
    }
  }
  
 private:

  void Timeout(const boost::system::error_code& err, const uint16_t sequence)
  {
    std::lock_guard<std::recursive_mutex> lock(client_->mutex_);
    if (err)
    {

      return;
    }
    Response(sequence, T(Error(ErrorCode::OperationTimeout, "Operation timeout")));
    client_->Destroy();
  }

  const int timeout_;

  C* client_;

  std::vector< std::unique_ptr< REQUEST<T> > > requests_;
 
};

}

}

#endif
