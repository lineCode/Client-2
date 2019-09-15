// servicemgr.cpp
//

#ifdef _WIN32

///// Includes /////

#include "utility/servicemgr.hpp"

#include <boost/make_shared.hpp>

#include "utility/service.hpp"

///// Namespaces /////

namespace utility
{

///// Methods /////
ServiceMgr::ServiceMgr() :
  handle_(nullptr)
{

}

ServiceMgr::~ServiceMgr()
{
  Destroy();

}

int ServiceMgr::Init(DWORD access)
{
  Destroy();

  handle_ = OpenSCManager(nullptr, nullptr, access);
  if (!handle_)
  {
    
    return 1;
  }

  return 0;
}

void ServiceMgr::Destroy()
{
  if (handle_)
  {
    CloseServiceHandle(handle_);
    handle_ = nullptr;
  }
}

boost::shared_ptr<Service> ServiceMgr::GetService(const std::string& name, DWORD access)
{
  if (!handle_)
  {

    return nullptr;
  }

  SC_HANDLE handle = OpenService(handle_, name.c_str(), access);
  if (!handle)
  {

    return nullptr;
  }

  return boost::make_shared<Service>(handle);
}

}

#endif