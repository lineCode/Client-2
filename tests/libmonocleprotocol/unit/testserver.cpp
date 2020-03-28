// testserver.cpp
//

///// Includes /////

#include "testserver.hpp"

#include <boost/make_shared.hpp>
#include <gtest/gtest.h>

#include "testconnection.hpp"

///// Namespaces /////

namespace monocle
{

namespace server
{

namespace tests
{

///// Methods /////

TestServer::TestServer(const std::string& testusername, const std::string& testpassword, const std::string& testnonce, const STREAM& teststream, const std::string& testname, const std::string& testpublickey, const std::string& testarchitecture, const int testoperatingsystem, const std::string& testcompiler, const std::string& testdatabasepath, const utility::Version& testversion, const uint64_t testidentifier, const std::vector<std::string>& testenvironmentvariables, const std::vector<std::string>& testcommandlinevariables, const std::vector<ONVIFUSER>& testonvifusers, const std::vector<GROUP>& testgroups, const std::vector<USER>& testusers, const std::vector<FILE>& testfiles, const std::vector<RECEIVER>& testreceivers, const std::vector<RECORDING>& testrecordings, const std::vector<monocle::LOGMESSAGE>& testrecordinglogmessages, const uint32_t maxrecordings, const std::vector<LAYOUT>& testlayouts, const std::vector<MAP>& testmaps, const std::vector<MOUNTPOINT>& testmountpoints, const std::string& testlatitude, const std::string& testlongitude, const int testnumcudadevices, const int testnumcldevices, const int testmaxobjectdetectors, const TESTFRAME& testframe) :
  Server(4),
  testusername_(testusername),
  testpassword_(testpassword),
  testnonce_(testnonce),
  teststream_(teststream),
  testname_(testname),
  testpublickey_(testpublickey),
  testarchitecture_(testarchitecture),
  testoperatingsystem_(testoperatingsystem),
  testcompiler_(testcompiler),
  testdatabasepath_(testdatabasepath),
  testversion_(testversion),
  testidentifier_(testidentifier),
  testenvironmentvariables_(testenvironmentvariables),
  testcommandlinevariables_(testcommandlinevariables),
  testonvifusers_(testonvifusers),
  testgroups_(testgroups),
  testusers_(testusers),
  testfiles_(testfiles),
  testreceivers_(testreceivers),
  testrecordings_(testrecordings),
  testrecordinglogmessages_(testrecordinglogmessages),
  maxrecordings_(maxrecordings),
  testlayouts_(testlayouts),
  testmaps_(testmaps),
  testmountpoints_(testmountpoints),
  testlatitude_(testlatitude),
  testlongitude_(testlongitude),
  testnumcudadevices_(testnumcudadevices),
  testnumcldevices_(testnumcldevices),
  testmaxobjectdetectors_(testmaxobjectdetectors),
  testframe_(testframe)
{

}

TestServer::~TestServer()
{

}

boost::shared_ptr<server::Connection> TestServer::CreateConnection(boost::asio::io_service& io)
{
  return boost::make_shared<TestConnection>(io, shared_from_this(), testusername_, testpassword_, testnonce_, teststream_, testname_, testpublickey_, testarchitecture_, testoperatingsystem_, testcompiler_, testdatabasepath_, testversion_, testidentifier_, testenvironmentvariables_, testcommandlinevariables_, testonvifusers_, testgroups_, testusers_, testfiles_, testreceivers_, testrecordings_, testrecordinglogmessages_, maxrecordings_, testlayouts_, testmaps_, testmountpoints_, testlatitude_, testlongitude_, testnumcudadevices_, testnumcldevices_, testmaxobjectdetectors_, testframe_);
}

}

}

}
