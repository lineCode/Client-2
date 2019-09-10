// onvifservice.hpp
//

///// Includes /////

#include "onvifserver/onvifservice.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <openssl/sha.h>
#include <utility/utility.hpp>

#include "onvifserver/onvifserver.hpp"
#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

const std::string ADDRESSING_NAMESPACE("http://www.w3.org/2005/08/addressing");
const std::string BASEFAULT_NAMESPACE("http://docs.oasis-open.org/wsrf/bf-2");
const std::string C14N_NAMESPACE("http://www.w3.org/2001/10/xml-exc-c14n#");
const std::string DEVICE_NAMESPACE("http://www.onvif.org/ver10/device/wsdl");
const std::string DEVICEIO_NAMESPACE("http://www.onvif.org/ver10/deviceio/wsdl");
const std::string DS_NAMESPACE("http://www.w3.org/2000/09/xmldsig#");
const std::string ERROR_NAMESPACE("http://www.onvif.org/ver10/error");
const std::string EVENT_NAMESPACE("http://www.onvif.org/ver10/events/wsdl");
const std::string RECEIVER_NAMESPACE("http://www.onvif.org/ver10/receiver/wsdl");
const std::string RECORDING_NAMESPACE("http://www.onvif.org/ver10/recording/wsdl");
const std::string REPLAY_NAMESPACE("http://www.onvif.org/ver10/replay/wsdl");
const std::string RESOURCEFAULT_NAMESPACE("http://docs.oasis-open.org/wsrf/r-2");
const std::string RPC_NAMESPACE("http://www.w3.org/2003/05/soap-rpc");
const std::string SCHEMA_INSTANCE_NAMESPACE("http://www.w3.org/2001/XMLSchema-instance");
const std::string SCHEMA_NAMESPACE("http://www.w3.org/2001/XMLSchema");
const std::string SEARCH_NAMESPACE("http://www.onvif.org/ver10/search/wsdl");
const std::string SOAP_NAMESPACE("http://www.w3.org/2003/05/soap-envelope");
const std::string TOPICS_NAMESPACE("http://www.onvif.org/ver10/topics");
const std::string TYPES_NAMESPACE("http://www.onvif.org/ver10/schema");
const std::string WSNT_NAMESPACE("http://docs.oasis-open.org/wsn/b-2");
const std::string WSSE_NAMESPACE("http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd");
const std::string WSTOP_NAMESPACE("http://docs.oasis-open.org/wsn/t-1");
const std::string XOP_NAMESPACE("http://www.w3.org/2004/08/xop/include");
const std::string XS_NAMESPACE("http://www.w3.org/2000/10/XMLSchema");

const std::string ADDRESSING_NAMESPACE_ATTRIBUTE("xmlns:wsa=\"" + ADDRESSING_NAMESPACE + "\"");
const std::string BASEFAULT_NAMESPACE_ATTRIBUTE("xmlns:wsrf-bf=\"" + BASEFAULT_NAMESPACE + "\"");
const std::string C14N_NAMESPACE_ATTRIBUTE("xmlns:c14n=\"" + C14N_NAMESPACE + "\"");
const std::string DEVICE_NAMESPACE_ATTRIBUTE("xmlns:tds=\"" + DEVICE_NAMESPACE + "\"");
const std::string DEVICEIO_NAMESPACE_ATTRIBUTE("xmlns:tmd=\"" + DEVICEIO_NAMESPACE + "\"");
const std::string DS_NAMESPACE_ATTRIBUTE("xmlns:ds=\"" + DS_NAMESPACE + "\"");
const std::string ERROR_NAMESPACE_ATTRIBUTE("xmlns:ter=\"" + ERROR_NAMESPACE + "\"");
const std::string EVENT_NAMESPACE_ATTRIBUTE("xmlns:tev=\"" + EVENT_NAMESPACE + "\"");
const std::string RECEIVER_NAMESPACE_ATTRIBUTE("xmlns:trv=\"" + RECEIVER_NAMESPACE + "\"");
const std::string RECORDING_NAMESPACE_ATTRIBUTE("xmlns:trc=\"" + RECORDING_NAMESPACE + "\"");
const std::string REPLAY_NAMESPACE_ATTRIBUTE("xmlns:trp=\"" + REPLAY_NAMESPACE + "\"");
const std::string RESOURCEFAULT_NAMESPACE_ATTRIBUTE("xmlns:wsrfr=\"" + RESOURCEFAULT_NAMESPACE + "\"");
const std::string RPC_NAMESPACE_ATTRIBUTE("xmlns:rpc=\"" + RPC_NAMESPACE + "\"");
const std::string SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE("xmlns:xsi=\"" + SCHEMA_INSTANCE_NAMESPACE + "\"");
const std::string SCHEMA_NAMESPACE_ATTRIBUTE("xmlns:xsd=\"" + SCHEMA_NAMESPACE + "\"");
const std::string SEARCH_NAMESPACE_ATTRIBUTE("xmlns:tse=\"" + SEARCH_NAMESPACE + "\"");
const std::string SOAP_NAMESPACE_ATTRIBUTE("xmlns:s=\"" + SOAP_NAMESPACE + "\"");
const std::string TOPICS_NAMESPACE_ATTRIBUTE("xmlns:tns1=\"" + TOPICS_NAMESPACE + "\"");
const std::string TYPES_NAMESPACE_ATTRIBUTE("xmlns:tt=\"" + TYPES_NAMESPACE + "\"");
const std::string WSNT_NAMESPACE_ATTRIBUTE("xmlns:wsnt=\"" + WSNT_NAMESPACE + "\"");
const std::string WSSE_NAMESPACE_ATTRIBUTE("xmlns:wsse=\"" + WSSE_NAMESPACE + "\"");
const std::string WSTOP_NAMESPACE_ATTRIBUTE("xmlns:wstop=\"" + WSTOP_NAMESPACE + "\"");
const std::string XOP_NAMESPACE_ATTRIBUTE("xmlns:xop=\"" + XOP_NAMESPACE + "\"");
const std::string XS_NAMESPACE_ATTRIBUTE("xmlns:xs=\"" + XS_NAMESPACE + "\"");

///// Globals /////

std::string Envelope(const std::string& operation, const std::string& header, const std::string& body, const std::string namespaces, const std::vector<std::string>& additionalnamespaces)
{
  // Concatenate all the namespaces
  std::string ns = namespaces;
  if (namespaces.size())
  {
    ns += std::string(" ") + boost::join(additionalnamespaces, std::string(" "));

  }

  std::string h;
  if (header.size())
  {
    h = "<s:Header>" + header + "</s:Header>";

  }

  return std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<s:Envelope ") + ns + std::string(">") + h + std::string("<s:Body><") + operation + std::string(">") + body + std::string("</") + operation + std::string("></s:Body></s:Envelope>");
}

///// Methods /////

OnvifService::OnvifService(const int onvifmajorversion, const int onvifminorversion, const int onvifauthenticationtimediff, const std::vector<std::string>& namespaces) :
  onvifmajorversion_(onvifmajorversion),
  onvifminorversion_(onvifminorversion),
  onvifauthenticationtimediff_(onvifauthenticationtimediff),
  namespaces_(boost::algorithm::join(namespaces, std::string(" ")))
{

}

OnvifService::~OnvifService()
{

}

bool OnvifService::IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const
{
  switch (requesttype)
  {
    case EVHTTP_REQ_GET:
    {

      return false;
    }
    case EVHTTP_REQ_POST:
    {
      pugi::xml_document doc;
      if (!doc.load_buffer(content.data(), content.size()))
      {

        return false; // The failure of xml parsing should come later as a 400 bad request, not as a 401 unauthorized
      }

      const auto body = doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']");
      for (auto& operationnode : body.node().children())
      {
        const auto usernametoken = doc.select_node("/*[local-name()='Envelope']/*[local-name()='Header']/*[local-name()='Security']/*[local-name()='UsernameToken']");
        if (usernametoken) // If it looks like we have ONVIF auth in this request, we will authorize later
        {

          return false;
        }
        else // If it looks like we don't have any ONVIF auth supplied, we need to check whether authorization is required
        {
          const std::string namespaceuri = GetNamespaceUri(GetNamespace(operationnode.name()), operationnode);
          const std::string action = GetName(operationnode.name());
          if (IsOperationRestricted(onvif::USERLEVEL_ANONYMOUS, namespaceuri, action))
          {

            return true;
          }
        }
      }
    }
    default:
    {

      return false;
    }
  }
}

bool OnvifService::DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return false;
  }

  const auto body = doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']");
  if (!body)
  {

    return false;
  }

  if (std::distance(body.node().children().begin(), body.node().children().end()) != 1) // Make sure there is only one request
  {

    return false;
  }

  const auto operationnode = *body.node().children().begin();
  return OnvifDigestAuthentication(GetNamespaceUri(GetNamespace(operationnode.name()), operationnode), GetName(operationnode.name()), requesttype, clientusername, clientrealm, clientnonce, clienturi, clientnc, clientcnonce, clientqop, clientresponse);
}

boost::shared_future<ServerResponse> OnvifService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (path != GetXAddr()) // Only match exactly
  {
    
    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_WELLFORMED, std::string("Not found")).ToString()));
  }

  switch (requesttype)
  {
    case EVHTTP_REQ_POST:
    {
      pugi::xml_document doc;
      if (!doc.load_buffer(content.data(), content.size()))
      {
        
        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_WELLFORMED, std::string("Malformed request")).ToString()));
      }

      pugi::xml_node headernode;
      const auto header = doc.select_node("/*[local-name()='Envelope']/*[local-name()='Header']");
      if (header)
      {
        headernode = header.node();

      }

      const auto body = doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']");
      if (!body)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_WELLFORMED, std::string("Malformed request")).ToString()));
      }

      if (std::distance(body.node().children().begin(), body.node().children().end()) != 1) // Make sure there is only one request
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Malformed request")).ToString()));
      }

      const auto operationnode = *body.node().children().begin();
      const std::string namespaceuri = GetNamespaceUri(GetNamespace(operationnode.name()), operationnode);
      const std::string action = GetName(operationnode.name());
      if (IsOperationRestricted(onvif::USERLEVEL_ANONYMOUS, namespaceuri, action) && !authenticated)
      {
        const auto usernametokennode = headernode.select_node("/*[local-name()='Security']/*[local-name()='UsernameToken']");
        if (!usernametokennode)
        {

          return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NOTAUTHORIZED, std::string("Action requires authorization")).ToString()));
        }

        const boost::optional<std::string> username = GetText(usernametokennode, "*[local-name()='Username']");
        const boost::optional<std::string> password = GetText(usernametokennode, "*[local-name()='Password']");
        const boost::optional<std::string> nonce = GetText(usernametokennode, "*[local-name()='Nonce']");
        const boost::optional<std::string> time = GetText(usernametokennode, "*[local-name()='Created']");
        if (!username.is_initialized() || !password.is_initialized() || !nonce.is_initialized() || !time.is_initialized() || password->empty() || username->empty() || nonce->empty() || time->empty())
        {

          return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NOTAUTHORIZED, std::string("Incomplete authorization parameters")).ToString()));
        }

        if (!OnvifAuthenticate(namespaceuri, action, *username, *password, *nonce, *time))
        {

          return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NOTAUTHORIZED, std::string("Sender not authorized")).ToString()));
        }
      }

      return OnvifRequest(localaddress, port, remoteaddress, path, namespaceuri, action, headernode, operationnode, mtomdata);
    }
    case EVHTTP_REQ_GET:
    {

      return boost::make_ready_future(GetRequest(localaddress, port, remoteaddress, path, parameters, content, mtomdata));
    }
    default:
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::METHODNOTFOUND, { CONTENT_TYPE }, "Error", "Error 404 Method Not Found"));
    }
  }
}

bool OnvifService::OnvifAuthenticate(const std::string& serverpassword, const std::string& clientpassword, const std::string& clientnonce, const std::string& clienttime) const
{
  // Make sure the time specified is within an acceptable range
  boost::posix_time::ptime pt;
  std::istringstream is(clienttime);
  is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S")));
  is >> pt;
  if (pt == boost::posix_time::ptime()) // If we couldn't parse the time correctly, it is probably in a bad format and we can't validate
  {

    return false;
  }
  
  // Invert the time if negative
  const auto diff = (boost::posix_time::microsec_clock::universal_time() - pt);
  if ((diff > boost::posix_time::time_duration(0, 0, onvifauthenticationtimediff_, 0)) && (diff < boost::posix_time::time_duration(0, 0, onvifauthenticationtimediff_, 0)))
  {

    return false;
  }

  // Validate WSS hash
  std::vector<unsigned char> passwordstring = utility::Base64Decode(clientnonce);
  if (passwordstring.empty())
  {

    return false;
  }

  passwordstring.insert(passwordstring.end(), clienttime.begin(), clienttime.end());
  passwordstring.insert(passwordstring.end(), serverpassword.begin(), serverpassword.end());

  unsigned char passwordhash[20];
  SHA1(reinterpret_cast<const unsigned char*>(passwordstring.data()), passwordstring.size(), passwordhash);

  if (utility::Base64Encode(passwordhash, sizeof(passwordhash)) != clientpassword)
  {

    return false;
  }

  return true;
}

std::string OnvifService::GetNamespace(const std::string& name) const
{
  const size_t index = name.find(':');
  if (index == std::string::npos)
  {

    return std::string();
  }
  else
  {

    return name.substr(0, index);
  }
}

std::string OnvifService::GetName(const std::string& name) const
{
  const size_t index = name.find(':');
  if (index == std::string::npos)
  {

    return name;
  }
  else
  {

    return name.substr(index + 1, std::string::npos);
  }
}

std::string OnvifService::GetNamespaceUri(const std::string& ns, const pugi::xml_node node) const // Traverse through parents looking for the matching namespace
{
  std::string xmlns;
  if (ns.empty()) // Looking for a default namespace
  {
    xmlns = std::string("xmlns");

  }
  else // Looking for a named namespace
  {
    xmlns = std::string("xmlns:") + ns;

  }

  pugi::xml_node currentnode = node;
  while (currentnode != currentnode.root())
  {
    for (const auto& attribute : currentnode.attributes())
    {
      const std::string name = attribute.name();
      if (xmlns == name.substr(0, xmlns.size()))
      {

        return std::string(attribute.value());
      }
    }

    currentnode = currentnode.parent();
  }

  return std::string();
}

boost::optional<std::string> OnvifService::GetText(const pugi::xpath_node& parent, const char* xpath)
{
  auto node = parent.node().select_node(xpath);
  if (node)
  {

    return std::string(node.node().text().get());
  }

  return boost::none;
}

}

}
