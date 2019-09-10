// ptzclient.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Classes /////

class PTZSignals
{
 public:

  Signal<PTZOPERATION, PTZClient, ContinuousMoveResponse, std::string, PTZSpeed, Duration> continuousmove_;
  Signal<PTZOPERATION, PTZClient, GetCompatibleConfigurationsResponse, std::string> getcompatibleconfigurations_;
  Signal<PTZOPERATION, PTZClient, GetConfigurationResponse, std::string> getconfiguration_;
  Signal<PTZOPERATION, PTZClient, GetConfigurationOptionsResponse, std::string> getconfigurationoptions_;
  Signal<PTZOPERATION, PTZClient, GetConfigurationsResponse> getconfigurations_;
  Signal<PTZOPERATION, PTZClient, GetNodesResponse> getnodes_;
  Signal<PTZOPERATION, PTZClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<PTZOPERATION, PTZClient, SetConfigurationResponse, PTZConfiguration, bool> setconfiguration_;
  Signal<PTZOPERATION, PTZClient, StopResponse, std::string, bool, bool> stop_;

};

///// Methods /////

PTZClient::PTZClient() :
  signals_(new PTZSignals(
  {
    Signal<PTZOPERATION, PTZClient, ContinuousMoveResponse, std::string, PTZSpeed, Duration>(this, PTZOPERATION_CONTINUOUSMOVE, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/ContinuousMove"), false),
    Signal<PTZOPERATION, PTZClient, GetCompatibleConfigurationsResponse, std::string>(this, PTZOPERATION_GETCOMPATIBLECONFIGURATIONS, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetCompatibleConfigurations"), false),
    Signal<PTZOPERATION, PTZClient, GetConfigurationResponse, std::string>(this, PTZOPERATION_GETCONFIGURATION, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetConfiguration"), false),
    Signal<PTZOPERATION, PTZClient, GetConfigurationOptionsResponse, std::string>(this, PTZOPERATION_GETCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetConfigurationOptions"), false),
    Signal<PTZOPERATION, PTZClient, GetConfigurationsResponse>(this, PTZOPERATION_GETCONFIGURATIONS, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetConfigurations"), false),
    Signal<PTZOPERATION, PTZClient, GetNodesResponse>(this, PTZOPERATION_GETNODES, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetNodes"), false),
    Signal<PTZOPERATION, PTZClient, GetServiceCapabilitiesResponse>(this, PTZOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/GetServiceCapabilities"), false),
    Signal<PTZOPERATION, PTZClient, SetConfigurationResponse, PTZConfiguration, bool>(this, PTZOPERATION_SETCONFIGURATION, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/SetConfiguration"), false),
    Signal<PTZOPERATION, PTZClient, StopResponse, std::string, bool, bool>(this, PTZOPERATION_STOP, true, std::string("http://www.onvif.org/ver20/ptz/wsdl/Stop"), false)
  }))
{

}

PTZClient::~PTZClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void PTZClient::Destroy()
{
  Client::Destroy();

  signals_->continuousmove_.Destroy();
  signals_->getcompatibleconfigurations_.Destroy();
  signals_->getconfiguration_.Destroy();
  signals_->getconfigurationoptions_.Destroy();
  signals_->getconfigurations_.Destroy();
  signals_->getnodes_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->setconfiguration_.Destroy();
  signals_->stop_.Destroy();
}

// Requests
void PTZClient::ContinuousMove(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout)
{
  signals_->continuousmove_.Create(ContinuousMoveBody(profiletoken, velocity, timeout), profiletoken, velocity, timeout);
}

void PTZClient::GetCompatibleConfigurations(const std::string& profiletoken)
{
  signals_->getcompatibleconfigurations_.Create(GetCompatibleConfigurationsBody(profiletoken), profiletoken);
}

void PTZClient::GetConfiguration(const std::string& ptzconfigurationtoken)
{
  signals_->getconfiguration_.Create(GetConfigurationBody(ptzconfigurationtoken), ptzconfigurationtoken);
}

void PTZClient::GetConfigurationOptions(const std::string& configurationtoken)
{
  signals_->getconfigurationoptions_.Create(GetConfigurationOptionsBody(configurationtoken), configurationtoken);
}

void PTZClient::GetConfigurations()
{
  signals_->getconfigurations_.Create(GetConfigurationsBody());
}

void PTZClient::GetNodes()
{
  signals_->getnodes_.Create(GetNodesBody());
}

void PTZClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void PTZClient::SetConfiguration(const PTZConfiguration& ptzconfiguration, bool forcepersistence)
{
  signals_->setconfiguration_.Create(SetConfigurationBody(ptzconfiguration, forcepersistence), ptzconfiguration, forcepersistence);
}

void PTZClient::Stop(const std::string& profiletoken, bool pantilt, bool zoom)
{
  signals_->stop_.Create(StopBody(profiletoken, pantilt, zoom), profiletoken, pantilt, zoom);
}

// Callbacks
Connection PTZClient::ContinuousMoveCallback(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout, boost::function<void(const ContinuousMoveResponse&)> callback)
{
  return signals_->continuousmove_.CreateCallback(ContinuousMoveBody(profiletoken, velocity, timeout), callback, profiletoken, velocity, timeout);
}

Connection PTZClient::GetCompatibleConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleConfigurationsResponse&)> callback)
{
  return signals_->getcompatibleconfigurations_.CreateCallback(GetCompatibleConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection PTZClient::GetConfigurationCallback(const std::string& ptzconfigurationtoken, boost::function<void(const GetConfigurationResponse&)> callback)
{
  return signals_->getconfiguration_.CreateCallback(GetConfigurationBody(ptzconfigurationtoken), callback, ptzconfigurationtoken);
}

Connection PTZClient::GetConfigurationOptionsCallback(const std::string& configurationtoken, boost::function<void(const GetConfigurationOptionsResponse&)> callback)
{
  return signals_->getconfigurationoptions_.CreateCallback(GetConfigurationOptionsBody(configurationtoken), callback, configurationtoken);
}

Connection PTZClient::GetConfigurationsCallback(boost::function<void(const GetConfigurationsResponse&)> callback)
{
  return signals_->getconfigurations_.CreateCallback(GetConfigurationsBody(), callback);
}

Connection PTZClient::GetNodesCallback(boost::function<void(const GetNodesResponse&)> callback)
{
  return signals_->getnodes_.CreateCallback(GetNodesBody(), callback);
}

Connection PTZClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection PTZClient::SetConfigurationCallback(const PTZConfiguration& ptzconfiguration, bool forcepersistence, boost::function<void(const SetConfigurationResponse&)> callback)
{
  return signals_->setconfiguration_.CreateCallback(SetConfigurationBody(ptzconfiguration, forcepersistence), callback, ptzconfiguration, forcepersistence);
}

Connection PTZClient::StopCallback(const std::string& profiletoken, bool pantilt, bool zoom, boost::function<void(const StopResponse&)> callback)
{
  return signals_->stop_.CreateCallback(StopBody(profiletoken, pantilt, zoom), callback, profiletoken, pantilt, zoom);
}

// Futures
boost::unique_future<ContinuousMoveResponse> PTZClient::ContinuousMoveFuture(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout)
{
  return signals_->continuousmove_.CreateFuture(ContinuousMoveBody(profiletoken, velocity, timeout), profiletoken, velocity, timeout);
}

boost::unique_future<GetCompatibleConfigurationsResponse> PTZClient::GetCompatibleConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatibleconfigurations_.CreateFuture(GetCompatibleConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetConfigurationResponse> PTZClient::GetConfigurationFuture(const std::string& ptzconfigurationtoken)
{
  return signals_->getconfiguration_.CreateFuture(GetConfigurationBody(ptzconfigurationtoken), ptzconfigurationtoken);
}

boost::unique_future<GetConfigurationOptionsResponse> PTZClient::GetConfigurationOptionsFuture(const std::string& configurationtoken)
{
  return signals_->getconfigurationoptions_.CreateFuture(GetConfigurationOptionsBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetConfigurationsResponse> PTZClient::GetConfigurationsFuture()
{
  return signals_->getconfigurations_.CreateFuture(GetConfigurationsBody());
}

boost::unique_future<GetNodesResponse> PTZClient::GetNodesFuture()
{
  return signals_->getnodes_.CreateFuture(GetNodesBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> PTZClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<SetConfigurationResponse> PTZClient::SetConfigurationFuture(const PTZConfiguration& ptzconfiguration, bool forcepersistence)
{
  return signals_->setconfiguration_.CreateFuture(SetConfigurationBody(ptzconfiguration, forcepersistence), ptzconfiguration, forcepersistence);
}

boost::unique_future<StopResponse> PTZClient::StopFuture(const std::string& profiletoken, bool pantilt, bool zoom)
{
  return signals_->stop_.CreateFuture(StopBody(profiletoken, pantilt, zoom), profiletoken, pantilt, zoom);
}

// Signals
boost::signals2::signal<void(const ContinuousMoveResponse&)>& PTZClient::ContinuousMoveSignal()
{
  return signals_->continuousmove_.GetSignal();
}

boost::signals2::signal<void(const GetCompatibleConfigurationsResponse&)>& PTZClient::GetCompatibleConfigurationsSignal()
{
  return signals_->getcompatibleconfigurations_.GetSignal();
}

boost::signals2::signal<void(const GetConfigurationResponse&)>& PTZClient::GetConfigurationSignal()
{
  return signals_->getconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetConfigurationOptionsResponse&)>& PTZClient::GetConfigurationOptionsSignal()
{
  return signals_->getconfigurationoptions_.GetSignal();
}

boost::signals2::signal<void(const GetConfigurationsResponse&)>& PTZClient::GetConfigurationsSignal()
{
  return signals_->getconfigurations_.GetSignal();
}

boost::signals2::signal<void(const GetNodesResponse&)>& PTZClient::GetNodesSignal()
{
  return signals_->getnodes_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& PTZClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const SetConfigurationResponse&)>& PTZClient::SetConfigurationSignal()
{
  return signals_->setconfiguration_.GetSignal();
}

boost::signals2::signal<void(const StopResponse&)>& PTZClient::StopSignal()
{
  return signals_->stop_.GetSignal();
}

void PTZClient::Update(PTZOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case PTZOPERATION_CONTINUOUSMOVE:
    {
      auto continuousmoveresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ContinuousMoveResponse']");
      if (!continuousmoveresponse)
      {
        SignalError(PTZOPERATION_CONTINUOUSMOVE, handle, localendpoint, latency, std::string("/Envelope/Body/ContinuousMoveResponse element not found"));
        break;
      }

      signals_->continuousmove_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case PTZOPERATION_GETCOMPATIBLECONFIGURATIONS:
    {
      auto getcompatibleconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleConfigurationsResponse']");
      if (!getcompatibleconfigurationsresponse)
      {
        SignalError(PTZOPERATION_GETCOMPATIBLECONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleConfigurationsResponse element not found"));
        break;
      }

      std::vector<PTZConfiguration> ptzconfigurations;
      for (const auto& ptzconfiguration : getcompatibleconfigurationsresponse.node().select_nodes("*[local-name()='PTZConfiguration']"))
      {
        ptzconfigurations.push_back(PTZConfiguration(ptzconfiguration.node()));

      }

      signals_->getcompatibleconfigurations_.Emit(handle, localendpoint, latency, std::string(), ptzconfigurations);
      break;
    }
    case PTZOPERATION_GETCONFIGURATION:
    {
      auto getconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetConfigurationResponse']");
      if (!getconfigurationresponse)
      {
        SignalError(PTZOPERATION_GETCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetConfigurationResponse element not found"));
        break;
      }

      signals_->getconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<PTZConfiguration>(getconfigurationresponse, "*[local-name()='PTZConfiguration']"));
      break;
    }
    case PTZOPERATION_GETCONFIGURATIONOPTIONS:
    {
      auto getconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetConfigurationOptionsResponse']");
      if (!getconfigurationoptionsresponse)
      {
        SignalError(PTZOPERATION_GETCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getconfigurationoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<PTZConfigurationOptions>(getconfigurationoptionsresponse, "*[local-name()='PTZConfigurationOptions']"));
      break;
    }
    case PTZOPERATION_GETCONFIGURATIONS:
    {
      auto getconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetConfigurationsResponse']");
      if (!getconfigurationsresponse)
      {
        SignalError(PTZOPERATION_GETCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetConfigurationsResponse element not found"));
        break;
      }

      std::vector<PTZConfiguration> ptzconfigurations;
      for (const auto& ptzconfiguration : getconfigurationsresponse.node().select_nodes("*[local-name()='PTZConfiguration']"))
      {
        ptzconfigurations.push_back(PTZConfiguration(ptzconfiguration.node()));

      }

      signals_->getconfigurations_.Emit(handle, localendpoint, latency, std::string(), ptzconfigurations);
      break;
    }
    case PTZOPERATION_GETNODES:
    {
      auto getnodesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNodesResponse']");
      if (!getnodesresponse)
      {
        SignalError(PTZOPERATION_GETNODES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      std::vector<PTZNode> ptznodes;
      for (const auto& ptznode : getnodesresponse.node().select_nodes("*[local-name()='PTZNode']"))
      {
        ptznodes.push_back(PTZNode(ptznode.node()));

      }

      signals_->getnodes_.Emit(handle, localendpoint, latency, std::string(), ptznodes);
      break;
    }
    case PTZOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(PTZOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case PTZOPERATION_SETCONFIGURATION:
    {
      auto setconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetConfigurationResponse']");
      if (!setconfigurationresponse)
      {
        SignalError(PTZOPERATION_SETCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetConfigurationResponse element not found"));
        break;
      }

      signals_->setconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case PTZOPERATION_STOP:
    {
      auto stopresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StopResponse']");
      if (!stopresponse)
      {
        SignalError(PTZOPERATION_STOP, handle, localendpoint, latency, std::string("/Envelope/Body/StopResponse element not found"));
        break;
      }

      signals_->stop_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    default:
    {

      break;
    }
  }
}

void PTZClient::SignalError(PTZOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case PTZOPERATION_CONTINUOUSMOVE:
    {
      signals_->continuousmove_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETCOMPATIBLECONFIGURATIONS:
    {
      signals_->getcompatibleconfigurations_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETCONFIGURATION:
    {
      signals_->getconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETCONFIGURATIONOPTIONS:
    {
      signals_->getconfigurationoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETCONFIGURATIONS:
    {
      signals_->getconfigurations_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETNODES:
    {
      signals_->getnodes_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_SETCONFIGURATION:
    {
      signals_->setconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case PTZOPERATION_STOP:
    {
      signals_->stop_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string PTZClient::ContinuousMoveBody(const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout)
{
  return std::string("<s:Body><ptz:ContinuousMove><ptz:ProfileToken>") + profiletoken + std::string("</ptz:ProfileToken>") + velocity.ToXml("ptz:Velocity") + onvif::ToXml("ptz:Timeout", boost::optional<std::string>(timeout.ToString())) + std::string("</ptz:ContinuousMove></s:Body>");
}

std::string PTZClient::GetCompatibleConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleConfigurations></s:Body>");
}

std::string PTZClient::GetConfigurationBody(const std::string& ptzconfigurationtoken)
{
  return std::string("<s:Body><ptz:GetConfiguration><tt:PTZConfigurationToken>") + ptzconfigurationtoken + std::string("</tt:PTZConfigurationToken></ptz:GetConfiguration></s:Body>");
}

std::string PTZClient::GetConfigurationOptionsBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><ptz:GetConfigurationOptions><tt:ConfigurationToken>") + configurationtoken + std::string("</tt:ConfigurationToken></ptz:GetConfigurationOptions></s:Body>");
}

std::string PTZClient::GetConfigurationsBody()
{
  return std::string("<s:Body><ptz:GetConfigurations/></s:Body>");
}

std::string PTZClient::GetNodesBody()
{
  return std::string("<s:Body><ptz:GetNodes/></s:Body>");
}

std::string PTZClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><ptz:GetServiceCapabilities/></s:Body>");
}

std::string PTZClient::SetConfigurationBody(const PTZConfiguration& ptzconfiguration, bool forcepersistence)
{
  return (std::string("<s:Body><ptz:SetConfiguration>") + ptzconfiguration.ToXml("tt:PTZConfiguration") + ToXml("tt:ForcePersistence", boost::optional<bool>(forcepersistence)) + std::string("</ptz:SetConfiguration></s:Body>"));
}

std::string PTZClient::StopBody(const std::string& profiletoken, bool pantilt, bool zoom)
{
  return (std::string("<s:Body><ptz:Stop><tt:ProfileToken>") + profiletoken + std::string("</tt:ProfileToken>") + ToXml("tt:PanTilt", boost::optional<bool>(pantilt)) + ToXml("tt:Zoom", boost::optional<bool>(zoom)) + std::string("</ptz:Stop></s:Body>"));
}

}

}
