// testsearchclient.hpp
//

#ifndef ID3KVKLNIP38669BUE69JKO3V9S5VFXLH5
#define ID3KVKLNIP38669BUE69JKO3V9S5VFXLH5

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummysearchservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestSearchClient : public TestClient<onvif::search::SearchClient>
{
 public:

  TestSearchClient();
  virtual ~TestSearchClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummySearchService> dummysearchservice_;

 private:

};

}

}

#endif
