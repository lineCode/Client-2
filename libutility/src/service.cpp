// service.cpp
//

#ifdef _WIN32

///// Includes /////

#include "utility/service.hpp"

#include <memory>

///// Namespaces /////

namespace utility
{

///// Methods /////

Service::Service(SC_HANDLE handle) :
  handle_(handle)
{

}

Service::~Service()
{
  if (handle_)
  {
    CloseServiceHandle(handle_);
    handle_ = nullptr;
  }
}

int Service::GetStatus(SERVICE_STATUS& servicestatus) const
{
  if (!handle_)
  {

    return 1;
  }

  if (!QueryServiceStatus(handle_, &servicestatus))
  {

    return 1;
  }

  return 0;
}

int Service::GetConfig(SERVICECONFIGURATION& serviceconfig) const
{
  if (!handle_)
  {

    return 1;
  }

  DWORD numbytes = 0;
  QueryServiceConfig(handle_, nullptr, 0, &numbytes);
  if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
  {

    return 1;
  }

  std::unique_ptr<unsigned char[]> tmp = std::make_unique<unsigned char[]>(numbytes);
  if (!QueryServiceConfig(handle_, reinterpret_cast<QUERY_SERVICE_CONFIG*>(tmp.get()), numbytes, &numbytes))
  {

    return 1;
  }

  serviceconfig = SERVICECONFIGURATION(*reinterpret_cast<QUERY_SERVICE_CONFIG*>(tmp.get()));
  return 0;
}

int Service::Start()
{
  if (!handle_)
  {

    return 1;
  }

  if (!StartService(handle_, 0, nullptr))
  {

    return 1;
  }

  return 0;
}

int Service::Stop()
{
  if (!handle_)
  {

    return 1;
  }

  SERVICE_STATUS servicestatus;
  if (!ControlService(handle_, SERVICE_CONTROL_STOP, &servicestatus))
  {
    
    return 1;
  }

  return 0;
}

int Service::GetDependentServices(std::vector<ENUM_SERVICE_STATUS>& dependentservices) const
{
  if (!handle_)
  {

    return 1;
  }

  DWORD numbytes = 0;
  DWORD count = 0;
  if (EnumDependentServices(handle_, SERVICE_STATE_ALL, nullptr, 0, &numbytes, &count))
  {

    return 0; // No dependent services, but still successful
  }
  else if (GetLastError() == ERROR_MORE_DATA)
  {
    std::unique_ptr<ENUM_SERVICE_STATUS[]> dependencies = std::make_unique<ENUM_SERVICE_STATUS[]>(numbytes);
    if (!EnumDependentServices(handle_, SERVICE_ACTIVE, dependencies.get(), numbytes, &numbytes, &count))
    {

      return 1;
    }

    for (DWORD i = 0; i < count; ++i)
    {
      dependentservices.push_back(dependencies.get()[i]);

    }

    return 0;
  }
  else
  {

    return 1;
  }
}

int Service::SetStartType(DWORD starttype)
{
  if (!handle_)
  {

    return 1;
  }

  if (!ChangeServiceConfig(handle_, SERVICE_NO_CHANGE, starttype, SERVICE_NO_CHANGE, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr))
  {

    return 1;
  }

  return 0;
}

}

#endif
