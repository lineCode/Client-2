// testservice.cpp
//

#ifdef _WIN32

///// Includes /////

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <utility/service.hpp>
#include <utility/servicemgr.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Classes /////

TEST(Service, OpenW32TimeStatusConfigDependent)
{
  ServiceMgr servicemgr;
  ASSERT_FALSE(servicemgr.Init(SC_MANAGER_ALL_ACCESS));
  auto service = servicemgr.GetService(std::string("W32Time"), SERVICE_ALL_ACCESS);
  ASSERT_TRUE(service);
  SERVICE_STATUS servicestatus;
  ASSERT_FALSE(service->GetStatus(servicestatus));
  utility::SERVICECONFIGURATION serviceconfig;
  ASSERT_FALSE(service->GetConfig(serviceconfig));
  std::vector<ENUM_SERVICE_STATUS> dependentservices;
  ASSERT_FALSE(service->GetDependentServices(dependentservices));
}

}

}

#endif
