// analyticsclient.hpp
//

#ifndef IDOU3D4R94UFEEOETNJPYD60X6UO0UIB7F
#define IDOU3D4R94UFEEOETNJPYD60X6UO0UIB7F

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace analytics
{

///// Enumerations /////

enum ANALYTICSOPERATION
{
  ANALYTICSOPERATION_GETSERVICECAPABILITIES
};

///// Declarations /////

class AnalyticsClient;
class AnalyticsSignals;

///// Classes /////

class GetServiceCapabilitiesResponse : public Response<AnalyticsClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<AnalyticsClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<AnalyticsClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class AnalyticsClient : public Client<ANALYTICSOPERATION>, public boost::enable_shared_from_this<AnalyticsClient>
{
  friend Signal<ANALYTICSOPERATION, AnalyticsClient, GetServiceCapabilitiesResponse>;

 public:
  
  using Client::Update;

  AnalyticsClient(const boost::shared_ptr<std::recursive_mutex>& mutex);
  virtual ~AnalyticsClient();

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse) override;
  virtual void Destroy() override;

  // Requests
  void GetServiceCapabilities();
  
  // Callback requests
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);

  // Future requests
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();

  // Boost Signals2
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();

 private:

  virtual void Update(ANALYTICSOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(ANALYTICSOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string GetServiceCapabilitiesBody();

  AnalyticsSignals* signals_;

};

}

}

#endif
