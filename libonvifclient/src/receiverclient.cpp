// receiverclient.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Classes /////

class ReceiverSignals
{
 public:
   
  Signal<RECEIVEROPERATION, ReceiverClient, ConfigureReceiverResponse, std::string, ReceiverConfiguration> configurereceiver_;
  Signal<RECEIVEROPERATION, ReceiverClient, CreateReceiverResponse, ReceiverConfiguration> createreceiver_;
  Signal<RECEIVEROPERATION, ReceiverClient, DeleteReceiverResponse, std::string> deletereceiver_;
  Signal<RECEIVEROPERATION, ReceiverClient, GetReceiverResponse, std::string> getreceiver_;
  Signal<RECEIVEROPERATION, ReceiverClient, GetReceiversResponse> getreceivers_;
  Signal<RECEIVEROPERATION, ReceiverClient, GetReceiverStateResponse, std::string> getreceiverstate_;
  Signal<RECEIVEROPERATION, ReceiverClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<RECEIVEROPERATION, ReceiverClient, SetReceiverModeResponse, std::string, RECEIVERMODE> setreceivermode_;

};

///// Methods /////

ReceiverClient::ReceiverClient(std::recursive_mutex& mutex) :
  Client(mutex),
  signals_(new ReceiverSignals(
  {
    Signal<RECEIVEROPERATION, ReceiverClient, ConfigureReceiverResponse, std::string, ReceiverConfiguration>(this, RECEIVEROPERATION_CONFIGURERECEIVER, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/ConfigureReceiver"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, CreateReceiverResponse, ReceiverConfiguration>(this, RECEIVEROPERATION_CREATERECEIVER, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/CreateReceiver"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, DeleteReceiverResponse, std::string>(this, RECEIVEROPERATION_DELETERECEIVER, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/DeleteReceiver"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, GetReceiverResponse, std::string>(this, RECEIVEROPERATION_GETRECEIVER, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/GetReceiver"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, GetReceiversResponse>(this, RECEIVEROPERATION_GETRECEIVERS, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/GetReceivers"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, GetReceiverStateResponse, std::string>(this, RECEIVEROPERATION_GETRECEIVERSTATE, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/GetReceiverState"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, GetServiceCapabilitiesResponse>(this, RECEIVEROPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/GetServiceCapabilities"), false),
    Signal<RECEIVEROPERATION, ReceiverClient, SetReceiverModeResponse, std::string, RECEIVERMODE>(this, RECEIVEROPERATION_SETRECEIVERMODE, true, std::string("http://www.onvif.org/ver10/receiver/wsdl/SetReceiverMode"), false)
  }))
{

}

ReceiverClient::~ReceiverClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void ReceiverClient::Destroy()
{
  Client::Destroy();

  signals_->configurereceiver_.Destroy();
  signals_->createreceiver_.Destroy();
  signals_->deletereceiver_.Destroy();
  signals_->getreceiver_.Destroy();
  signals_->getreceivers_.Destroy();
  signals_->getreceiverstate_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->setreceivermode_.Destroy();
}

// Requests
void ReceiverClient::ConfigureReceiver(const std::string& receivertoken, const ReceiverConfiguration& configuration)
{
  signals_->configurereceiver_.Create(ConfigureReceiverBody(receivertoken, configuration), receivertoken, configuration);
}

void ReceiverClient::CreateReceiver(const ReceiverConfiguration& configuration)
{
  signals_->createreceiver_.Create(CreateReceiverBody(configuration), configuration);
}

void ReceiverClient::DeleteReceiver(const std::string& receivertoken)
{
  signals_->deletereceiver_.Create(DeleteReceiverBody(receivertoken), receivertoken);
}

void ReceiverClient::GetReceiver(const std::string& receivertoken)
{
  signals_->getreceiver_.Create(GetReceiverBody(receivertoken), receivertoken);
}

void ReceiverClient::GetReceivers()
{
  signals_->getreceivers_.Create(GetReceiversBody());
}

void ReceiverClient::GetReceiverState(const std::string& receivertoken)
{
  signals_->getreceiverstate_.Create(GetReceiverStateBody(receivertoken), receivertoken);
}

void ReceiverClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void ReceiverClient::SetReceiverMode(const std::string& receivertoken, const RECEIVERMODE mode)
{
  signals_->setreceivermode_.Create(SetReceiverModeBody(receivertoken, mode), receivertoken, mode);
}

// Callbacks
Connection ReceiverClient::ConfigureReceiverCallback(const std::string& receivertoken, const ReceiverConfiguration& configuration, boost::function<void(const ConfigureReceiverResponse&)> callback)
{
  return signals_->configurereceiver_.CreateCallback(ConfigureReceiverBody(receivertoken, configuration), callback, receivertoken, configuration);
}

Connection ReceiverClient::CreateReceiverCallback(const ReceiverConfiguration& configuration, boost::function<void(const CreateReceiverResponse&)> callback)
{
  return signals_->createreceiver_.CreateCallback(CreateReceiverBody(configuration), callback, configuration);
}

Connection ReceiverClient::DeleteReceiverCallback(const std::string& receivertoken, boost::function<void(const DeleteReceiverResponse&)> callback)
{
  return signals_->deletereceiver_.CreateCallback(DeleteReceiverBody(receivertoken), callback, receivertoken);
}

Connection ReceiverClient::GetReceiverCallback(const std::string& receivertoken, boost::function<void(const GetReceiverResponse&)> callback)
{
  return signals_->getreceiver_.CreateCallback(GetReceiverBody(receivertoken), callback, receivertoken);
}

Connection ReceiverClient::GetReceiversCallback(boost::function<void(const GetReceiversResponse&)> callback)
{
  return signals_->getreceivers_.CreateCallback(GetReceiversBody(), callback);
}

Connection ReceiverClient::GetReceiverStateCallback(const std::string& receivertoken, boost::function<void(const GetReceiverStateResponse&)> callback)
{
  return signals_->getreceiverstate_.CreateCallback(GetReceiverStateBody(receivertoken), callback, receivertoken);
}

Connection ReceiverClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection ReceiverClient::SetReceiverModeCallback(const std::string& receivertoken, const RECEIVERMODE mode, boost::function<void(const SetReceiverModeResponse&)> callback)
{
  return signals_->setreceivermode_.CreateCallback(SetReceiverModeBody(receivertoken, mode), callback, receivertoken, mode);
}

// Futures
boost::unique_future<ConfigureReceiverResponse> ReceiverClient::ConfigureReceiverFuture(const std::string& receivertoken, const ReceiverConfiguration& configuration)
{
  return signals_->configurereceiver_.CreateFuture(ConfigureReceiverBody(receivertoken, configuration), receivertoken, configuration);
}

boost::unique_future<CreateReceiverResponse> ReceiverClient::CreateReceiverFuture(const ReceiverConfiguration& configuration)
{
  return signals_->createreceiver_.CreateFuture(CreateReceiverBody(configuration), configuration);
}

boost::unique_future<DeleteReceiverResponse> ReceiverClient::DeleteReceiverFuture(const std::string& receivertoken)
{
  return signals_->deletereceiver_.CreateFuture(DeleteReceiverBody(receivertoken), receivertoken);
}

boost::unique_future<GetReceiverResponse> ReceiverClient::GetReceiverFuture(const std::string& receivertoken)
{
  return signals_->getreceiver_.CreateFuture(GetReceiverBody(receivertoken), receivertoken);
}

boost::unique_future<GetReceiversResponse> ReceiverClient::GetReceiversFuture()
{
  return signals_->getreceivers_.CreateFuture(GetReceiversBody());
}

boost::unique_future<GetReceiverStateResponse> ReceiverClient::GetReceiverStateFuture(const std::string& receivertoken)
{
  return signals_->getreceiverstate_.CreateFuture(GetReceiverStateBody(receivertoken), receivertoken);
}

boost::unique_future<GetServiceCapabilitiesResponse> ReceiverClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<SetReceiverModeResponse> ReceiverClient::SetReceiverModeFuture(const std::string& receivertoken, const RECEIVERMODE mode)
{
  return signals_->setreceivermode_.CreateFuture(SetReceiverModeBody(receivertoken, mode), receivertoken, mode);
}

void ReceiverClient::Update(RECEIVEROPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case RECEIVEROPERATION_CONFIGURERECEIVER:
    {
      auto configurereceiverresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ConfigureReceiverResponse']");
      if (!configurereceiverresponse)
      {
        SignalError(RECEIVEROPERATION_CONFIGURERECEIVER, handle, localendpoint, latency, std::string("/Envelope/Body/ConfigureReceiverResponse element not found"));
        break;
      }
      
      signals_->configurereceiver_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECEIVEROPERATION_CREATERECEIVER:
    {
      auto createreceiverresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateReceiverResponse']");
      if (!createreceiverresponse)
      {
        SignalError(RECEIVEROPERATION_CREATERECEIVER, handle, localendpoint, latency, std::string("/Envelope/Body/CreateReceiverResponse element not found"));
        break;
      }
      
      signals_->createreceiver_.Emit(handle, localendpoint, latency, std::string(), GetClass<Receiver>(createreceiverresponse, "*[local-name()='Receiver']"));
      break;
    }
    case RECEIVEROPERATION_DELETERECEIVER:
    {
      auto deletereceiverresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteReceiverResponse']");
      if (!deletereceiverresponse)
      {
        SignalError(RECEIVEROPERATION_DELETERECEIVER, handle, localendpoint, latency, std::string("/Envelope/Body/DeleteReceiverResponse element not found"));
        break;
      }
      
      signals_->deletereceiver_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECEIVEROPERATION_GETRECEIVER:
    {
      auto getreceiverresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceiverResponse']");
      if (!getreceiverresponse)
      {
        SignalError(RECEIVEROPERATION_GETRECEIVER, handle, localendpoint, latency, std::string("/Envelope/Body/GetReceiverResponse element not found"));
        break;
      }
      
      signals_->getreceiver_.Emit(handle, localendpoint, latency, std::string(), GetClass<Receiver>(getreceiverresponse, "*[local-name()='Receiver']"));
      break;
    }
    case RECEIVEROPERATION_GETRECEIVERS:
    {
      auto getreceiversresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceiversResponse']");
      if (!getreceiversresponse)
      {
        SignalError(RECEIVEROPERATION_GETRECEIVERS, handle, localendpoint, latency, std::string("/Envelope/Body/GetReceiversResponse element not found"));
        break;
      }
      
      std::vector<Receiver> receivers;
      for (const auto& reveier : getreceiversresponse.node().select_nodes("*[local-name()='Receivers']"))
      {
        receivers.push_back(Receiver(reveier.node()));

      }

      signals_->getreceivers_.Emit(handle, localendpoint, latency, std::string(), receivers);
      break;
    }
    case RECEIVEROPERATION_GETRECEIVERSTATE:
    {
      auto getreceiverstateresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceiverStateResponse']");
      if (!getreceiverstateresponse)
      {
        SignalError(RECEIVEROPERATION_GETRECEIVERSTATE, handle, localendpoint, latency, std::string("/Envelope/Body/GetReceiverStateResponse element not found"));
        break;
      }
      
      signals_->getreceiverstate_.Emit(handle, localendpoint, latency, std::string(), GetClass<ReceiverStateInformation>(getreceiverstateresponse, "*[local-name()='ReceiverState']"));
      break;
    }
    case RECEIVEROPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(RECEIVEROPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }
      
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case RECEIVEROPERATION_SETRECEIVERMODE:
    {
      auto setreceivermoderesponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetReceiverModeResponse']");
      if (!setreceivermoderesponse)
      {
        SignalError(RECEIVEROPERATION_SETRECEIVERMODE, handle, localendpoint, latency, std::string("/Envelope/Body/SetReceiverModeResponse element not found"));
        break;
      }
      
      signals_->setreceivermode_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    default:
    {

      break;
    }
  }
}

// Signals
boost::signals2::signal<void(const ConfigureReceiverResponse&)>& ReceiverClient::ConfigureReceiverSignal()
{
  return signals_->configurereceiver_.GetSignal();
}

boost::signals2::signal<void(const CreateReceiverResponse&)>& ReceiverClient::CreateReceiverSignal()
{
  return signals_->createreceiver_.GetSignal();
}

boost::signals2::signal<void(const DeleteReceiverResponse&)>& ReceiverClient::DeleteReceiverSignal()
{
  return signals_->deletereceiver_.GetSignal();
}

boost::signals2::signal<void(const GetReceiverResponse&)>& ReceiverClient::GetReceiverSignal()
{
  return signals_->getreceiver_.GetSignal();
}

boost::signals2::signal<void(const GetReceiversResponse&)>& ReceiverClient::GetReceiversSignal()
{
  return signals_->getreceivers_.GetSignal();
}

boost::signals2::signal<void(const GetReceiverStateResponse&)>& ReceiverClient::GetReceiverStateSignal()
{
  return signals_->getreceiverstate_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& ReceiverClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const SetReceiverModeResponse&)>& ReceiverClient::SetReceiverModeSignal()
{
  return signals_->setreceivermode_.GetSignal();
}

void ReceiverClient::SignalError(RECEIVEROPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case RECEIVEROPERATION_CONFIGURERECEIVER:
    {
      signals_->configurereceiver_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_CREATERECEIVER:
    {
      signals_->createreceiver_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_DELETERECEIVER:
    {
      signals_->deletereceiver_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_GETRECEIVER:
    {
      signals_->getreceiver_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_GETRECEIVERS:
    {
      signals_->getreceivers_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_GETRECEIVERSTATE:
    {
      signals_->getreceiverstate_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECEIVEROPERATION_SETRECEIVERMODE:
    {
      signals_->setreceivermode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
      assert(false);
      break;
    }
  }
}

std::string ReceiverClient::ConfigureReceiverBody(const std::string& receivertoken, const ReceiverConfiguration& configuration)
{
  return std::string("<s:Body><trv:ConfigureReceiver><trv:ReceiverToken>") + receivertoken + std::string("</trv:ReceiverToken>") + configuration.ToXml("trv:Configuration") + std::string("</trv:ConfigureReceiver></s:Body>");
}

std::string ReceiverClient::CreateReceiverBody(const ReceiverConfiguration& configuration)
{
  return std::string("<s:Body><trv:CreateReceiver>") + configuration.ToXml("trv:Configuration") + std::string("</trv:CreateReceiver></s:Body>");
}

std::string ReceiverClient::DeleteReceiverBody(const std::string& receivertoken)
{
  return std::string("<s:Body><trv:DeleteReceiver><trv:ReceiverToken>") + receivertoken + std::string("</trv:ReceiverToken></trv:DeleteReceiver></s:Body>");
}

std::string ReceiverClient::GetReceiverBody(const std::string& receivertoken)
{
  return std::string("<s:Body><trv:GetReceiver><trv:ReceiverToken>") + receivertoken + std::string("</trv:ReceiverToken></trv:GetReceiver></s:Body>");
}

std::string ReceiverClient::GetReceiversBody()
{
  return std::string("<s:Body><trv:GetReceivers>") + std::string("</trv:GetReceivers></s:Body>");
}

std::string ReceiverClient::GetReceiverStateBody(const std::string& receivertoken)
{
  return std::string("<s:Body><trv:GetReceiverState><trv:ReceiverToken>") + receivertoken + std::string("</trv:ReceiverToken></trv:GetReceiverState></s:Body>");
}

std::string ReceiverClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><trv:GetServiceCapabilities>") + std::string("</trv:GetServiceCapabilities></s:Body>");
}

std::string ReceiverClient::SetReceiverModeBody(const std::string& receivertoken, const RECEIVERMODE mode)
{
  return std::string("<s:Body><trv:SetReceiverMode><trv:ReceiverToken>") + receivertoken + std::string("</trv:ReceiverToken><trv:Mode>") + ToString(mode) + std::string("</trv:Mode></trv:SetReceiverMode></s:Body>");
}

}

}
