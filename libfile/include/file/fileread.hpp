// fileread.hpp
//

#ifndef ID435380099CVXCVCXVCXVXCSXT0STS5UA
#define ID435380099CVXCVCXVCXVXCSXT0STS5UA

///// Includes /////

#include <atomic>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <future>
#include <stdint.h>
#include <thread>
#include <vector>

#ifdef _WIN32
  #include <windows.h>
#endif

#include "file.hpp"

///// Namespace /////

namespace file
{

///// Classes /////

class FileRead
{
 public:

  FileRead();
  ~FileRead();

  std::future<int> Init(const boost::filesystem::path& path, const std::shared_ptr<float>& progress);
  void Destroy();

  int Read(const uint64_t offset, const uint64_t size, char* buffer);

  inline const FILE& GetFile() const { return file_; }
  boost::optional<DEVICE> GetDevice(const uint64_t deviceindex) const;
  boost::optional<RECORDING> GetRecording(const uint64_t deviceindex, const uint64_t recordingindex) const;

 private:

#ifdef _WIN32
  HANDLE handle_;
#else
  int handle_;
#endif
  uint64_t size_;

  FILE file_;

  std::thread thread_;
  std::atomic<bool> running_;

};

}

#endif
