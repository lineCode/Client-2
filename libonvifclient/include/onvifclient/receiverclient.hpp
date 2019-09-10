// receiverclient.hpp
//

#ifndef IDMP2IK0TBGPWAAV125JJNVNCLZHFGP2CB
#define IDMP2IK0TBGPWAAV125JJNVNCLZHFGP2CB

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Enumerations /////

enum RECEIVEROPERATION
{
  RECEIVEROPERATION_CONFIGURERECEIVER,
  RECEIVEROPERATION_CREATERECEIVER,
  RECEIVEROPERATION_DELETERECEIVER,
  RECEIVEROPERATION_GETRECEIVER,
  RECEIVEROPERATION_GETRECEIVERS,
  RECEIVEROPERATION_GETRECEIVERSTATE,
  RECEIVEROPERATION_GETSERVICECAPABILITIES,
  RECEIVEROPERATION_SETRECEIVERMODE
};

///// Declarations /////

class ReceiverClient;
class ReceiverSignals;

///// Classes /////

class ConfigureReceiverResponse : public Response<ReceiverClient>
{
 public:

  ConfigureReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const ReceiverConfiguration& configuration);
  virtual ~ConfigureReceiverResponse();

  std::string receivertoken_;
  ReceiverConfiguration configuration_;

};

class CreateReceiverResponse : public Response<ReceiverClient>
{
 public:

  CreateReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ReceiverConfiguration& configuration);
  CreateReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ReceiverConfiguration& configuration, const boost::optional<Receiver>& receiver);
  virtual ~CreateReceiverResponse();

  ReceiverConfiguration configuration_;

  boost::optional<Receiver> receiver_;

};

class DeleteReceiverResponse : public Response<ReceiverClient>
{
 public:

  DeleteReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken);
  virtual ~DeleteReceiverResponse();

  std::string receivertoken_;

};

class GetReceiverResponse : public Response<ReceiverClient>
{
 public:

  GetReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken);
  GetReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const boost::optional<Receiver>& receiver);
  virtual ~GetReceiverResponse();

  std::string receivertoken_;

  boost::optional<Receiver> receiver_;

};

class GetReceiversResponse : public Response<ReceiverClient>
{
 public:

  GetReceiversResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetReceiversResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Receiver>& receivers);
  virtual ~GetReceiversResponse();

  std::vector<Receiver> receivers_;

};

class GetReceiverStateResponse : public Response<ReceiverClient>
{
 public:

  GetReceiverStateResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken);
  GetReceiverStateResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const boost::optional<ReceiverStateInformation>& receiverstate);
  virtual ~GetReceiverStateResponse();

  std::string receivertoken_;

  boost::optional<ReceiverStateInformation> receiverstate_;

};

class GetServiceCapabilitiesResponse : public Response<ReceiverClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class SetReceiverModeResponse : public Response<ReceiverClient>
{
 public:

  SetReceiverModeResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const RECEIVERMODE mode);
  virtual ~SetReceiverModeResponse();
  
  std::string receivertoken_;
  RECEIVERMODE mode_;

};

class ReceiverClient : public Client<RECEIVEROPERATION>, public boost::enable_shared_from_this<ReceiverClient>
{
 public:
  
  using Client::Update;

  ReceiverClient();
  virtual ~ReceiverClient();

  virtual void Destroy() override;

  // Requests
  void ConfigureReceiver(const std::string& receivertoken, const ReceiverConfiguration& configuration);
  void CreateReceiver(const ReceiverConfiguration& configuration);
  void DeleteReceiver(const std::string& receivertoken);
  void GetReceiver(const std::string& receivertoken);
  void GetReceivers();
  void GetReceiverState(const std::string& receivertoken);
  void GetServiceCapabilities();
  void SetReceiverMode(const std::string& receivertoken, const RECEIVERMODE mode);
  
  // Callback requests
  Connection ConfigureReceiverCallback(const std::string& receivertoken, const ReceiverConfiguration& configuration, boost::function<void(const ConfigureReceiverResponse&)> callback);
  Connection CreateReceiverCallback(const ReceiverConfiguration& configuration, boost::function<void(const CreateReceiverResponse&)> callback);
  Connection DeleteReceiverCallback(const std::string& receivertoken, boost::function<void(const DeleteReceiverResponse&)> callback);
  Connection GetReceiverCallback(const std::string& receivertoken, boost::function<void(const GetReceiverResponse&)> callback);
  Connection GetReceiversCallback(boost::function<void(const GetReceiversResponse&)> callback);
  Connection GetReceiverStateCallback(const std::string& receivertoken, boost::function<void(const GetReceiverStateResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection SetReceiverModeCallback(const std::string& receivertoken, const RECEIVERMODE mode, boost::function<void(const SetReceiverModeResponse&)> callback);

  // Future requests
  boost::unique_future<ConfigureReceiverResponse> ConfigureReceiverFuture(const std::string& receivertoken, const ReceiverConfiguration& configuration);
  boost::unique_future<CreateReceiverResponse>  CreateReceiverFuture(const ReceiverConfiguration& configuration);
  boost::unique_future<DeleteReceiverResponse>  DeleteReceiverFuture(const std::string& receivertoken);
  boost::unique_future<GetReceiverResponse>  GetReceiverFuture(const std::string& receivertoken);
  boost::unique_future<GetReceiversResponse>  GetReceiversFuture();
  boost::unique_future<GetReceiverStateResponse>  GetReceiverStateFuture(const std::string& receivertoken);
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<SetReceiverModeResponse>  SetReceiverModeFuture(const std::string& receivertoken, const RECEIVERMODE mode);

  // Boost Signals2
  boost::signals2::signal<void(const ConfigureReceiverResponse&)>& ConfigureReceiverSignal();
  boost::signals2::signal<void(const CreateReceiverResponse&)>& CreateReceiverSignal();
  boost::signals2::signal<void(const DeleteReceiverResponse&)>& DeleteReceiverSignal();
  boost::signals2::signal<void(const GetReceiverResponse&)>& GetReceiverSignal();
  boost::signals2::signal<void(const GetReceiversResponse&)>& GetReceiversSignal();
  boost::signals2::signal<void(const GetReceiverStateResponse&)>& GetReceiverStateSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const SetReceiverModeResponse&)>& SetReceiverModeSignal();

 private:

  virtual void Update(RECEIVEROPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(RECEIVEROPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string ConfigureReceiverBody(const std::string& receivertoken, const ReceiverConfiguration& configuration);
  std::string CreateReceiverBody(const ReceiverConfiguration& configuration);
  std::string DeleteReceiverBody(const std::string& receivertoken);
  std::string GetReceiverBody(const std::string& receivertoken);
  std::string GetReceiversBody();
  std::string GetReceiverStateBody(const std::string& receivertoken);
  std::string GetServiceCapabilitiesBody();
  std::string SetReceiverModeBody(const std::string& receivertoken, const RECEIVERMODE mode);

  ReceiverSignals* signals_;

};

}

}

#endif
