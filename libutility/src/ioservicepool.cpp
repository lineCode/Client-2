// ioservicepool.cpp
//

///// Includes /////

#include "utility/ioservicepool.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>

#ifdef __linux__
  #include <sys/sysinfo.h>
  #include <pthread.h>
#endif

///// Namespaces /////

namespace utility
{

///// Methods /////

IoServicePool::IoServicePool(const size_t numioservice, const size_t numthreads, const std::function<void()>& threadstart, const std::function<void()>& threadend) :
  nextioservice_(0)
{
  if (numioservice == 0)
  {
    throw std::runtime_error("Invalid number of ioservices");

  }

  if (numthreads == 0)
  {
    throw std::runtime_error("Invalid number of ioservice threads");

  }

#ifdef _WIN32
  SYSTEM_INFO systeminfo;
  memset(&systeminfo, 0, sizeof(systeminfo));
  GetSystemInfo(&systeminfo);
  const uint64_t numcpu = systeminfo.dwNumberOfProcessors;
#elif __ANDROID__
  const uint64_t numcpu = sysconf(_SC_NPROCESSORS_ONLN);
#else
  const uint64_t numcpu = get_nprocs_conf();
#endif
  uint64_t nextcpu = 0;

  ioservices_.reserve(numioservice);
  for (int i = 0; i < numioservice; ++i)
  {
    ioservices_.emplace_back(std::make_unique<boost::asio::io_service>(0));
    workers_.push_back(boost::asio::io_service::work(*ioservices_.back()));
    for (int i = 0; i < numthreads; ++i)
    {
      threads_.push_back(std::thread([threadstart, threadend, io = ioservices_.back().get()]()
      {
        threadstart();
        io->run();
        threadend();
      }));
      // Pin all the threads of a single ioservice to a core so the reactor queue remains in the same NUMA locale with a round robin
#ifdef _WIN32
      if (SetThreadAffinityMask(threads_.back().native_handle(), 1ull << nextcpu) == 0)
      {
        std::cout << "IoServicePool::IoServicePool() SetThreadAffinityMask failed " << nextcpu << std::endl; // This just shouldn't happen often enough to care about logging this from the utility library into the server or client

      }
#elif defined(__linux__) && !defined(__ANDROID__)
      cpu_set_t mask;
      CPU_ZERO(&mask);
      CPU_SET(nextcpu, &mask);
      if (pthread_setaffinity_np(threads_.back().native_handle(), sizeof(cpu_set_t), &mask))
      {
        std::cout << "IoServicePool::IoServicePool() pthread_setaffinity_np failed " << nextcpu << std::endl; // This just shouldn't happen often enough to care about logging this from the utility library into the server or client

      }
#endif
    }

    if (++nextcpu >= numcpu)
    {
      nextcpu = 0;

    }
  }
}

IoServicePool::~IoServicePool()
{
  Destroy();

}

void IoServicePool::Destroy()
{
  std::lock_guard<std::mutex> lock(mutex_);
  for (std::unique_ptr<boost::asio::io_service>& ioservice : ioservices_)
  {
    ioservice->stop();

  }

  for (std::thread& thread : threads_)
  {
    thread.join();

  }

  for (std::unique_ptr<boost::asio::io_service>& ioservice : ioservices_)
  {
    ioservice->reset();

  }

  threads_.clear();
  workers_.clear();
  ioservices_.clear();
}

boost::asio::io_service& IoServicePool::GetIoService()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (nextioservice_ >= ioservices_.size())
  {
    nextioservice_ = 0;

  }
  return *ioservices_[nextioservice_++];
}

}
