// imagingclient.cpp
//

///// Includes /////

#include "onvifclient/imagingclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Classes /////

class ImagingSignals
{
 public:

  Signal<IMAGINGOPERATION, ImagingClient, GetImagingSettingsResponse, std::string> getimagingsettings_;
  Signal<IMAGINGOPERATION, ImagingClient, GetOptionsResponse, std::string> getoptions_;
  Signal<IMAGINGOPERATION, ImagingClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<IMAGINGOPERATION, ImagingClient, SetImagingSettingsResponse, std::string, ImagingSettings20, bool> setimagingsettings_;
};

///// Methods /////

ImagingClient::ImagingClient(std::recursive_mutex& mutex) :
  Client(mutex),
  signals_(new ImagingSignals(
  {
    Signal<IMAGINGOPERATION, ImagingClient, GetImagingSettingsResponse, std::string>(this, IMAGINGOPERATION_GETIMAGINGSETTINGS, true, std::string("http://www.onvif.org/ver20/imaging/wsdl/GetImagingSettings"), false),
    Signal<IMAGINGOPERATION, ImagingClient, GetOptionsResponse, std::string>(this, IMAGINGOPERATION_GETOPTIONS, true, std::string("http://www.onvif.org/ver20/imaging/wsdl/GetOptions"), false),
    Signal<IMAGINGOPERATION, ImagingClient, GetServiceCapabilitiesResponse>(this, IMAGINGOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver20/imaging/wsdl/GetServiceCapabilities"), false),
      Signal<IMAGINGOPERATION, ImagingClient, SetImagingSettingsResponse, std::string, ImagingSettings20, bool>(this, IMAGINGOPERATION_SETIMAGINGSETTINGS, true, std::string("http://www.onvif.org/ver20/imaging/wsdl/SetImagingSettings"), false)
  }))
{
  
}

ImagingClient::~ImagingClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void ImagingClient::Destroy()
{
  Client::Destroy();

  signals_->getimagingsettings_.Destroy();
  signals_->getoptions_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->setimagingsettings_.Destroy();
}

// Requests
void ImagingClient::GetImagingSettings(const std::string& videosourcetoken)
{
  signals_->getimagingsettings_.Create(GetImagingSettingsBody(videosourcetoken), videosourcetoken);
}

void ImagingClient::GetOptions(const std::string& videosourcetoken)
{
  signals_->getoptions_.Create(GetOptionsBody(videosourcetoken), videosourcetoken);
}

void ImagingClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void ImagingClient::SetImagingSettings(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence)
{
  signals_->setimagingsettings_.Create(SetImagingSettingsBody(videosourcetoken, imagingsettings, forcepersistence), videosourcetoken, imagingsettings, forcepersistence);
}

// Callbacks
Connection ImagingClient::GetImagingSettingsCallback(const std::string& videosourcetoken, boost::function<void(const GetImagingSettingsResponse&)> callback)
{
  return signals_->getimagingsettings_.CreateCallback(GetImagingSettingsBody(videosourcetoken), callback, videosourcetoken);
}

Connection ImagingClient::GetOptionsCallback(const std::string& videosourcetoken, boost::function<void(const GetOptionsResponse&)> callback)
{
  return signals_->getoptions_.CreateCallback(GetOptionsBody(videosourcetoken), callback, videosourcetoken);
}

Connection ImagingClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection ImagingClient::SetImagingSettingsCallback(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence, boost::function<void(const SetImagingSettingsResponse&)> callback)
{
  return signals_->setimagingsettings_.CreateCallback(SetImagingSettingsBody(videosourcetoken, imagingsettings, forcepersistence), callback, videosourcetoken, imagingsettings, forcepersistence);
}

// Futures
boost::unique_future<GetOptionsResponse> ImagingClient::GetOptionsFuture(const std::string& videosourcetoken)
{
  return signals_->getoptions_.CreateFuture(GetOptionsBody(videosourcetoken), videosourcetoken);
}

boost::unique_future<GetImagingSettingsResponse> ImagingClient::GetImagingSettingsFuture(const std::string& videosourcetoken)
{
  return signals_->getimagingsettings_.CreateFuture(GetImagingSettingsBody(videosourcetoken), videosourcetoken);
}

boost::unique_future<GetServiceCapabilitiesResponse> ImagingClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<SetImagingSettingsResponse> ImagingClient::SetImagingSettingsFuture(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence)
{
  return signals_->setimagingsettings_.CreateFuture(SetImagingSettingsBody(videosourcetoken, imagingsettings, forcepersistence), videosourcetoken, imagingsettings, forcepersistence);
}

// Signals
boost::signals2::signal<void(const GetImagingSettingsResponse&)>& ImagingClient::GetImagingSettingsSignal(const std::string& videosourcetoken)
{
  return signals_->getimagingsettings_.GetSignal();
}

boost::signals2::signal<void(const GetOptionsResponse&)>& ImagingClient::GetOptionsSignal(const std::string& videosourcetoken)
{
  return signals_->getoptions_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& ImagingClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const SetImagingSettingsResponse&)>& ImagingClient::SetImagingSettingsSignal(const std::string& videosourcetoken)
{
  return signals_->setimagingsettings_.GetSignal();
}

void ImagingClient::Update(IMAGINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case IMAGINGOPERATION_GETIMAGINGSETTINGS:
    {
      auto getimagingsettingsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetImagingSettingsResponse']");
      if (!getimagingsettingsresponse)
      {
        SignalError(IMAGINGOPERATION_GETIMAGINGSETTINGS, handle, localendpoint, latency, std::string("/Envelope/Body/GetImagingSettingsResponse element not found"));
        break;
      }
      
      signals_->getimagingsettings_.Emit(handle, localendpoint, latency, std::string(), GetClass<ImagingSettings20>(getimagingsettingsresponse, "*[local-name()='ImagingSettings']"));
      break;
    }
    case IMAGINGOPERATION_GETOPTIONS:
    {
      auto getoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetOptionsResponse']");
      if (!getoptionsresponse)
      {
        SignalError(IMAGINGOPERATION_GETOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetOptionsResponse element not found"));
        break;
      }
      
      signals_->getoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<ImagingOptions20>(getoptionsresponse, "*[local-name()='ImagingOptions']"));
      break;
    }
    case IMAGINGOPERATION_GETSERVICECAPABILITIES:
    {
      auto getserviecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getserviecapabilitiesresponse)
      {
        SignalError(IMAGINGOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }
      
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getserviecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case IMAGINGOPERATION_SETIMAGINGSETTINGS:
    {
      auto setimagingsettingsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetImagingSettingsResponse']");
      if (!setimagingsettingsresponse)
      {
        SignalError(IMAGINGOPERATION_SETIMAGINGSETTINGS, handle, localendpoint, latency, std::string("/Envelope/Body/GetImagingSettingsResponse element not found"));
        break;
      }

      signals_->setimagingsettings_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    default:
    {

      break;
    }
  }
}

void ImagingClient::SignalError(IMAGINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case IMAGINGOPERATION_GETIMAGINGSETTINGS:
    {
      signals_->getimagingsettings_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case IMAGINGOPERATION_GETOPTIONS:
    {
      signals_->getoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case IMAGINGOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case IMAGINGOPERATION_SETIMAGINGSETTINGS:
    {
      signals_->setimagingsettings_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
      assert(false);
      break;
    }
  }
}

std::string ImagingClient::GetImagingSettingsBody(const std::string& videosourcetoken)
{
  return std::string("<s:Body><timg:GetImagingSettings><timg:VideoSourceToken>" + videosourcetoken + "</timg:VideoSourceToken></timg:GetImagingSettings></s:Body>");
}

std::string ImagingClient::GetOptionsBody(const std::string& videosourcetoken)
{
  return std::string("<s:Body><timg:GetOptions><timg:VideoSourceToken>" + videosourcetoken + "</timg:VideoSourceToken></timg:GetOptions></s:Body>");
}

std::string ImagingClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><timg:GetServiceCapabilities/></s:Body>");
}

std::string ImagingClient::SetImagingSettingsBody(const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence)
{
  return std::string("<s:Body><timg:SetImagingSettings><timg:VideoSourceToken>" + videosourcetoken + "</timg:VideoSourceToken>" + imagingsettings.ToXml("timg:ImagingSettings") + onvif::ToXml("timg:ForcePersistence", boost::optional<bool>(forcepersistence)) + "</timg:SetImagingSettings></s:Body>");
}

}

}
