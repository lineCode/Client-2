// servicemgr.hpp
//

#ifndef IDEF6OJQ0VIBSZHZJH3QIQKMV2MV5K81AB
#define IDEF6OJQ0VIBSZHZJH3QIQKMV2MV5K81AB

#ifdef _WIN32

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <string>
#include <windows.h>

#include "service.hpp"

///// Namespaces /////

namespace utility
{

///// Classes /////

class ServiceMgr
{
 public:

  ServiceMgr();
  ServiceMgr(const ServiceMgr&) = delete;
  ~ServiceMgr();

  int Init(DWORD access);
  void Destroy();

  boost::shared_ptr<Service> GetService(const std::string& name, DWORD access);

 private:

  SC_HANDLE handle_;

};

}

#endif

#endif
