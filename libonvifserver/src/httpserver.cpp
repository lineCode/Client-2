// httpserver.hpp
//

///// Includes /////

#include "onvifserver/httpserver.hpp"

#include <algorithm>
#include <boost/algorithm/string_regex.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

const std::string HttpServer::alphanumeric_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const boost::regex HttpServer::contenttypemultipart_("[\\s]*multipart\\/related[\\s]*;[\\s]*(.*)", boost::regex::ECMAScript | boost::regex::icase);
const boost::regex HttpServer::digest_("[\\s]*Digest[\\s]*(.*)", boost::regex::ECMAScript | boost::regex::icase);
const boost::regex HttpServer::parameters_("([^=]+)=\\\"?([^\"]+)\\\"?", boost::regex::ECMAScript | boost::regex::icase);
const boost::regex HttpServer::contenttype_("Content-Type:[\\s](.*)", boost::regex::ECMAScript | boost::regex::icase);
const boost::regex HttpServer::contentid_("Content-ID:[\\s]<([^>]+)>", boost::regex::ECMAScript | boost::regex::icase);

///// Methods /////

void HttpServer::GenericCallback(evhttp_request* request, void* arg)
{
  HttpServer* httpserver = static_cast<HttpServer*>(arg);
  const evhttp_uri* uri = evhttp_request_get_evhttp_uri(request);
  
  // Get the command type
  evhttp_cmd_type requesttype = evhttp_request_get_command(request);
  if ((requesttype != EVHTTP_REQ_GET) && (requesttype != EVHTTP_REQ_POST))
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::BADREQUEST, "Error", "Error 400 Bad Request"));
    return;
  }

  // Get headers
  evkeyvalq* evheaders = evhttp_request_get_input_headers(request);
  if (!evheaders)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::BADREQUEST, "Error", "Error 400 Bad Request"));
    return;
  }

  // Decode uri components
  std::string path;
  const char* evpath = evhttp_uri_get_path(uri);
  if (evpath)
  {
    char* decodedpath = evhttp_uridecode(evpath, 0, nullptr);
    if (decodedpath)
    {
      path = std::string(decodedpath);
      free(decodedpath);
    }
    else
    {
      path = std::string(evpath);

    }
  }
  
  // Find the server
  std::vector< std::pair<std::string, Service*> >::iterator server = std::find_if(httpserver->servers_.begin(), httpserver->servers_.end(), [&path](const std::pair<std::string, Service*>& service){ return (path.compare(0, service.first.length(), service.first) == 0); });
  if (server == httpserver->servers_.end())
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::BADREQUEST, "Error", "Error 400 Bad Request"));
    return;
  }

  std::string parameters;
  const char* evparameters = evhttp_uri_get_query(uri);
  if (evparameters)
  {
    char* decodedparameters = evhttp_uridecode(evparameters, 0, nullptr);
    if (decodedparameters)
    {
      parameters = std::string(decodedparameters);
      free(decodedparameters);
    }
    else
    {
      parameters = std::string(evparameters);

    }
  }

  // Get the body
  evbuffer* evbuf = evhttp_request_get_input_buffer(request);
  size_t const length = evbuffer_get_length(evbuf);
  if (length >= MAXBUFFER)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::BADREQUEST, "Error", "Error 400 Bad Request"));
    return;
  }

  if (evbuffer_remove(evbuf, httpserver->inbuffer_, sizeof(httpserver->inbuffer_)) == -1)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::INTERNALSERVERERROR, "Error", "Error 500 Internal Server Error"));
    return;
  }

  std::vector<char> content = std::vector<char>(httpserver->inbuffer_, httpserver->inbuffer_ + length);

  // Reorganise the body if it is MTOM
  std::map< std::string, std::vector<char> > mtomdatas;
  boost::cmatch cmatch;
  const char* contenttype = evhttp_find_header(evheaders, "Content-Type");
  if (contenttype && boost::regex_search(contenttype, cmatch, httpserver->contenttypemultipart_, boost::regex_constants::match_continuous))
  {
    // Parse MTOM
    std::vector<std::string> contenttypeparameters;
    contenttypeparameters = boost::algorithm::split_regex(contenttypeparameters, cmatch[1].str(), boost::regex(";[\\s]*"));

    std::vector<char> boundary;
    for (const auto& contenttypeparameter : contenttypeparameters)
    {
      boost::smatch smatch;
      if (!boost::regex_search(contenttypeparameter, smatch, httpserver->parameters_, boost::regex_constants::match_continuous))
      {

        continue;
      }

      if (boost::iequals(smatch[1].str(), std::string("boundary")))
      {
        const std::string tmp = std::string("--") + smatch[2].str();
        boundary = std::vector<char>(tmp.begin(), tmp.end());
      }
    }

    if (boundary.empty())
    {
      httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::BADREQUEST, "Error", "Error 400 Bad Request"));
      return;
    }

    // Split over boundary
    for (const auto& block : utility::Split(content, boundary))
    {
      // Split headers and content
      const std::vector<char> httpdelimiter({ '\r', '\n', '\r', '\n' });
      std::vector<char>::const_iterator i = std::search(block.begin(), block.end(), httpdelimiter.begin(), httpdelimiter.end());
      if (i == block.end())
      {

        continue;
      }

      // Split headers
      std::vector<std::string> mtomheaders;
      boost::split_regex(mtomheaders, std::string(block.begin(), i), boost::regex("\r\n|\r[^\n]|[^\r]\n"));
      const std::vector<char> mtomdata(i + httpdelimiter.size(), block.end());

      const std::vector<std::string>::const_iterator contenttype = std::find_if(mtomheaders.begin(), mtomheaders.end(), [httpserver](const std::string& mtomheader) { return boost::regex_search(mtomheader, httpserver->contenttype_, boost::regex_constants::match_continuous); });
      if (contenttype != mtomheaders.end() && (contenttype->find("application/soap+xml") != std::string::npos)) // Cheap check to see if this is the SOAP xml
      {
        content = std::vector<char>(mtomdata.begin(), mtomdata.end());

      }
      else
      {
        // mtomdata
        boost::smatch smatch;
        const std::vector<std::string>::const_iterator contentid = std::find_if(mtomheaders.begin(), mtomheaders.end(), [httpserver, &smatch](const std::string& mtomheader) { return boost::regex_search(mtomheader, smatch, httpserver->contentid_, boost::regex_constants::match_continuous); });
        if (contentid == mtomheaders.end()) // Ignore anything that does not have a Content-ID
        {

          continue;
        }

        mtomdatas[smatch[1].str()] = mtomdata;
      }
    }
  }

  evhttp_connection* connection = evhttp_request_get_connection(request);
  if (!connection)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::INTERNALSERVERERROR, "Error", "500 Internal Server Error"));
    return;
  }

  char* address = nullptr;
  u_short remoteport = 0;
  evhttp_connection_get_peer(connection, &address, &remoteport);
  if (address == nullptr)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
    return;
  }
  const std::string remoteaddress(address);

  // Authorisation
  bool authenticated = false;
  if (server->second->IsRestricted(requesttype, path, std::string(content.begin(), content.end())))
  {
    const char* authorization = evhttp_find_header(evheaders, "Authorization");
    if (authorization)
    {
      boost::cmatch cmatch;
      if (boost::regex_search(authorization, cmatch, httpserver->digest_, boost::regex_constants::match_continuous))
      {
        // Separate inputs
        std::vector<std::string> authorisationparameters;
        authorisationparameters = boost::algorithm::split_regex(authorisationparameters, cmatch[1].str(), boost::regex(",[\\s]*"));

        std::string clientusername;
        std::string clientrealm;
        std::string clientnonce;
        std::string clienturi;
        std::string clientcnonce;
        std::string clientnc;
        std::string clientresponse;
        std::string clientqop;
        for (const auto& authorisationparameter : authorisationparameters)
        {
          boost::smatch smatch;
          if (!boost::regex_search(authorisationparameter, smatch, httpserver->parameters_, boost::regex_constants::match_continuous))
          {

            continue;
          }

          if (boost::iequals(smatch[1].str(), std::string("username")))
          {
            clientusername = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("realm")))
          {
            clientrealm = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("nonce")))
          {
            clientnonce = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("uri")))
          {
            clienturi = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("cnonce")))
          {
            clientcnonce = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("nc")))
          {
            clientnc = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("response")))
          {
            clientresponse = smatch[2].str();

          }
          else if (boost::iequals(smatch[1].str(), std::string("qop")))
          {
            clientqop = smatch[2].str();

          }
        }

        if (clientusername.empty() || clientrealm.empty() || clientnonce.empty() || clienturi.empty() || clientcnonce.empty() || clientnc.empty() || clientresponse.empty())
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }

        if (clientrealm != httpserver->realm_)
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }

        // Get the user
        const bool digestauthenticated = server->second->DigestAuthenticate(requesttype, content, clientusername, clientrealm, clientnonce, clienturi, clientnc, clientcnonce, clientqop, clientresponse);

        // Get the user with previous nonce to indicate staleness
        std::vector<User>::iterator staleuser = std::find_if(httpserver->users_.begin(), httpserver->users_.end(), [remoteaddress, clientnonce](const User& user) { return ((user.GetAddress() == remoteaddress) && (std::find(user.GetPrevNonces().begin(), user.GetPrevNonces().end(), clientnonce) != user.GetPrevNonces().end())); });
        if ((staleuser != httpserver->users_.end()) && digestauthenticated) // We must only indicate stale if the authentication is valid, but old
        {
          std::string nonce;
          if (staleuser->GetNonce().IsEmpty())
          {
            staleuser->SetNonce(httpserver->GenerateNonce());
            nonce = staleuser->GetNonce().GetNonce();
          }
          else
          {
            nonce = staleuser->GetNonce().GetNonce();

          }

          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, { Header::WWWAuthenticate(httpserver->realm_, nonce, true) }, "Error", "401 Unauthorized"));
          return;
        }

        // Get the user matched by the current nonce
        const std::vector<User>::iterator user = std::find_if(httpserver->users_.begin(), httpserver->users_.end(), [remoteaddress, clientnonce](const User& user) { return ((user.GetAddress() == remoteaddress) && (user.GetNonce() == clientnonce)); });
        if (user == httpserver->users_.end())
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }
        
        // Check nc is increasing
        unsigned int nc = std::strtoul(clientnc.c_str(), nullptr, 10);
        if (nc <= user->GetNc())
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }
        user->SetNc(nc);

        // Check path is correct
        if (clienturi != path)
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }

        // Check the authentication values are correct
        if (!digestauthenticated)
        {
          httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
          return;
        }

        authenticated = true;
      }
      else
      {
        httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, "Error", "401 Unauthorized"));
        return;
      }
    }
    else
    {
      const Nonce nonce = httpserver->GenerateNonce();
      httpserver->users_.push_back(User(remoteaddress, nonce, std::chrono::steady_clock::now()));

      httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::UNAUTHORIZED, { Header::WWWAuthenticate(httpserver->realm_, nonce.GetNonce(), false) }, "Error", "401 Unauthorized"));
      return;
    }
  }

  const char* localaddress = evhttp_request_get_host(request);
  if (!localaddress)
  {
    httpserver->SendResponse(request, ServerResponse(HTTPSTATUSCODE::INTERNALSERVERERROR, "Error", "500 Internal Server Error"));
    return;
  }

  // Pass the message to the handler
  boost::shared_future<ServerResponse> response = server->second->Request(authenticated, requesttype, localaddress, httpserver->port_, remoteaddress, path, parameters, content, mtomdatas);
  if (response.is_ready())
  {
    httpserver->SendResponse(request, response.get());

  }
  else
  {
    httpserver->responses_.emplace_back(std::move(std::make_pair(request, std::move(response))));

  }
}

///// Methods /////

HttpServer::HttpServer(const std::string& realm) :
  realm_(realm),
  rng_(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
  distribution_(0, static_cast<unsigned int>(alphanumeric_.length() - 1)),
  base_(nullptr),
  http_(nullptr),
  handle_(nullptr),
  port_(0),
  inbuffer_{},
  outbuffer_(nullptr),
  usertimeout_(std::chrono::seconds(600)),
  noncetimeout_(std::chrono::seconds(60)),
  prevnoncetimeout_(std::chrono::seconds(300))
{

}

HttpServer::~HttpServer()
{
  Destroy();

}

int HttpServer::Init(const std::string& address, const uint16_t port)
{
  if (Destroy())
  {

    return 1;
  }

  std::lock_guard<std::mutex> lock(mutex_);

  base_ = event_base_new();
  if (!base_)
  {

    return 1;
  }

  http_ = evhttp_new(base_);
  if (!http_)
  {

    return 1;
  }

  evhttp_set_gencb(http_, GenericCallback, this);

  outbuffer_ = evbuffer_new();
  if (!outbuffer_)
  {

    return 1;
  }

  handle_ = evhttp_bind_socket_with_handle(http_, address.c_str(), port);
  if (!handle_)
  {

    return 1;
  }

  port_ = port;
  
  return 0;
}

int HttpServer::Destroy()
{
  std::lock_guard<std::mutex> lock(mutex_);

  if (outbuffer_)
  {
    evbuffer_free(outbuffer_);
    outbuffer_ = nullptr;
  }

  if (http_)
  {
    evhttp_set_gencb(http_, nullptr, nullptr);

    evhttp_free(http_);
    http_ = nullptr;
  }

  if (base_)
  {
    event_base_free(base_);
    base_ = nullptr;
  }

  port_ = 0;

  servers_.clear();
  users_.clear();

  return 0;
}

int HttpServer::AddServer(const std::string& uri, Service* server)
{
  if (server == nullptr)
  {

    return 1;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  const auto i = std::find_if(servers_.begin(), servers_.end(), [&uri](const std::pair<std::string, Service*>& service){ return (service.first == uri); });
  if (i != servers_.end())
  {

    return 2;
  }
  servers_.push_back(std::make_pair(uri, server));
  return 0;
}

int HttpServer::RemoveServer(const std::string& uri)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto i = std::find_if(servers_.begin(), servers_.end(), [uri](const std::pair<std::string, Service*>& service){ return (service.first == uri); });
  if (i == servers_.end())
  {

    return 1;
  }
  servers_.erase(i);
  return 0;
}

int HttpServer::Update()
{
  std::lock_guard<std::mutex> lock(mutex_);

  const auto now = std::chrono::steady_clock::now();

  // Timeout users
  users_.erase(std::remove_if(users_.begin(), users_.end(), [this, now](const User& user) { return ((now - user.GetLastRequestTime()) > usertimeout_); }), users_.end());

  // Sort out nonces
  for (auto& user : users_)
  {
    // Timeout current nonce
    if ((now - user.GetNonce().GetTime()) > noncetimeout_)
    {
      user.GetPrevNonces().push_back(user.GetNonce());
      user.SetNonce(Nonce());
    }

    // Timeout prev nonces
    user.GetPrevNonces().erase(std::remove_if(user.GetPrevNonces().begin(), user.GetPrevNonces().end(), [this, now](const Nonce& prevnonce) { return ((now - prevnonce.GetTime()) > prevnoncetimeout_); }), user.GetPrevNonces().end());
  }

  if (!base_)
  {

    return 1;
  }

  if (event_base_loop(base_, EVLOOP_NONBLOCK))
  {

    return 1;
  }

  // Deal with async requests
  for (auto response = responses_.begin(); response != responses_.end();)
  {
    if (response->second.is_ready())
    {
      SendResponse(response->first, response->second.get());
      response = responses_.erase(response);
    }
    else
    {
      ++response;

    }
  }

  // Provide tick for all services
  for (auto& service : servers_)
  {
    service.second->Update();

  }

  return 0;
}

// There is an expectation that the calls in this method will not fail, so we do not respond if there is a failure as it may indicate that it may be a suspicious request
void HttpServer::SendResponse(evhttp_request* request, const ServerResponse& response)
{
  // Headers
  evkeyvalq* headers = evhttp_request_get_output_headers(request);
  for (const auto& header : response.GetHeaders())
  {
    if (evhttp_add_header(headers, header.GetKey().c_str(), header.GetValue().c_str()))
    {

      return;
    }
  }

  // Body
  if (evbuffer_add(outbuffer_, response.GetBody().c_str(), response.GetBody().size()))
  {

    return;
  }

  evhttp_send_reply(request, static_cast<int>(response.GetHttpStatusCode()), response.GetReason().c_str(), outbuffer_);
}

Nonce HttpServer::GenerateNonce()
{
  static const unsigned int noncelength = 64;
  std::string nonce;
  nonce.resize(noncelength);
  for (unsigned int i = 0; i < noncelength; ++i)
  {
    nonce.at(i) = alphanumeric_[distribution_(rng_)];

  }
  return Nonce(nonce, std::chrono::steady_clock::now());
}

}

}
