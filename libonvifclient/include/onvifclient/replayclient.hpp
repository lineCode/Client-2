// replayclient.hpp
//

#ifndef IDWS2L6BROSKGK46TT6W3QS85AISNXDUO5
#define IDWS2L6BROSKGK46TT6W3QS85AISNXDUO5

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace replay
{

///// Enumerations /////

enum REPLAYOPERATION
{
  REPLAYOPERATION_GETREPLAYURI,
  REPLAYOPERATION_GETSERVICECAPABILITIES
};

///// Declarations /////

class ReplayClient;
class ReplaySignals;

///// Classes /////

class GetReplayUriResponse : public Response<ReplayClient>
{
 public:

  GetReplayUriResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::StreamSetup& streamsetup, const std::string& recordingtoken);
  GetReplayUriResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::StreamSetup& streamsetup, const std::string& recordingtoken, const boost::optional<std::string>& uri);
  virtual ~GetReplayUriResponse();

  onvif::StreamSetup streamsetup_;
  std::string recordingtoken_;

  boost::optional<std::string> uri_;

};

class GetServiceCapabilitiesResponse : public Response<ReplayClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class ReplayClient : public Client<REPLAYOPERATION>, public boost::enable_shared_from_this<ReplayClient>
{
 public:
  
  using Client::Update;

  ReplayClient();
  virtual ~ReplayClient();

  virtual void Destroy() override;

  // Requests
  void GetReplayUri(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken);
  void GetServiceCapabilities();
  
  // Callback requests
  Connection GetReplayUriCallback(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken, boost::function<void(const GetReplayUriResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);

  // Future requests
  boost::unique_future<GetReplayUriResponse> GetReplayUriFuture(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken);
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();

  // Boost Signals2
  boost::signals2::signal<void(const GetReplayUriResponse&)>& GetReplayUriSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();

 private:

  virtual void Update(REPLAYOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(REPLAYOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string GetReplayUriBody(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken);
  std::string GetServiceCapabilitiesBody();

  ReplaySignals* signals_;

};

}

}

#endif
