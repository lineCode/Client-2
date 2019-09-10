// teststorageconfiguration.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestStorageConfiguration, Compare)
{
  Compare(device::StorageConfiguration(std::string("token"), device::StorageConfigurationData(std::string("localpath"), std::string("storageuri"), UserCredential(std::string("username"), std::string("password")), std::string("type"))));
}

}

}
