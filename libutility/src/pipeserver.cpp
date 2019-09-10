// pipeserver.cpp
//

///// Includes /////

#include "utility/pipeserver.hpp"

#include <array>
#include <boost/scope_exit.hpp>

///// Namespaces /////

namespace utility
{

///// Methods /////

PIPE::PIPE() :
#ifdef _WIN32
  connected_(false),
  handle_(INVALID_HANDLE_VALUE)
#else
  handle_(-1)
#endif
{
#ifdef _WIN32
  memset(&overlapped_, 0, sizeof(overlapped_));
#endif
  buffer_.fill('\0');
}

PipeServer::PipeServer() :
#ifdef _WIN32
  securitydescriptor_(nullptr)
#else
  pipe_(-1)
#endif
{

}

PipeServer::~PipeServer()
{
  Destroy();

}

int PipeServer::Init(const std::string& uuid)
{
  Destroy();
#ifdef _WIN32
  securitydescriptor_ = (SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
  if (securitydescriptor_ == nullptr)
  {
    return 1;
  }

  if (!InitializeSecurityDescriptor(securitydescriptor_, SECURITY_DESCRIPTOR_REVISION))
  {

    return 2;
  }

  if (!SetSecurityDescriptorDacl(securitydescriptor_, true, nullptr, false))
  {

    return 3;
  }

  SECURITY_ATTRIBUTES securityattributes;
  securityattributes.nLength = sizeof(securityattributes);
  securityattributes.lpSecurityDescriptor = securitydescriptor_;
  securityattributes.bInheritHandle = FALSE;

  for (size_t i = 0; i < NUM_PIPES; ++i)
  {
    pipes_[i].handle_ = CreateNamedPipe(("\\\\.\\pipe\\" + uuid).c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, static_cast<DWORD>(NUM_PIPES), 2048, 128, static_cast<DWORD>(NUM_PIPES), &securityattributes);
    if (pipes_[i].handle_ == INVALID_HANDLE_VALUE)
    {

      return 4;
    }
    pipes_[i].connected_ = false;

    memset(&pipes_[i].overlapped_, 0, sizeof(OVERLAPPED));
    pipes_[i].overlapped_.hEvent = CreateEvent(nullptr, true, false, nullptr);
    if (pipes_[i].overlapped_.hEvent == nullptr)
    {

      return 5;
    }

    if (ConnectNamedPipe(pipes_[i].handle_, &pipes_[i].overlapped_) == 0)
    {
      const DWORD lasterror = GetLastError();
      if (lasterror)
      {
        if ((lasterror == ERROR_IO_PENDING) || (lasterror == ERROR_PIPE_CONNECTED))
        {
          // Is ok, will catch this later

        }
        else
        {

          return 6;
        }
      }
    }
    else
    {
      // Should never happen according to the documentation
      return 7;
    }
  }
#else
  
#endif
  return 0;
}

void PipeServer::Destroy()
{
#ifdef _WIN32
  for (size_t i = 0; i < NUM_PIPES; ++i)
  {
    if (pipes_[i].handle_ != INVALID_HANDLE_VALUE)
    {
      CloseHandle(pipes_[i].handle_);
      pipes_[i].handle_ = INVALID_HANDLE_VALUE;
      pipes_[i].connected_ = false;
    }

    if (pipes_[i].overlapped_.hEvent)
    {
      CloseHandle(pipes_[i].overlapped_.hEvent);
      pipes_[i].overlapped_.hEvent = nullptr;
    }
    pipes_[i].buffer_.fill('\0');
    pipes_[i].message_.clear();
  }

  if (securitydescriptor_)
  {
    LocalFree(securitydescriptor_);
    securitydescriptor_ = nullptr;
  }
#endif
}

int PipeServer::Update()
{
#ifdef _WIN32
  for (size_t i = 0; i < NUM_PIPES; ++i)
  {
    if (pipes_[i].handle_ == INVALID_HANDLE_VALUE)
    {

      continue;
    }

    if (pipes_[i].connected_)
    {
      const DWORD ret = WaitForSingleObject(pipes_[i].overlapped_.hEvent, 0);
      if (ret == WAIT_ABANDONED)
      {
        if (ResetPipe(i))
        {

          return 1;
        }
      }
      else if (ret == WAIT_OBJECT_0)
      {
        const size_t size = pipes_[i].message_.size();
        pipes_[i].message_.resize(size + pipes_[i].overlapped_.InternalHigh);
        std::copy(pipes_[i].buffer_.begin(), pipes_[i].buffer_.begin() + pipes_[i].overlapped_.InternalHigh, pipes_[i].message_.data() + size);

        size_t pos = pipes_[i].message_.find('\0');
        while (pos != std::string::npos)
        {
          Message(pipes_[i].message_.substr(0, pos));
          pipes_[i].message_.erase(pipes_[i].message_.begin(), pipes_[i].message_.begin() + pos + 1);
          pos = pipes_[i].message_.find('\0');
        }

        if (ResetOverlapped(pipes_[i].overlapped_))
        {

          return 2;
        }

        if (ReadFile(pipes_[i].handle_, pipes_[i].buffer_.data(), static_cast<DWORD>(pipes_[i].buffer_.size()), nullptr, &pipes_[i].overlapped_))
        {
          const DWORD lasterror = GetLastError();
          if (lasterror && ((lasterror != ERROR_NO_DATA) && (lasterror != ERROR_IO_PENDING)))
          {

            return 3;
          }
        }
        else
        {
          const DWORD lasterror = GetLastError();
          if (lasterror != ERROR_IO_PENDING)
          {
            if (ResetPipe(i))
            {

              return 4;
            }
          }
        }
      }
      else if (ret == WAIT_TIMEOUT)
      {
        // Nothing happened

      }
      else // WAIT_FAILED
      {
        if (ResetPipe(i))
        {

          return 5;
        }
      }
    }
    else
    {
      // Connect clients
      const DWORD ret = WaitForSingleObject(pipes_[i].overlapped_.hEvent, 0);
      if (ret == WAIT_ABANDONED)
      {
        if (ResetPipe(i))
        {

          return 6;
        }
      }
      else if (ret == WAIT_OBJECT_0)
      {
        if (ResetOverlapped(pipes_[i].overlapped_))
        {

          return 7;
        }

        DWORD bytesread = 0;
        if (ReadFile(pipes_[i].handle_, pipes_[i].buffer_.data(), static_cast<DWORD>(pipes_[i].buffer_.size()), &bytesread, &pipes_[i].overlapped_))
        {
          const DWORD lasterror = GetLastError();
          if (lasterror && (lasterror != ERROR_IO_PENDING))
          {

            return 8;
          }
        }

        pipes_[i].connected_ = true;
        PipeConnected();
      }
      else if (ret == WAIT_TIMEOUT)
      {
        // Nothing happened

      }
      else
      {
        if (ResetPipe(i))
        {

          return 9;
        }
      }
    }
  }
#endif
  return 0;
}

int PipeServer::SendText(const std::string& message)
{
  if (message.empty())
  {

    return 1;
  }
#ifdef _WIN32
  for (size_t i = 0; i < NUM_PIPES; ++i)
  {
    if (pipes_[i].handle_ == INVALID_HANDLE_VALUE)
    {
      // This shouldn't really happen...
      continue;
    }

    if (pipes_[i].connected_)
    {
      DWORD byteswritten = 0;
      OVERLAPPED overlapped;
      memset(&overlapped, 0, sizeof(overlapped));
      overlapped.hEvent = CreateEvent(nullptr, true, false, nullptr);
      BOOST_SCOPE_EXIT(overlapped)
      {
        CloseHandle(overlapped.hEvent);
      } BOOST_SCOPE_EXIT_END
      if (WriteFile(pipes_[i].handle_, message.data(), static_cast<DWORD>(message.size() + 1), &byteswritten, &overlapped) == 0)
      {
        if (GetLastError() != ERROR_IO_PENDING)
        {
          if (ResetPipe(i))
          {

            return 2;
          }
          continue;
        }
      }

      if (GetOverlappedResult(pipes_[i].handle_, &overlapped, &byteswritten, true) == 0)
      {

        return 3;
      }
    }
  }
#endif
  return 0;
}

#ifdef _WIN32
int PipeServer::ResetOverlapped(OVERLAPPED& overlapped) const
{
  if (overlapped.hEvent)
  {
    CloseHandle(overlapped.hEvent);

  }

  memset(&overlapped, 0, sizeof(overlapped));
  overlapped.hEvent = CreateEvent(nullptr, true, false, nullptr);
  if (overlapped.hEvent == nullptr)
  {

    return 1;
  }
  return 0;
}
#endif

int PipeServer::ResetPipe(const size_t i)
{
#ifdef _WIN32
  if (DisconnectNamedPipe(pipes_[i].handle_) == 0)
  {

    return 1;
  }

  if (ResetOverlapped(pipes_[i].overlapped_))
  {

    return 2;
  }

  if (ConnectNamedPipe(pipes_[i].handle_, &pipes_[i].overlapped_) == 0)
  {
    const DWORD lasterror = GetLastError();
    if (lasterror && (lasterror != ERROR_IO_PENDING) && (lasterror != ERROR_PIPE_CONNECTED))
    {

      return 3;
    }
  }

  pipes_[i].connected_ = false;
#endif
  return 0;
}

}
