// replayclient.cpp
//

///// Includes /////

#include "onvifclient/replayclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace replay
{

///// Classes /////

class ReplaySignals
{
 public:
   
  Signal<REPLAYOPERATION, ReplayClient, GetReplayUriResponse, StreamSetup, std::string> getreplayuri_;
  Signal<REPLAYOPERATION, ReplayClient, GetServiceCapabilitiesResponse> getservicecapabilities_;

};

///// Methods /////

ReplayClient::ReplayClient() :
  signals_(new ReplaySignals(
  {
    Signal<REPLAYOPERATION, ReplayClient, GetReplayUriResponse, StreamSetup, std::string>(this, REPLAYOPERATION_GETREPLAYURI, true, std::string("http://www.onvif.org/ver10/replay/wsdl/GetReplayUri"), false),
    Signal<REPLAYOPERATION, ReplayClient, GetServiceCapabilitiesResponse>(this, REPLAYOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/replay/wsdl/GetServiceCapabilities"), false)
  }))
{

}

ReplayClient::~ReplayClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void ReplayClient::Destroy()
{
  Client::Destroy();

  signals_->getreplayuri_.Destroy();
  signals_->getservicecapabilities_.Destroy();
}

// Requests
void ReplayClient::GetReplayUri(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken)
{
  signals_->getreplayuri_.Create(GetReplayUriBody(streamsetup, recordingtoken), streamsetup, recordingtoken);
}

void ReplayClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

// Callbacks
Connection ReplayClient::GetReplayUriCallback(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken, boost::function<void(const GetReplayUriResponse&)> callback)
{
  return signals_->getreplayuri_.CreateCallback(GetReplayUriBody(streamsetup, recordingtoken), callback, streamsetup, recordingtoken);
}

Connection ReplayClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

// Futures
boost::unique_future<GetReplayUriResponse> ReplayClient::GetReplayUriFuture(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken)
{
  return signals_->getreplayuri_.CreateFuture(GetReplayUriBody(streamsetup, recordingtoken), streamsetup, recordingtoken);
}

boost::unique_future<GetServiceCapabilitiesResponse> ReplayClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

// Signals
boost::signals2::signal<void(const GetReplayUriResponse&)>& ReplayClient::GetReplayUriSignal()
{
  return signals_->getreplayuri_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& ReplayClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

void ReplayClient::Update(REPLAYOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case REPLAYOPERATION_GETREPLAYURI:
    {
      auto getreplayuriresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReplayUriResponse']");
      if (!getreplayuriresponse)
      {
        SignalError(REPLAYOPERATION_GETREPLAYURI, handle, localendpoint, latency, std::string("/Envelope/Body/GetReplayUriResponse element not found"));
        break;
      }

      signals_->getreplayuri_.Emit(handle, localendpoint, latency, std::string(), GetText(getreplayuriresponse, "*[local-name()='Uri']"));
      break;
    }
    case REPLAYOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(REPLAYOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    default:
    {

      break;
    }
  }
}

void ReplayClient::SignalError(REPLAYOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case REPLAYOPERATION_GETREPLAYURI:
    {
      signals_->getreplayuri_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case REPLAYOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string ReplayClient::GetReplayUriBody(const onvif::StreamSetup& streamsetup, const std::string& recordingtoken)
{
  return std::string("<s:Body><trp1:GetReplayUri>" + streamsetup.ToXml("trp1:StreamSetup") + ToXml("trp1:RecordingToken", boost::optional<std::string>(recordingtoken)) + "</trp1:GetReplayUri></s:Body>");
}

std::string ReplayClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><trp1:GetServiceCapabilities/></s:Body>");
}

}

}
