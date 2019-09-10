// pipeclient.cpp
//

///// Includes /////

#include "utility/pipeclient.hpp"

#include <boost/scope_exit.hpp>

///// Namespaces /////

namespace utility
{

///// Methods /////

PipeClient::PipeClient() :
#ifdef _WIN32
  pipe_(INVALID_HANDLE_VALUE)
#else
  pipe_(-1)
#endif
{
  buffer_.fill('\0');
#ifdef _WIN32
  memset(&overlapped_, 0, sizeof(overlapped_));
#endif
}

PipeClient::~PipeClient()
{
  Destroy();

}

int PipeClient::Init(const std::string& uuid)
{
  Destroy();
#ifdef _WIN32
  const std::string pipename = ("\\\\.\\pipe\\" + uuid);
  pipe_ = CreateFile(pipename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
  if (pipe_ == INVALID_HANDLE_VALUE)
  {
    // Wait a short period to have a go again if another pipe becomes available
    if (WaitNamedPipe(pipename.c_str(), 500) == 0)
    {

      return 1;
    }

    pipe_ = CreateFile(pipename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
    if (pipe_ == INVALID_HANDLE_VALUE)
    {

      return 2;
    }
  }

  memset(&overlapped_, 0, sizeof(OVERLAPPED));
  overlapped_.hEvent = CreateEvent(nullptr, true, false, nullptr);
  if (overlapped_.hEvent == nullptr)
  {

    return 3;
  }

  if (ReadFile(pipe_, buffer_.data(), static_cast<DWORD>(buffer_.size()), nullptr, &overlapped_))
  {

    return 4;
  }
#endif
  return 0;
}

void PipeClient::Destroy()
{
#ifdef _WIN32
  if (overlapped_.hEvent)
  {
    CloseHandle(overlapped_.hEvent);
    overlapped_.hEvent = nullptr;
  }

  if (pipe_ != INVALID_HANDLE_VALUE)
  {
    CloseHandle(pipe_);
    pipe_ = INVALID_HANDLE_VALUE;
  }
#endif
}

int PipeClient::Update()
{
#ifdef _WIN32
  if (pipe_ == INVALID_HANDLE_VALUE)
  {

    return 1;
  }
  
  if (overlapped_.hEvent == nullptr)
  {

    return 2;
  }

  const DWORD ret = WaitForSingleObject(overlapped_.hEvent, 0);
  if (ret == WAIT_ABANDONED)
  {
    // Ignore this because it shouldn't happen

  }
  else if (ret == WAIT_OBJECT_0)
  {
    const size_t size = message_.size();
    message_.resize(size + (overlapped_.InternalHigh - overlapped_.Internal));
    std::copy(buffer_.begin() + overlapped_.Internal, buffer_.begin() + overlapped_.InternalHigh, message_.data() + size);

    size_t pos = message_.find('\0');
    while (pos != std::string::npos)
    {
      Message(message_.substr(0, pos));
      message_.erase(message_.begin(), message_.begin() + pos + 1);
      pos = message_.find('\0');
    }

    CloseHandle(overlapped_.hEvent);
    memset(&overlapped_, 0, sizeof(OVERLAPPED));
    overlapped_.hEvent = CreateEvent(nullptr, true, false, nullptr);
    if (ReadFile(pipe_, buffer_.data(), static_cast<DWORD>(buffer_.size()), nullptr, &overlapped_))
    {
      const DWORD lasterror = GetLastError();
      if (lasterror && ((lasterror != ERROR_NO_DATA) && (lasterror != ERROR_IO_PENDING)))
      {

        return 4;
      }
    }
  }
  else if (ret == WAIT_TIMEOUT)
  {
    // Nothing happened

  }
  else // WAIT_FAILED
  {

    return 5;
  }
#endif
  return 0;
}

int PipeClient::SendText(const std::string& message)
{
  if (message.empty())
  {

    return 1;
  }
#ifdef _WIN32
  if (pipe_ == INVALID_HANDLE_VALUE)
  {

    return 2;
  }

  DWORD byteswritten = 0;
  OVERLAPPED overlapped;
  memset(&overlapped, 0, sizeof(overlapped));
  overlapped.hEvent = CreateEvent(nullptr, true, false, nullptr);
  BOOST_SCOPE_EXIT(overlapped)
  {
    CloseHandle(overlapped.hEvent);
  } BOOST_SCOPE_EXIT_END

  if (WriteFile(pipe_, message.data(), static_cast<DWORD>(message.size() + 1), &byteswritten, &overlapped) == 0)
  {
    if (GetLastError() != ERROR_IO_PENDING)
    {

      return 3;
    }

    // Wait for WriteFile to complete synchronously
    if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0)
    {

      return 4;
    }

    if (GetOverlappedResult(pipe_, &overlapped, &byteswritten, true) == 0)
    {

      return 5;
    }
  }
  else
  {
    // WriteFile completed immediately

  }
#endif
  return 0;
}

}
