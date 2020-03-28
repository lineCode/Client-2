// testserver.hpp
//

#ifndef IDHDWTLPP6ZXZJF32MXCSXZ4YWZOJ6O51N
#define IDHDWTLPP6ZXZJF32MXCSXZ4YWZOJ6O51N

///// Includes /////

#include <monocleprotocol/server/connection.hpp>
#include <monocleprotocol/server/server.hpp>

#include "testconnection.hpp"

///// Namespaces /////

namespace monocle
{

namespace server
{

namespace tests
{

///// Classes /////

class TestServer : public monocle::server::Server
{
 public:

  TestServer(const std::string& testusername, const std::string& testpassword, const std::string& testnonce, const STREAM& teststream, const std::string& testname, const std::string& testpublickey, const std::string& testarchitecture, const int testoperatingsystem, const std::string& testcompiler, const std::string& testdatabasepath, const utility::Version& testversion, const uint64_t testidentifier, const std::vector<std::string>& testenvironmentvariables, const std::vector<std::string>& testcommandlinevariables, const std::vector<ONVIFUSER>& testonvifusers, const std::vector<GROUP>& testgroups, const std::vector<USER>& testusers, const std::vector<FILE>& testfiles, const std::vector<RECEIVER>& testreceivers, const std::vector<RECORDING>& testrecordings, const std::vector<monocle::LOGMESSAGE>& testrecordinglogmessages, const uint32_t maxrecordings, const std::vector<LAYOUT>& testlayouts, const std::vector<MAP>& testmaps, const std::vector<MOUNTPOINT>& testmountpoints, const std::string& testlatitude, const std::string& testlongitude, const int testnumcudadevices, const int testnumcldevices, const int testmaxobjectdetectors, const TESTFRAME& testframe);
  virtual ~TestServer();

  virtual boost::shared_ptr<server::Connection> CreateConnection(boost::asio::io_service& io) override;

  std::vector< boost::shared_ptr<Connection> > GetConnections() { std::lock_guard<std::mutex> lock(mutex_); return connections_; }

  

 protected:


 private:

  const std::string testusername_;
  const std::string testpassword_;
  const std::string testnonce_;
  const STREAM teststream_;
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
  const std::vector<monocle::LOGMESSAGE> testrecordinglogmessages_;
  const uint32_t maxrecordings_;
  const std::vector<LAYOUT> testlayouts_;
  const std::vector<MAP> testmaps_;
  const std::vector<MOUNTPOINT> testmountpoints_;
  const std::string testlatitude_;
  const std::string testlongitude_;
  const int testnumcudadevices_;
  const int testnumcldevices_;
  const int testmaxobjectdetectors_;
  const TESTFRAME testframe_;

};

}

}

}

#endif
