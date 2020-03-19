// testprotocol.hpp
//

#ifndef IDHDWTLL26M05JF72MXCSXT4YWZMMMO51N
#define IDHDWTLL26M05JF72MXCSXT4YWZMMMO51N

///// Includes /////

#include <gtest/gtest.h>
#include <thread>

#include "testclient.hpp"
#include "testconnection.hpp"
#include "testserver.hpp"

///// Namespaces /////

namespace monocle
{

namespace tests
{

///// Classes /////

class TestProtocol : public testing::Test
{
 public:

  TestProtocol();
  virtual ~TestProtocol();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  const server::STREAM teststream_;
  const std::string testname_;
  const std::string testpublickey_;
  const std::string testarchitecture_;
  const int testoperatingsystem_;
  const std::string testcompiler_;
  const std::string testdatabasepath_;
  const utility::Version testversion_;
  const uint64_t testidentifier_;
  const std::vector<std::string> testenvironmentvariables_;
  const std::vector<std::string> testcommandlinevariables_;
  const std::vector<ONVIFUSER> testonvifusers_;
  const std::vector<GROUP> testgroups_;
  const std::vector<USER> testusers_;
  const std::vector<FILE> testfiles_;
  const std::vector<RECEIVER> testreceivers_;
  const std::vector<RECORDING> testrecordings_;
  const std::vector<LOGMESSAGE> testrecordinglogmessages_;
  const uint32_t testmaxrecordings_;
  const std::vector<LAYOUT> testlayouts_;
  const std::vector<MAP> testmaps_;
  const std::vector<MOUNTPOINT> testmountpoints_;
  const std::string testlatitude_;
  const std::string testlongitude_;
  const int testnumcudadevices_;
  const int testnumcldevices_;
  const int testmaxobjectdetectors_;
  const server::tests::TESTFRAME testframe_;

  boost::shared_ptr<client::tests::TestClient> client_;
  boost::shared_ptr<server::tests::TestServer> server_;


 private:

};

}

}

#endif
