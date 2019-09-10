// wsdiscoverclient.cpp
//

///// Includes /////

#include "wsdiscover/wsdiscoverclient.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <utility/utility.hpp>
#include <thread>
#include <pugixml.hpp>

#ifdef _WIN32
#include <windows.h>
#include <wsdapi.h>
#endif

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

///// Classes /////

#ifdef _WIN32
class NotificationSink : public IWSDiscoveryProviderNotify
{
 public:

  NotificationSink(const boost::shared_ptr<WsDiscoverClient>& wsdiscoverclient);
  ~NotificationSink();

  HRESULT STDMETHODCALLTYPE Add(IWSDiscoveredService* service); // Hello
  HRESULT STDMETHODCALLTYPE Remove(IWSDiscoveredService* service); // Bye
  HRESULT STDMETHODCALLTYPE SearchFailed(HRESULT hr, LPCWSTR tag);
  HRESULT STDMETHODCALLTYPE SearchComplete(LPCWSTR tag);
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR* __RPC_FAR* object);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();

 private:

  bool IsNVT(WSD_NAME_LIST* types) const;

  std::recursive_mutex mutex_;

  boost::shared_ptr<WsDiscoverClient> wsdiscoverclient_;
  ULONG ref_; // IUnknown reference counter

};
#endif

///// Methods /////

#ifdef _WIN32
NotificationSink::NotificationSink(const boost::shared_ptr<WsDiscoverClient>& wsdiscoverclient) :
  wsdiscoverclient_(wsdiscoverclient),
  ref_(1)
{

}

NotificationSink::~NotificationSink()
{

}

HRESULT STDMETHODCALLTYPE NotificationSink::Add(IWSDiscoveredService* service)
{
  if (service == nullptr)
  {

    return E_INVALIDARG;
  }

  std::lock_guard<std::recursive_mutex> lock(mutex_);

  // Get the types and ignore anything that isn't a network video transmitter
  WSD_NAME_LIST* types = nullptr; // Do not deallocate
  HRESULT hr = service->GetTypes(&types);
  if (FAILED(hr))
  {

    return hr;
  }

  if (!IsNVT(types)) // Ignore everything that isn't a NVT
  {

    return S_OK;
  }

  // Collect the XAddrs
  std::vector<std::string> xaddrs;
  WSD_URI_LIST* uris = nullptr; // Do not deallocate
  hr = service->GetXAddrs(&uris);
  if (FAILED(hr))
  {

    return hr;
  }

  while (uris)
  {
    xaddrs.push_back(utility::ToString(uris->Element));
    uris = uris->Next;
  }

  // Collect the scopes
  std::vector<std::string> scopes;
  uris = nullptr; // Do not deallocate
  hr = service->GetScopes(&uris);
  if (FAILED(hr))
  {

    return hr;
  }

  while (uris)
  {
    scopes.push_back(utility::ToString(uris->Element));
    uris = uris->Next;
  }

  wsdiscoverclient_->hello_(xaddrs, scopes);

  return S_OK;
}

HRESULT STDMETHODCALLTYPE NotificationSink::Remove(IWSDiscoveredService* service)
{
  if (service == nullptr)
  {

    return E_INVALIDARG;
  }

  return S_OK;
}

HRESULT STDMETHODCALLTYPE NotificationSink::SearchFailed(HRESULT hr, LPCWSTR tag)
{

  return S_OK;
}

HRESULT STDMETHODCALLTYPE NotificationSink::SearchComplete(LPCWSTR tag)
{

  return S_OK;
}

HRESULT STDMETHODCALLTYPE NotificationSink::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR* __RPC_FAR* object)
{
  if (object == nullptr)
  {

    return E_POINTER;
  }

  *object = nullptr;
  if (__uuidof(IWSDiscoveryProviderNotify) == riid)
  {
    *object = static_cast<IWSDiscoveryProviderNotify *>(this);

  }
  else if (__uuidof(IUnknown) == riid)
  {
    *object = static_cast<IUnknown *>(this);

  }
  else
  {

    return E_NOINTERFACE;
  }

  ((LPUNKNOWN)*object)->AddRef();

  return S_OK;
}

ULONG STDMETHODCALLTYPE NotificationSink::AddRef()
{

  return (ULONG)InterlockedIncrement((LONG*)&ref_);
}

ULONG STDMETHODCALLTYPE NotificationSink::Release()
{
  ULONG newRefCount = (ULONG)InterlockedDecrement((LONG*)&ref_);
  if (0 == newRefCount)
  {
    delete this;

  }
  return newRefCount;
}

bool NotificationSink::IsNVT(WSD_NAME_LIST* types) const
{
  while (types)
  {
    if (types->Element && types->Element->LocalName && types->Element->Space && types->Element->Space->Uri)
    {
      if ((wcscmp(types->Element->Space->Uri, L"http://www.onvif.org/ver10/network/wsdl") == 0) && (wcscmp(types->Element->LocalName, L"NetworkVideoTransmitter") == 0))
      {

        return true;
      }
    }
    types = types->Next;
  }
  return false;
}


#endif

WsDiscoverClient::WsDiscoverClient(boost::asio::io_service& io, unsigned short port) :
  broadcast_(boost::asio::ip::address_v4::from_string("239.255.255.250"), port),
#ifdef _WIN32
  provider_(nullptr),
  sink_(nullptr),
  notify_(nullptr),
  wsdapi_(false),
#endif
  socketv4_(io)
{

}

WsDiscoverClient::~WsDiscoverClient()
{
  Destroy();

}

int WsDiscoverClient::Init()
{
  Destroy();

  std::lock_guard<std::recursive_mutex> lock(mutex_);
#ifdef _WIN32
  // First we try the Windows WSDAPI
  if (InitWindows() == 0)
  {
    wsdapi_ = true;
    return 0;
  }
  wsdapi_ = false;
#endif
  // Now we try the manual method because Windows failed to initialise properly(or we are not on Windows
  boost::system::error_code err;
  if(socketv4_.open(broadcast_.protocol(), err))
  {

    return 1;
  }

  if (socketv4_.set_option(boost::asio::ip::udp::socket::reuse_address(true), err))
  {

    return 2;
  }

  if (socketv4_.set_option(boost::asio::ip::multicast::enable_loopback(true), err))
  {

    return 3;
  }

  if (socketv4_.set_option(boost::asio::ip::multicast::join_group(boost::asio::ip::address_v4::from_string("239.255.255.250")), err))
  {

    return 5;
  }
  
  socketv4_.async_receive_from(boost::asio::buffer(buffer_), endpointv4_, boost::bind(&WsDiscoverClient::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

  return 0;
}

int WsDiscoverClient::Destroy()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
#ifdef _WIN32
  DestroyWindows();
  wsdapi_ = false;
#endif

  if(socketv4_.is_open())
  {
    boost::system::error_code err;
    if(socketv4_.close(err))
    {

      return 1;
    }
  }
  return 0;
}

int WsDiscoverClient::Broadcast()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);

#ifdef _WIN32
  if (wsdapi_)
  {
    if (provider_ == nullptr)
    {

      return 1;
    }

    const std::wstring tag = utility::GenerateRandomWString(16);
    if (FAILED(provider_->SearchByType(nullptr, nullptr, nullptr, tag.data()))) // We could provide the types list here for NVT, but easier to just accept all
    {
      DestroyWindows();
      return 2;
    }
  }
  else
#endif
  {
    const std::string probe = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\"><s:Header><a:Action s:mustUnderstand=\"1\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</a:Action><a:MessageID>uuid:" + boost::lexical_cast<std::string>(randomgenerator_()) + "</a:MessageID><a:ReplyTo><a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address></a:ReplyTo><a:To s:mustUnderstand=\"1\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</a:To></s:Header><s:Body><Probe xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><d:Types xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dp0=\"http://www.onvif.org/ver10/network/wsdl\">dp0:NetworkVideoTransmitter</d:Types></Probe></s:Body></s:Envelope>";
    boost::system::error_code err;
    socketv4_.send_to(boost::asio::buffer(probe), broadcast_, 0, err);
    if (err)
    {

      return 1;
    }
  }
  return 0;
}

void WsDiscoverClient::HandleReceive(const boost::system::error_code& error, const size_t receivedbytes)
{
  if (error)
  {

    return;
  }

  Parse(receivedbytes);
  
  socketv4_.async_receive_from(boost::asio::buffer(buffer_), endpointv4_, boost::bind(&WsDiscoverClient::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

#ifdef _WIN32
int WsDiscoverClient::InitWindows()
{
  if (FAILED(WSDCreateDiscoveryProvider(nullptr, &provider_)))
  {
    DestroyWindows();
    return 1;
  }

  sink_ = new NotificationSink(shared_from_this());

  if (FAILED(sink_->QueryInterface(__uuidof(IWSDiscoveryProviderNotify), reinterpret_cast<void**>(&notify_))))
  {
    DestroyWindows();
    return 2;
  }

  if (notify_ == nullptr)
  {
    DestroyWindows();
    return 3;
  }

  if (FAILED(provider_->Attach(notify_)))
  {
    DestroyWindows();
    return 4;
  }

  return 0;
}

void WsDiscoverClient::DestroyWindows()
{
  if (notify_)
  {
    notify_->Release();
    notify_ = nullptr;
  }

  if (sink_)
  {
    sink_->Release();
    sink_ = nullptr;
  }

  if (provider_)
  {
    provider_->Release();
    provider_ = nullptr;
  }
}

#endif

void WsDiscoverClient::Parse(const size_t receivedbytes)
{
  // Parse the data
  pugi::xml_document xml;
  std::string test(buffer_.data(), receivedbytes);
  std::memset(buffer_.data(), 0, receivedbytes);
  if (xml.load(test.c_str()))
  {
    // Check this is a SOAP envelope
    const auto envelope = xml.select_single_node("/*[local-name()='Envelope']");
    if (!envelope.node())
    {

      return;
    }

    // Get the uuid node and compare it against what was sent
    /*const auto relatestonode = envelope.node().select_single_node("*[local-name()='Header']/*[local-name()='RelatesTo']");
    if(!relatestonode.node())
    {

      return;
    }*/

    // Get the Scopes
    const auto scopesnode = envelope.node().select_single_node("*[local-name()='Body']//*[local-name()='Scopes']");
    if (!scopesnode.node())
    {

      return;
    }

    // Get the XAddrs
    const auto addrsnode = envelope.node().select_single_node("*[local-name()='Body']//*[local-name()='XAddrs']");
    if (!addrsnode.node())
    {

      return;
    }

    // Invoke listeners
    std::vector<std::string> addresses;
    std::vector<std::string> scopes;
    const std::string addressesinput(scopesnode.node().text().get());
    const std::string scopesinput(addrsnode.node().text().get());
    boost::algorithm::split(scopes, addressesinput, boost::is_any_of("\n\r\t "));
    boost::algorithm::split(addresses, scopesinput, boost::is_any_of("\n\r\t "));
    hello_(addresses, scopes);
  }
}

}

}
