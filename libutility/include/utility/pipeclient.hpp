// pipeclient.hpp
//

#ifndef IDENFCXUZG1ONK73GSDFSDFSU6WYBM77DZ
#define IDENFCXUZG1ONK73GSDFSDFSU6WYBM77DZ

///// Includes /////

#include <array>
#include <string>
#include <vector>

#ifdef _WIN32
  #include <windows.h>
#else

#endif

///// Namespaces /////

namespace utility
{

///// Classes /////

class PipeClient
{
 public:

  PipeClient();
  PipeClient(const PipeClient&) = delete;
  virtual ~PipeClient();

  virtual int Init(const std::string& uuid);
  virtual void Destroy();
  
  int Update();

  int SendText(const std::string& message); // Message must not be empty

  virtual void Message(const std::string& message) = 0;
  
 private:

#ifdef _WIN32  
  HANDLE pipe_;
  OVERLAPPED overlapped_;
#else
  int pipe_;
#endif

  std::array<char, 128> buffer_;
  std::string message_;

};

}

#endif
