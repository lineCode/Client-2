// imagingclient.hpp
//

#ifndef IDJV1LUL0O4KE8R1IFZOFE6H6DHB1J0OZ8
#define IDJV1LUL0O4KE8R1IFZOFE6H6DHB1J0OZ8

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Declarations /////

class ImagingClient;
class ImagingSignals;

///// Enumerations /////

enum IMAGINGOPERATION
{
  IMAGINGOPERATION_GETIMAGINGSETTINGS,
  IMAGINGOPERATION_GETOPTIONS,
  IMAGINGOPERATION_GETSERVICECAPABILITIES,
  IMAGINGOPERATION_SETIMAGINGSETTINGS,
};

///// Classes /////

class GetImagingSettingsResponse : public Response<ImagingClient>
{
 public:

  GetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken);
  GetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const boost::optional<ImagingSettings20>& imagingsettings);
  virtual ~GetImagingSettingsResponse();

  std::string videosourcetoken_;
  boost::optional<ImagingSettings20> imagingsettings_;

};

class GetOptionsResponse : public Response<ImagingClient>
{
 public:

  GetOptionsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken);
  GetOptionsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const boost::optional<ImagingOptions20>& imagingoptions);
  virtual ~GetOptionsResponse();

  std::string videosourcetoken_;
  boost::optional<ImagingOptions20> imagingoptions_;

};

class GetServiceCapabilitiesResponse : public Response<ImagingClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class SetImagingSettingsResponse : public Response<ImagingClient>
{
 public:

  SetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence);
  virtual ~SetImagingSettingsResponse();

  std::string videosourcetoken_;
  ImagingSettings20 imagingsettings_;
  bool forcepersistence_;

};

class ImagingClient : public Client<IMAGINGOPERATION>, public boost::enable_shared_from_this<ImagingClient>
{
  friend Signal<IMAGINGOPERATION, ImagingClient, GetImagingSettingsResponse, std::string>;
  friend Signal<IMAGINGOPERATION, ImagingClient, GetOptionsResponse, std::string>;
  friend Signal<IMAGINGOPERATION, ImagingClient, GetServiceCapabilitiesResponse>;
  friend Signal<IMAGINGOPERATION, ImagingClient, SetImagingSettingsResponse, std::string, ImagingSettings20, bool>;

 public:
  
  using Client::Update;

  ImagingClient(const boost::shared_ptr<std::recursive_mutex>& mutex);
  virtual ~ImagingClient();

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse) override;
  virtual void Destroy() override;

  // Requests
  void GetImagingSettings(const std::string& videosourcetoken);
  void GetOptions(const std::string& videosourcetoken);
  void GetServiceCapabilities();
  void SetImagingSettings(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence);
  
  // Callback requests
  Connection GetImagingSettingsCallback(const std::string& videosourcetoken, boost::function<void(const GetImagingSettingsResponse&)> callback);
  Connection GetOptionsCallback(const std::string& videosourcetoken, boost::function<void(const GetOptionsResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection SetImagingSettingsCallback(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence, boost::function<void(const SetImagingSettingsResponse&)> callback);

  // Future requests
  boost::unique_future<GetImagingSettingsResponse> GetImagingSettingsFuture(const std::string& videosourcetoken);
  boost::unique_future<GetOptionsResponse> GetOptionsFuture(const std::string& videosourcetoken);
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<SetImagingSettingsResponse> SetImagingSettingsFuture(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence);

  // Boost Signals2
  boost::signals2::signal<void(const GetImagingSettingsResponse&)>& GetImagingSettingsSignal(const std::string& videosourcetoken);
  boost::signals2::signal<void(const GetOptionsResponse&)>& GetOptionsSignal(const std::string& videosourcetoken);
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const SetImagingSettingsResponse&)>& SetImagingSettingsSignal(const std::string& videosourcetoken);

 private:

  virtual void Update(IMAGINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(IMAGINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string GetImagingSettingsBody(const std::string& videosourcetoken);
  std::string GetOptionsBody(const std::string& videosourcetoken);
  std::string GetServiceCapabilitiesBody();
  std::string SetImagingSettingsBody(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence);

  // Signals
  ImagingSignals* signals_;

};

}

}

#endif
