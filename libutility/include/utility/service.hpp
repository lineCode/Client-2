// service.hpp
//

#ifndef IDU3ZGRJ38MY1ZDJK44FB7IC67QEG35NT5
#define IDU3ZGRJ38MY1ZDJK44FB7IC67QEG35NT5

#ifdef _WIN32

///// Includes /////

#include <string>
#include <vector>
#include <windows.h>

///// Namespaces /////

namespace utility
{

///// Structures /////

struct SERVICECONFIGURATION
{
  SERVICECONFIGURATION() :
    starttype_(0)
  {

  }

  SERVICECONFIGURATION(const QUERY_SERVICE_CONFIG& serviceconfig) :
    starttype_(serviceconfig.dwStartType)
  {
    if (serviceconfig.lpBinaryPathName)
    {
      path_ = std::string(serviceconfig.lpBinaryPathName);

    }

    if (serviceconfig.lpDisplayName)
    {
      displayname_ = std::string(serviceconfig.lpDisplayName);

    }
  }

  DWORD starttype_;
  std::string path_;
  std::string displayname_;
};

struct SERVICEDETAILS
{
  std::string servicename_;
  std::string displayname_;
  SERVICE_STATUS servicestatus_;
};

///// Classes /////

class Service
{
 public:

  Service(SC_HANDLE handle);
  Service(const Service&) = delete;
  ~Service();

  int GetStatus(SERVICE_STATUS& servicestatus) const;
  int GetConfig(SERVICECONFIGURATION& serviceconfig) const;
  int GetDependentServices(std::vector<ENUM_SERVICE_STATUS>& dependentservices) const;

  int SetStartType(DWORD starttype);

  int Start();
  int Stop();

 private:

  SC_HANDLE handle_;

};

}

#endif

#endif
