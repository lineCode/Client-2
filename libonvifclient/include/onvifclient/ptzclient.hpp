// ptzclient.hpp
//

#ifndef IDR8BLYC5CVEVOVP8G9UQ4CIOHEY28BKS6
#define IDR8BLYC5CVEVOVP8G9UQ4CIOHEY28BKS6

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Enumerations /////

enum PTZOPERATION
{
  PTZOPERATION_CONTINUOUSMOVE,
  PTZOPERATION_GETCOMPATIBLECONFIGURATIONS,
  PTZOPERATION_GETCONFIGURATION,
  PTZOPERATION_GETCONFIGURATIONOPTIONS,
  PTZOPERATION_GETCONFIGURATIONS,
  PTZOPERATION_GETNODES,
  PTZOPERATION_GETSERVICECAPABILITIES,
  PTZOPERATION_SETCONFIGURATION,
  PTZOPERATION_STOP
};

///// Declarations /////

class PTZClient;
class PTZSignals;

///// Classes /////

class ContinuousMoveResponse : public Response<PTZClient>
{
 public:

  ContinuousMoveResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout);
  virtual ~ContinuousMoveResponse();

  std::string profiletoken_;
  PTZSpeed velocity_;
  Duration timeout_;

};

class GetCompatibleConfigurationsResponse : public Response<PTZClient>
{
 public:

  GetCompatibleConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<PTZConfiguration>& ptzconfigurations);
  virtual ~GetCompatibleConfigurationsResponse();

  std::string profiletoken_;
  std::vector<PTZConfiguration> ptzconfigurations_;

};

class GetConfigurationResponse : public Response<PTZClient>
{
 public:

  GetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& ptzconfigurationtoken);
  GetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& ptzconfigurationtoken, const boost::optional<PTZConfiguration>& ptzconfiguration);
  virtual ~GetConfigurationResponse();

  std::string ptzconfigurationtoken_;

  boost::optional<PTZConfiguration> ptzconfiguration_;

};

class GetConfigurationOptionsResponse : public Response<PTZClient>
{
 public:

  GetConfigurationOptionsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetConfigurationOptionsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<PTZConfigurationOptions>& ptzconfigurationoptions);
  virtual ~GetConfigurationOptionsResponse();

  std::string configurationtoken_;

  boost::optional<PTZConfigurationOptions> ptzconfigurationoptions_;

};

class GetConfigurationsResponse : public Response<PTZClient>
{
 public:

  GetConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<PTZConfiguration>& ptzconfiguration);
  virtual ~GetConfigurationsResponse();

  std::vector<PTZConfiguration> ptzconfiguration_;

};

class GetNodesResponse : public Response<PTZClient>
{
 public:

  GetNodesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetNodesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<PTZNode>& ptznode);
  virtual ~GetNodesResponse();

  std::vector<PTZNode> ptznode_;

};

class GetServiceCapabilitiesResponse : public Response<PTZClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class SetConfigurationResponse : public Response<PTZClient>
{
 public:

  SetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const PTZConfiguration& ptzconfiguration, bool forcepersistence);
  virtual ~SetConfigurationResponse();

  PTZConfiguration ptzconfiguration_;
  bool forcepersistence_;

};

class StopResponse : public Response<PTZClient>
{
 public:

  StopResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, bool pantilt, bool zoom);
  virtual ~StopResponse();

  std::string profiletoken_;
  bool pantilt_;
  bool zoom_;

};

class PTZClient : public Client<PTZOPERATION>, public boost::enable_shared_from_this<PTZClient>
{
 public:
  
  using Client::Update;

  PTZClient(std::recursive_mutex& mutex);
  virtual ~PTZClient();

  virtual void Destroy() override;

  // Requests
  void ContinuousMove(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout);
  void GetCompatibleConfigurations(const std::string& profiletoken);
  void GetConfiguration(const std::string& ptzconfigurationtoken);
  void GetConfigurationOptions(const std::string& configurationtoken);
  void GetConfigurations();
  void GetNodes();
  void GetServiceCapabilities();
  void SetConfiguration(const PTZConfiguration& ptzconfiguration, bool forcepersistence);
  void Stop(const std::string& profiletoken, bool pantilt, bool zoom);
  
  // Callback requests
  Connection ContinuousMoveCallback(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout, boost::function<void(const ContinuousMoveResponse&)> callback);
  Connection GetCompatibleConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleConfigurationsResponse&)> callback);
  Connection GetConfigurationCallback(const std::string& ptzconfigurationtoken, boost::function<void(const GetConfigurationResponse&)> callback);
  Connection GetConfigurationOptionsCallback(const std::string& configurationtoken, boost::function<void(const GetConfigurationOptionsResponse&)> callback);
  Connection GetConfigurationsCallback(boost::function<void(const GetConfigurationsResponse&)> callback);
  Connection GetNodesCallback(boost::function<void(const GetNodesResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection SetConfigurationCallback(const PTZConfiguration& ptzconfiguration, bool forcepersistence, boost::function<void(const SetConfigurationResponse&)> callback);
  Connection StopCallback(const std::string& profiletoken, bool pantilt, bool zoom, boost::function<void(const StopResponse&)> callback);

  // Future requests
  boost::unique_future<ContinuousMoveResponse> ContinuousMoveFuture(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout);
  boost::unique_future<GetCompatibleConfigurationsResponse> GetCompatibleConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetConfigurationResponse> GetConfigurationFuture(const std::string& ptzconfigurationtoken);
  boost::unique_future<GetConfigurationOptionsResponse> GetConfigurationOptionsFuture(const std::string& configurationtoken);
  boost::unique_future<GetConfigurationsResponse> GetConfigurationsFuture();
  boost::unique_future<GetNodesResponse> GetNodesFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<SetConfigurationResponse> SetConfigurationFuture(const PTZConfiguration& ptzconfiguration, bool forcepersistence);
  boost::unique_future<StopResponse> StopFuture(const std::string& profiletoken, bool pantilt, bool zoom);

  // Boost Signals2
  boost::signals2::signal<void(const ContinuousMoveResponse&)>& ContinuousMoveSignal();
  boost::signals2::signal<void(const GetCompatibleConfigurationsResponse&)>& GetCompatibleConfigurationsSignal();
  boost::signals2::signal<void(const GetConfigurationResponse&)>& GetConfigurationSignal();
  boost::signals2::signal<void(const GetConfigurationOptionsResponse&)>& GetConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetConfigurationsResponse&)>& GetConfigurationsSignal();
  boost::signals2::signal<void(const GetNodesResponse&)>& GetNodesSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const SetConfigurationResponse&)>& SetConfigurationSignal();
  boost::signals2::signal<void(const StopResponse&)>& StopSignal();

 private:

  virtual void Update(PTZOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(PTZOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string ContinuousMoveBody(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout);
  std::string GetCompatibleConfigurationsBody(const std::string& profiletoken);
  std::string GetConfigurationBody(const std::string& ptzconfigurationtoken);
  std::string GetConfigurationOptionsBody(const std::string& configurationtoken);
  std::string GetConfigurationsBody();
  std::string GetNodesBody();
  std::string GetServiceCapabilitiesBody();
  std::string SetConfigurationBody(const PTZConfiguration& ptzconfiguration, bool forcepersistence);
  std::string StopBody(const std::string& profiletoken, bool pantilt, bool zoom);

  PTZSignals* signals_;

};

}

}

#endif
