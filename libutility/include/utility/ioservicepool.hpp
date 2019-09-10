// ioservicepool.hpp
//

#ifndef IDENFCXU123912731931230923WYBM77DZ
#define IDENFCXU123912731931230923WYBM77DZ

///// Includes /////

#include <boost/asio/io_service.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

///// Namespaces /////

namespace utility
{

///// Classes /////

class IoServicePool
{
 public:

  IoServicePool(const size_t numioservice, const size_t numthreads, const std::function<void()>& threadstart, const std::function<void()>& threadend);
  ~IoServicePool();

  void Destroy();
  
  boost::asio::io_service& GetIoService(); // Round robin

 private:
 
  mutable std::mutex mutex_;

  std::vector< std::unique_ptr<boost::asio::io_service> > ioservices_; // These are pointers to keep them away from eachother so we don't get cache overlapping
  std::vector<boost::asio::io_service::work> workers_;
  std::vector<std::thread> threads_;

  size_t nextioservice_;
  
};

}

#endif
