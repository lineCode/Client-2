// testwsdiscoverserver.hpp
//

#ifndef IDZ376SCQKGEJDFMK8HBQ3WLH0K4N9M2N6
#define IDZ376SCQKGEJDFMK8HBQ3WLH0K4N9M2N6

///// Includes /////

#include <gtest/gtest.h>
#include <memory>
#include <utility/ioservicepool.hpp>
#include <wsdiscover/wsdiscoverserver.hpp>

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

namespace tests
{

///// Classes /////

class TestWsDiscoverServer : public testing::Test
{
 public:

 TestWsDiscoverServer();
 virtual ~TestWsDiscoverServer();

 protected:

 virtual void SetUp();
 virtual void TearDown();

 utility::IoServicePool ioservicepool_;
    
 boost::shared_ptr<WsDiscoverServer> discoverserver_;

};

}

}

}

#endif
