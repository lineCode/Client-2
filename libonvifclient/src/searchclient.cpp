// searchclient.cpp
//

///// Includes /////

#include "onvifclient/searchclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace search
{

///// Classes /////

class SearchSignals
{
 public:

  std::unique_ptr< Signal<SEARCHOPERATION, SearchClient, GetServiceCapabilitiesResponse> > getservicecapabilities_;
    
};

///// Methods /////

SearchClient::SearchClient(const boost::shared_ptr<std::recursive_mutex>& mutex) :
  Client(mutex),
  signals_(nullptr)
{

}

SearchClient::~SearchClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

int SearchClient::Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse)
{
  signals_ = new SearchSignals(
  {
    std::make_unique< Signal<SEARCHOPERATION, SearchClient, GetServiceCapabilitiesResponse> >(shared_from_this(), SEARCHOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/search/wsdl/GetServiceCapabilities"), false)
  });

  return Client::Init(proxyparams, address, username, password, maxconcurrentrequests, forcehttpauthentication, forbidreuse);
}

void SearchClient::Destroy()
{
  Client::Destroy();

  if (signals_)
  {
    signals_->getservicecapabilities_->Destroy();
    delete signals_;
    signals_ = nullptr;
  }
}

// Requests
void SearchClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_->Create(GetServiceCapabilitiesBody());
}

// Callbacks
Connection SearchClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_->CreateCallback(GetServiceCapabilitiesBody(), callback);
}

// Futures
boost::unique_future<GetServiceCapabilitiesResponse> SearchClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_->CreateFuture(GetServiceCapabilitiesBody());
}

// Signals
boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& SearchClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_->GetSignal();
}

void SearchClient::Update(SEARCHOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case SEARCHOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(SEARCHOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      signals_->getservicecapabilities_->Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    default:
    {

      break;
    }
  }
}

void SearchClient::SignalError(SEARCHOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case SEARCHOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_->Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string SearchClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><search:GetServiceCapabilities/></s:Body>");
}

}

}
