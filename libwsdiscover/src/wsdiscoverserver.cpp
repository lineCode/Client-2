// wsdiscoverserver.cpp
//

///// Includes /////

#include "wsdiscover/wsdiscoverserver.hpp"

#ifdef _WIN32
  #include <codecvt>
#endif

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <locale>
#include <network/uri.hpp>
#include <thread>
#include <pugixml.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

///// Globals /////

const boost::asio::ip::address_v4 WSDISCOVERY_ADDRESS = boost::asio::ip::address_v4::from_string("239.255.255.250");

///// Methods /////

WsDiscoverServer::WsDiscoverServer(boost::asio::io_service& io) :
#ifdef _WIN32
  context_(nullptr),
  host_(nullptr),
#endif
  socketv4_(io),
  enabled_(true),
  instance_(0),
  messagenumber_(0),
  metadataversion_(1)
{

}

WsDiscoverServer::~WsDiscoverServer()
{
  Destroy();

}

int WsDiscoverServer::Init(const boost::asio::ip::address& address, unsigned short port, const bool enabled, const boost::uuids::uuid& wsaddress, const uint64_t instance, const std::vector<std::string>& scopes, const std::vector<std::string>& xaddrs)
{
  if (Destroy())
  {
    
    return 1;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  listenendpoint_ = boost::asio::ip::udp::endpoint(address, port);
  enabled_ = enabled;
  wsaddress_ = wsaddress;
  instance_ = instance;
  scopes_ = scopes;
  xaddrs_ = xaddrs;

#ifdef _WIN32
  //if (!InitWindows()) // Attempt to initialise with Windows Discovery API
  {

   // return 0;
  }
#endif

  boost::system::error_code err;
  if (socketv4_.open(listenendpoint_.protocol(), err))
  {
    
    return 1;
  }

  // We don't really want to share this port anymore, but may want to in the future
  if (socketv4_.set_option(boost::asio::ip::udp::socket::reuse_address(true), err))
  {

    return 2;
  }

  if (socketv4_.set_option(boost::asio::ip::multicast::enable_loopback(true), err))
  {

    return 3;
  }
  
  if (socketv4_.bind(listenendpoint_, err))
  {

    return 4;
  }

  if (socketv4_.set_option(boost::asio::ip::multicast::join_group(WSDISCOVERY_ADDRESS, listenendpoint_.address().to_v4()), err))
  {

    return 5;
  }
  
  socketv4_.async_receive_from(boost::asio::buffer(buffer_), senderendpoint_, boost::bind(&WsDiscoverServer::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

  return 0;
}

int WsDiscoverServer::Destroy()
{
  std::lock_guard<std::mutex> lock(mutex_);
#ifdef _WIN32
  if (DestroyWindows())
  {


  }
#endif

  if (socketv4_.is_open())
  {
    boost::system::error_code err;
    if (socketv4_.close(err))
    {


    }
  }

  enabled_ = true;
  instance_ = 0;
  wsaddress_ = boost::uuids::nil_uuid();
  scopes_.clear();
  xaddrs_.clear();

  return 0;
}

int WsDiscoverServer::Hello()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (!enabled_)
  {

    return 0;
  }

  const std::string message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"><SOAP-ENV:Header><wsa:MessageID>uuid:" + boost::lexical_cast<std::string>(randomgenerator_()) + "</wsa:MessageID><wsa:To SOAP-ENV:mustUnderstand=\"true\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello</wsa:Action><d:AppSequence SOAP-ENV:mustUnderstand=\"true\" MessageNumber=\"0\" InstanceId=\"" + std::to_string(instance_) + "\"/></SOAP-ENV:Header><SOAP-ENV:Body><d:Hello><wsa:EndpointReference><wsa:Address>urn:uuid:" + boost::lexical_cast<std::string>(wsaddress_) +"</wsa:Address></wsa:EndpointReference><d:Types>tds:Device dn:NetworkVideoTransmitter</d:Types><d:Scopes>" + GetScopesString() + "</d:Scopes><d:XAddrs>" + GetXAddrsString() + "</d:XAddrs><d:MetadataVersion>1</d:MetadataVersion></d:Hello></SOAP-ENV:Body></SOAP-ENV:Envelope>";
  boost::system::error_code err;
  if (socketv4_.send_to(boost::asio::buffer(message), boost::asio::ip::udp::endpoint(WSDISCOVERY_ADDRESS, listenendpoint_.port()), 0, err) == -1)
  {

    return 1;
  }
  return 0;
}

int WsDiscoverServer::Bye()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (!enabled_)
  {

    return 0;
  }

  const std::string message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"><SOAP-ENV:Header><wsa:MessageID>uuid:" + boost::lexical_cast<std::string>(randomgenerator_()) + "</wsa:MessageID><wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Bye</wsa:Action><d:AppSequence MessageNumber=\"0\" InstanceId=\"" + std::to_string(instance_) + "\"/></SOAP-ENV:Header><SOAP-ENV:Body><d:Bye><wsa:EndpointReference><wsa:Address>urn:uuid:" + boost::lexical_cast<std::string>(wsaddress_) + "</wsa:Address></wsa:EndpointReference></d:Bye></SOAP-ENV:Body></SOAP-ENV:Envelope>";
  boost::system::error_code err;
  if (socketv4_.send_to(boost::asio::buffer(message), boost::asio::ip::udp::endpoint(WSDISCOVERY_ADDRESS, listenendpoint_.port()), 0, err) == -1)
  {

    return 1;
  }
  return 0;
}

void WsDiscoverServer::Enable()
{
  std::lock_guard<std::mutex> lock(mutex_);
  enabled_ = true;
}

void WsDiscoverServer::Disable()
{
  std::lock_guard<std::mutex> lock(mutex_);
  enabled_ = false;
}

void WsDiscoverServer::SetScopes(const std::vector<std::string>& scopes)
{
  std::lock_guard<std::mutex> lock(mutex_);
  scopes_ = scopes;
}

void WsDiscoverServer::AddScope(const std::string& uri)
{
  std::lock_guard<std::mutex> lock(mutex_);

  // Does the scope already exist?
  if (std::find(scopes_.begin(), scopes_.end(), uri) == scopes_.end())
  {

    return;
  }

  scopes_.push_back(uri);
}

void WsDiscoverServer::AddScopes(const std::vector<std::string>& scopes)
{
  std::lock_guard<std::mutex> lock(mutex_);

  for (const std::string& scope : scopes)
  {
    if (std::find(scopes_.begin(), scopes_.end(), scope) != scopes_.end())
    {

      return;
    }
    scopes_.push_back(scope);
  }
}

bool WsDiscoverServer::RemoveScope(const std::string& uri)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto i = std::find(scopes_.begin(), scopes_.end(), uri);
  if (i == scopes_.end())
  {

    return false;
  }

  scopes_.erase(i);
  return true;
}

void WsDiscoverServer::RemoveScopes(const std::vector<std::string>& scopes)
{
  std::lock_guard<std::mutex> lock(mutex_);
  for (const std::string& scope : scopes)
  {
    auto i = std::find(scopes_.cbegin(), scopes_.cend(), scope);
    if (i == scopes_.cend())
    {

      continue;
    }
    scopes_.erase(i);
  }
}

void WsDiscoverServer::AddXAddr(const std::string& xaddr)
{
  std::lock_guard<std::mutex> lock(mutex_);
  for (const auto& i : xaddrs_)
  {
    if (i == xaddr)
    {

      return;
    }
  }
  xaddrs_.push_back(xaddr);
}

int WsDiscoverServer::RemoveXAddr(const std::string& xaddr)
{
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto i = xaddrs_.begin(); i != xaddrs_.end(); ++i)
  {
    if (*i == xaddr)
    {
      xaddrs_.erase(i);
      return 0;
    }
  }
  return 1;
}

void WsDiscoverServer::SetXAddrs(const std::vector<std::string>& xaddrs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  xaddrs_ = xaddrs;
}

void WsDiscoverServer::HandleReceive(const boost::system::error_code& error, const size_t receivedbytes)
{
  if (error)
  {
    // Just ignore this for the moment because it is so unlikely to occur
    return;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  if (enabled_)
  {
    HandleReceiveData(buffer_.data(), receivedbytes);

  }
  socketv4_.async_receive_from(boost::asio::buffer(buffer_), senderendpoint_, boost::bind(&WsDiscoverServer::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

#ifdef _WIN32
int WsDiscoverServer::InitWindows()
{
  if (WSDXMLCreateContext(&context_))
  {
    
    return 1;
  }

  return 1;
}

int WsDiscoverServer::DestroyWindows()
{

  return 0;
}
#endif

void WsDiscoverServer::HandleReceiveData(const char* buffer, const size_t size)
{
  pugi::xml_document xml;
  if (!xml.load_buffer(buffer, size))
  {

    return;
  }

  // Check this is a SOAP envelope
  const auto envelope = xml.select_single_node("/*[local-name()='Envelope']");
  if (!envelope.node())
  {

    return;
  }

  // Get the action and only respond to probes
  const auto actionnode = envelope.node().select_single_node("*[local-name()='Header']/*[local-name()='Action']");
  if (!actionnode.node())
  {

    return;
  }
  const std::string action = actionnode.node().text().get();
  if (action != "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe")
  {

    return;
  }

  // Get the uuid
  const auto messageidnode = envelope.node().select_single_node("*[local-name()='Header']/*[local-name()='MessageID']");
  if (!messageidnode.node())
  {

    return;
  }
  const std::string messageid = messageidnode.node().text().get();

  // Scopes
  const auto scopesnode = envelope.node().select_single_node("*[local-name()='Body']/*[local-name()='Probe']/*[local-name()='Scopes']");
  if (scopesnode)
  {
    pugi::xml_attribute matchby = scopesnode.node().attribute("MatchBy");
    if (matchby) // Ignore anything that isn't empty. Perhaps this can be improved in the future..?
    {
      try
      {
        network::uri(matchby.value());

      }
      catch (...)
      {
        const std::string message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"><SOAP-ENV:Header><wsa:RelatesTo>" + messageid + "</wsa:RelatesTo><wsa:To SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To><wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/fault</wsa:Action></SOAP-ENV:Header><SOAP-ENV:Body><SOAP-ENV:Fault><SOAP-ENV:Code><SOAP-ENV:Value>SOAP-ENV:Sender</SOAP-ENV:Value><SOAP-ENV:Subcode><SOAP-ENV:Value>d:MatchingRuleNotSupported</SOAP-ENV:Value></SOAP-ENV:Subcode></SOAP-ENV:Code><SOAP-ENV:Reason><SOAP-ENV:Text xml:lang=\"en\">The matching rule specified is not supported</SOAP-ENV:Text></SOAP-ENV:Reason><SOAP-ENV:Detail><d:SupportedMatchingRules>http://schemas.xmlsoap.org/ws/2005/04/discovery/rfc3986</d:SupportedMatchingRules></SOAP-ENV:Detail></SOAP-ENV:Fault></SOAP-ENV:Body></SOAP-ENV:Envelope>";
        boost::system::error_code err;
        if (socketv4_.send_to(boost::asio::buffer(message), senderendpoint_, 0, err) == -1)
        {

          return;
        }
      }

      return;
    }

    const std::vector<std::string> scopes = utility::SplitString(scopesnode.node().text().get(), { ' ', '\t', '\r', '\n' });
    if (!Filter(scopes))
    {

      return;
    }
  }

  // Put together message and send response
  const std::string message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"><SOAP-ENV:Header><wsa:MessageID>uuid:" + boost::lexical_cast<std::string>(randomgenerator_()) + "</wsa:MessageID><wsa:RelatesTo>" + messageid + "</wsa:RelatesTo><wsa:To SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To><wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action><d:AppSequence SOAP-ENV:mustUnderstand=\"true\" MessageNumber=\"" + std::to_string(++messagenumber_) + "\" InstanceId=\"" + std::to_string(instance_) + "\"></d:AppSequence></SOAP-ENV:Header><SOAP-ENV:Body><d:ProbeMatches><d:ProbeMatch><wsa:EndpointReference><wsa:Address>urn:uuid:" + boost::lexical_cast<std::string>(wsaddress_) + "</wsa:Address></wsa:EndpointReference><d:Types>tds:Device dn:NetworkVideoTransmitter</d:Types><d:Scopes>" + GetScopesString() + "</d:Scopes><d:XAddrs>" + GetXAddrsString() + "</d:XAddrs><d:MetadataVersion>" + std::to_string(metadataversion_) + "</d:MetadataVersion></d:ProbeMatch></d:ProbeMatches></SOAP-ENV:Body></SOAP-ENV:Envelope>";
  boost::system::error_code err;
  if (socketv4_.send_to(boost::asio::buffer(message), senderendpoint_, 0, err) == -1)
  {

    return;
  }
}

bool WsDiscoverServer::Filter(const std::vector<std::string>& scopes) const
{
  if (scopes.empty())
  {

    return true;
  }

  for (const std::string& scope : scopes)
  {
    network::uri uri;
    try
    {
      uri = network::uri(scope);

    }
    catch (...)
    {

      continue;
    }

    if (uri.scheme().compare("onvif"))
    {

       continue;
    }

    if (uri.host().compare("www.onvif.org"))
    {

      continue;
    }

    // Check against local scopes
    const std::string scopepath = scope + "/";
    if (std::find_if(scopes_.cbegin(), scopes_.cend(), [&scope, &scopepath](const std::string& s){ return ((s == scope) || (s.compare(0, scopepath.length(), scopepath) == 0)); }) != scopes_.cend())
    {

      return true;
    }
  }

  return false;
}

std::string WsDiscoverServer::GetScopesString() const
{
  std::string scopes;
  for (const auto& scope : scopes_)
  {
    if (!scopes.empty())
    {

      scopes += ' ';
    }
    scopes += scope;
  }
  return scopes;
}

std::string WsDiscoverServer::GetXAddrsString() const
{
  std::string xaddrs;
  for (const auto& xaddr : xaddrs_)
  {
    if (!xaddrs.empty())
    {

      xaddrs += ' ';
    }
    xaddrs += xaddr;
  }
  return xaddrs;
}

}

}
