// pipeserver.hpp
//

#ifndef IDENFCXUZG1ON3403943434347344M77DZ
#define IDENFCXUZG1ON3403943434347344M77DZ

///// Includes /////

#include <array>
#include <string>

#ifdef _WIN32
  #include <windows.h>
#else

#endif

///// Namespaces /////

namespace utility
{

#ifdef _WIN32
const size_t NUM_PIPES = 5;
#endif

///// Structures /////

struct PIPE
{
  PIPE();

#ifdef _WIN32
  bool connected_;
  HANDLE handle_;
  OVERLAPPED overlapped_;
#else
  int handle_;
#endif
  std::array<char, 128> buffer_;
  std::string message_;

};

///// Classes /////

class PipeServer
{
 public:

  PipeServer();
  PipeServer(const PipeServer&) = delete;
  virtual ~PipeServer();

  virtual int Init(const std::string& uuid);
  virtual void Destroy();
  
  int Update();

  int SendText(const std::string& message); // Message must not be empty

  virtual void PipeConnected() = 0;
  virtual void Message(const std::string& message) = 0;
  
 private:

#ifdef _WIN32
  int ResetOverlapped(OVERLAPPED& overlapped) const;
#endif
  int ResetPipe(const size_t i);
  
#ifdef _WIN32
  SECURITY_DESCRIPTOR* securitydescriptor_;
  std::array<PIPE, NUM_PIPES> pipes_;
#else
  int pipe_;
#endif

};

}

#endif
