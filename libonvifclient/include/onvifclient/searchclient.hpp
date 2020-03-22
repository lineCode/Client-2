// searchclient.hpp
//

#ifndef IDR4PNN5NH4XQKLZRVVTTWBLKF63BIKXGI
#define IDR4PNN5NH4XQKLZRVVTTWBLKF63BIKXGI

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace search
{

///// Enumerations /////

enum SEARCHOPERATION
{
  SEARCHOPERATION_GETSERVICECAPABILITIES
};

///// Declarations /////

class SearchClient;
class SearchSignals;

///// Classes /////

class GetServiceCapabilitiesResponse : public Response<SearchClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<SearchClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<SearchClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class SearchClient : public Client<SEARCHOPERATION>, public boost::enable_shared_from_this<SearchClient>
{
 public:
  
  using Client::Update;

  SearchClient(std::recursive_mutex& mutex);
  virtual ~SearchClient();

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

  virtual void Update(SEARCHOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(SEARCHOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string GetServiceCapabilitiesBody();

  SearchSignals* signals_;

};

}

}

#endif
