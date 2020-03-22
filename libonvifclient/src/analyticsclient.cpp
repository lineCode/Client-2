// analyticsclient.cpp
//

///// Includes /////

#include "onvifclient/analyticsclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace analytics
{

///// Classes /////

class AnalyticsSignals
{
 public:

  Signal<ANALYTICSOPERATION, AnalyticsClient, GetServiceCapabilitiesResponse> getservicecapabilities_;

};

///// Methods /////

AnalyticsClient::AnalyticsClient(std::recursive_mutex& mutex) :
  Client(mutex),
  signals_(new AnalyticsSignals(
  {
    Signal<ANALYTICSOPERATION, AnalyticsClient, GetServiceCapabilitiesResponse>(this, ANALYTICSOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver20/analytics/wsdl/GetServiceCapabilities"), false)
  }))
{

}

AnalyticsClient::~AnalyticsClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void AnalyticsClient::Destroy()
{
  Client::Destroy();

  signals_->getservicecapabilities_.Destroy();
}

// Requests
void AnalyticsClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

// Callbacks
Connection AnalyticsClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

// Futures
boost::unique_future<GetServiceCapabilitiesResponse> AnalyticsClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

// Signals
boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& AnalyticsClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

void AnalyticsClient::Update(ANALYTICSOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case ANALYTICSOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(ANALYTICSOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
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

void AnalyticsClient::SignalError(ANALYTICSOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case ANALYTICSOPERATION_GETSERVICECAPABILITIES:
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

std::string AnalyticsClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><analytics:GetServiceCapabilities/></s:Body>");
}

}

}
