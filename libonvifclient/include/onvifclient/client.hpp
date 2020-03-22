// client.hpp
//

#ifndef IDPVDH92OPQN16NIY6T83EZ45OVBJV2JC3
#define IDPVDH92OPQN16NIY6T83EZ45OVBJV2JC3

#pragma warning(push)
#pragma warning(disable : 4003)

///// Includes /////

#include <algorithm>
#include <boost/algorithm/string_regex.hpp>
#include <boost/date_time.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include <boost/scope_exit.hpp>
#include <boost/signals2.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <curl/curl.h>
#include <pugixml.hpp>
#include <socket/proxyparams.hpp>
#include <socket/socket.hpp>
#include <utility/utility.hpp>
#include <mutex>
#include <onviftypes/onviftypes.hpp>
#include <openssl/sha.h>
#include <string>
#include <vector>

///// Namespaces /////

namespace onvif
{

///// Globals /////

const std::string UNABLETOCONNECT("Unable to Connect");

///// Structures /////

template<class T>
struct Request
{
  Request(T operation) :
    operation_(operation),
    handle_(curl_easy_init()),
    headers_(nullptr),
    latency_(std::chrono::milliseconds(0))
  {
    assert(handle_);

  }

  ~Request()
  {
    curl_easy_cleanup(handle_);
    FreeHeaders();
    handle_ = nullptr;
  }

  void FreeHeaders()
  {
    if (headers_)
    {
      curl_slist_free_all(headers_);
      headers_ = nullptr;
    }
  }

  T operation_;
  CURL* handle_;
  curl_slist* headers_;
  std::vector<std::string> responseheaders_;
  std::vector<char> responsebody_;
  std::chrono::high_resolution_clock::time_point latency_;
};

///// Classes /////

template<class T>
class Client
{
  struct QUEUEREQUEST
  {
    QUEUEREQUEST(Request<T>* request, T operation, const bool authentication, const bool event, const std::string& action, const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map< std::string, std::vector<char> >& mtomdata) :
      request_(request),
      operation_(operation),
      authentication_(authentication),
      event_(event),
      action_(action),
      referenceparameters_(referenceparameters),
      to_(to),
      body_(body),
      mtomdata_(mtomdata)
    {

    }

    Request<T>* request_;
    T operation_;
    bool authentication_;
    bool event_;
    const std::string action_;
    const std::vector<Element> referenceparameters_;
    const std::string to_;
    const std::string body_;
    const std::map< std::string, std::vector<char> > mtomdata_;
  };

 public:

  static size_t SendCommandHeaderCallback(void* data, size_t size, size_t nmemb, void* headers)
  {
    static_cast<std::vector<std::string>*>(headers)->push_back(std::string(static_cast<char*>(data)));
    return (size*nmemb);
  }
 
  static size_t SendCommandBodyCallback(void* data, size_t size, size_t nmemb, void* body)
  {
    static_cast<std::vector<char>*>(body)->insert(static_cast<std::vector<char>*>(body)->end(), static_cast<char*>(data), static_cast<char*>(data) + (size*nmemb));
    return (size*nmemb);
  }
  
  Client(std::recursive_mutex& mutex) :
    mutex_(mutex),
    contenttypemultipart_("Content-Type:[\\s]*multipart\\/related", boost::regex::ECMAScript | boost::regex::icase),
    contenttypeparameters_("([^=]+)=\\\"?([^\"]+)\\\"?", boost::regex::ECMAScript | boost::regex::icase),
    contenttype_("Content-Type:[\\s](.*)", boost::regex::ECMAScript | boost::regex::icase),
    contentid_("Content-ID:[\\s]<([^>]+)>", boost::regex::ECMAScript | boost::regex::icase),
    curlm_(nullptr),
    maxconcurrentrequests_(0),
    numconcurrentrequests_(0),
    forcehttpauthentication_(false),
    forbidreuse_(false)
  {

  }
  
  virtual ~Client()
  {
    Destroy();

  }

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password)
  {
    return Init(proxyparams, address, username, password, 0, false, false);
  }

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse)
  {
    if (address.empty())
    {
    
      return 1;
    }

    curlm_ = curl_multi_init();
    if (!curlm_)
    {

      return 2;
    }

    maxconcurrentrequests_ = 0;
    numconcurrentrequests_ = 0;
    
    proxyparams_ = proxyparams;
    address_ = address;
    username_ = username;
    password_ = password;
    maxconcurrentrequests_ = maxconcurrentrequests;
    forcehttpauthentication_ = forcehttpauthentication;
    forbidreuse_ = forbidreuse;

    return 0;
  }

  virtual void Destroy()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
  
    // Destory multi handle
    if (curlm_)
    {
      if (curl_multi_cleanup(curlm_))
      {
        // Just ignore this as it should never really be happening

      }
      curlm_ = nullptr;
    }

    latencysignal_.disconnect_all_slots();

    queue_.clear();
    maxconcurrentrequests_ = 0;
    numconcurrentrequests_ = 0;

    proxyparams_.Clear();
    address_.clear();
    username_.clear();
    password_.clear();
  }

  CURL* SendCommand(T operation, const bool authentication, const bool event, const std::string& action, const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map<std::string, std::vector<char> >& mtomdata)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    Request<T>* request = new Request<T>(operation);

    if ((maxconcurrentrequests_ != 0) && (numconcurrentrequests_ >= maxconcurrentrequests_)) // If we have too many request running concurrently, slow down a bit
    {
      queue_.push_back(QUEUEREQUEST(request, operation, authentication, event, action, referenceparameters, to, body, mtomdata));
      return request->handle_;
    }

    return SendCommand(request, operation, authentication, event, action, referenceparameters, to, body, mtomdata);
  }
  
  virtual int Update()
  {
    // Process requests
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    
    int numrunning = 0;
    if (curl_multi_perform(curlm_, &numrunning))
    {
    
      return 1;
    }

    CURLMsg* msg = nullptr;
    while ((msg = curl_multi_info_read(curlm_, &numrunning))) // Extra parenthesis to silence clang warning
    {
      Request<T>* request = nullptr;
      if (curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &request))
      {

        return 2;
      }
      --numconcurrentrequests_;

      assert(request);
      assert(msg->msg == CURLMSG_DONE);
      
      // Add the requests back to the pool regardless of outcome
      BOOST_SCOPE_EXIT_ALL(&request, this)
      {
        curl_multi_remove_handle(curlm_, request->handle_);
        delete request;
      };

      boost::asio::ip::address localendpoint;
      char* buffer = nullptr;
      if (curl_easy_getinfo(msg->easy_handle, CURLINFO_LOCAL_IP, &buffer) == CURLE_OK)
      {
        if (buffer)
        {
          boost::system::error_code err;
          localendpoint = boost::asio::ip::address::from_string(buffer, err);
        }
      }
      
      const int64_t latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - request->latency_).count();

      latencysignal_(boost::posix_time::time_duration(boost::posix_time::milliseconds(latency)));

      if (msg->data.result != CURLE_OK)
      {
        SignalError(request->operation_, msg->easy_handle, localendpoint, latency, UNABLETOCONNECT);
        continue;
      }

      // Get the content
      std::string content;
      std::map< std::string, std::vector<char> > mtomdatas;
      boost::smatch match;
      std::vector<std::string>::iterator multipartrelatedheader = std::find_if(request->responseheaders_.begin(), request->responseheaders_.end(), [this, &match](const std::string& header) { return boost::regex_search(header, match, contenttypemultipart_, boost::regex_constants::match_continuous); });
      if ((multipartrelatedheader != request->responseheaders_.end()) && (match.size() > 0))
      {
        // Parse MTOM
        std::vector<std::string> contenttypeparameters;
        contenttypeparameters = boost::algorithm::split_regex(contenttypeparameters, multipartrelatedheader->substr(match[0].str().size(), std::string::npos), boost::regex(";[\\s]*"));
         
        std::vector<char> boundary;
        for (const auto& contenttypeparameter : contenttypeparameters)
        {
          if (!boost::regex_search(contenttypeparameter, match, contenttypeparameters_, boost::regex_constants::match_continuous))
          {
            
            continue;
          }

          if (boost::iequals(match[1].str(), std::string("boundary")))
          {
            const std::string tmp = std::string("\r\n--") + match[2].str();
            boundary = std::vector<char>(tmp.begin(), tmp.end());
          }
        }

        if (boundary.empty())
        {
          SignalError(request->operation_, msg->easy_handle, localendpoint, latency, std::string("Invalid xml"));
          continue;
        }

        // Split over boundary
        for (const auto& block : utility::Split(request->responsebody_, boundary))
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

          const std::vector<std::string>::const_iterator contenttype = std::find_if(mtomheaders.begin(), mtomheaders.end(), [this, &match](const std::string& mtomheader) { return boost::regex_search(mtomheader, match, contenttype_, boost::regex_constants::match_continuous); });
          if (contenttype != mtomheaders.end() && (contenttype->find("application/soap+xml") != std::string::npos)) // Cheap check to see if this is the SOAP xml
          {
            content = std::string(mtomdata.begin(), mtomdata.end());

          }
          else
          {
            // mtomdata
            const std::vector<std::string>::const_iterator contentid = std::find_if(mtomheaders.begin(), mtomheaders.end(), [this, &match](const std::string& mtomheader) { return boost::regex_search(mtomheader, match, contentid_, boost::regex_constants::match_continuous); });
            if (contentid == mtomheaders.end()) // Ignore anything that does not have a Content-ID
            {

              continue;
            }

            mtomdatas[match[1].str()] = mtomdata;
          }
        }
      }
      else
      {
        content = std::string(request->responsebody_.data(), request->responsebody_.size());

      }

      if (content.empty())
      {
        SignalError(request->operation_, msg->easy_handle, localendpoint, latency, std::string("Invalid xml"));
        continue;
      }
      
      // Parse xml
      pugi::xml_document document;
      if (!document.load(content.c_str()))
      {
        SignalError(request->operation_, msg->easy_handle, localendpoint, latency, std::string("Invalid xml:\n") + content);
        continue;
      }
      
      // Check whether the http request was successful
      long responsecode = 0;
      if (curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &responsecode) != CURLE_OK)
      {
        SignalError(request->operation_, msg->easy_handle, localendpoint, latency, std::string("CURLINFO_RESPONSE_CODE error"));
        return 1;
      }

      // If not successful, try to parse what went wrong and send result back
      if ((responsecode <= 199) || (responsecode >= 300))
      {
        std::string message;
        auto fault = document.select_single_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='Fault']");
        if (fault.node())
        {
          auto faulstringnode = fault.node().select_node("*[local-name()='faulstring']");
          auto reasontextnode = fault.node().select_node("*[local-name()='Reason']/*[local-name()='Text']");
          if (faulstringnode.node()) // Soap 1.1 fault message
          {
            message = faulstringnode.node().text().get();

          }
          if (reasontextnode.node()) // Soap 1.2 fault message
          {
            message = reasontextnode.node().text().get();

          }
        }

        if (message.empty()) // If we haven't been able to find a proper error message, just send the entire xml as the error message
        {
           message = std::string("Fault:\n" + content);

        }
        
        SignalError(request->operation_, msg->easy_handle, localendpoint, latency, message);
        continue;
      }

      // Everything looks ok, send to subclass to sort out
      Update(request->operation_, msg->easy_handle, localendpoint, latency, document, mtomdatas);
    }

    while(queue_.size() && ((numconcurrentrequests_ < maxconcurrentrequests_) || (maxconcurrentrequests_ == 0))) // If we have requests waiting in the queue, and we have room to send them
    {
      QUEUEREQUEST queuerequest = queue_.back();
      queue_.pop_back();
      SendCommand(queuerequest.request_, queuerequest.operation_, queuerequest.authentication_, queuerequest.event_, queuerequest.action_, queuerequest.referenceparameters_, queuerequest.to_, queuerequest.body_, queuerequest.mtomdata_);
    }

    return 0;
  }

  inline bool IsInitialised()
  {
    return (curlm_ ? true : false);
  }

  inline void SetMaxConcurrentRequests(unsigned int maxconcurrentrequests) { maxconcurrentrequests_ = maxconcurrentrequests; }
  inline unsigned int GetMaxConcurrentRequests() const { return maxconcurrentrequests_; }

  boost::signals2::signal<void(const boost::posix_time::time_duration&)>& GetLatencySignal()
  {
    return latencysignal_;
  }

  inline void SetAddress(const std::string& address)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    address_ = address;
  }

  inline const std::string GetAddress()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return address_;
  }

  inline void SetUsername(const std::string& username)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    username_ = username;
  }

  inline const std::string GetUsername()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return username_;
  }

  inline void SetPassword(const std::string& password)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    password_ = password;
  }

  inline const std::string GetPassword()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return password_;
  }

  inline void SetForceHttpAuthentication(bool forcehttpauthentication)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    forcehttpauthentication_ = forcehttpauthentication;
  }

  inline bool GetForceHttpAuthentication() const
  {
    return forcehttpauthentication_;
  }

  inline void SetProxyParams(const sock::ProxyParams& proxyparams)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    proxyparams_ = proxyparams;
  }

  inline const sock::ProxyParams GetProxyParams()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return proxyparams_;
  }

  void SetTimeOffset(uint64_t offset) { offset_ = offset; }
  uint64_t GetTimeOffset() const { return offset_; }

 protected:

  virtual void Update(T operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) = 0;
  virtual void SignalError(T operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) = 0;

  template<class Class>
  boost::optional<Class> GetClass(const pugi::xpath_node& parent, const char* xpath)
  {
    auto node = parent.node().select_node(xpath);
    if (node)
    {

      return Class(node.node());
    }

    return boost::none;
  }

  boost::optional<std::string> GetText(const pugi::xpath_node& parent, const char* xpath)
  {
    auto node = parent.node().select_node(xpath);
    if (node)
    {

      return std::string(node.node().text().get());
    }

    return boost::none;
  }

  boost::optional<bool> GetBool(const pugi::xpath_node& parent, const char* xpath)
  {
    auto node = parent.node().select_node(xpath);
    if (node)
    {

      return node.node().text().as_bool();
    }

    return boost::none;
  }

  boost::optional<int> GetInt(const pugi::xpath_node& parent, const char* xpath)
  {
    auto node = parent.node().select_node(xpath);
    if (node)
    {

      return node.node().text().as_int();
    }

    return boost::none;
  }
  
  boost::optional<float> GetFloat(const pugi::xpath_node& parent, const char* xpath)
  {
    auto node = parent.node().select_node(xpath);
    if (node)
    {

      return node.node().text().as_float();
    }

    return boost::none;
  }

 protected:
  
  uint64_t offset_; // The offset of the current utc clock, to the utc clock on the device, in milliseconds

 private:

  CURL* SendCommand(Request<T>* request, T operation, const bool authentication, const bool event, const std::string& action, const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body, const std::map<std::string, std::vector<char> >& mtomdatas)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if (curl_easy_setopt(request->handle_, CURLOPT_URL, address_.c_str()))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_SSL_VERIFYHOST, 0))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_SSL_VERIFYPEER, 0))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_PROXY_SSL_VERIFYHOST, 0))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_PROXY_SSL_VERIFYPEER, 0))
    {
      delete request;
      return nullptr;
    }

    // Headers
    request->FreeHeaders();
    if (mtomdatas.empty())
    {
      // Regular message
      request->headers_ = curl_slist_append(request->headers_, (std::string("Content-Type: application/soap+xml; charset=utf-8; action=\"") + action + std::string("\"")).c_str());
      if (!request->headers_)
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_COPYPOSTFIELDS, CreateEnvelope(authentication, event, action, referenceparameters, to, body).c_str()))
      {
        delete request;
        return nullptr;
      }
    }
    else
    {
      // MTOM message
      const std::string boundary = utility::GenerateRandomString(40);
    
      // Add main SOAP stuff
      std::vector<char> content;
      const std::string contentboundary(std::string("--") + boundary);
      const std::string newline("\r\n");
      content.insert(content.end(), newline.begin(), newline.end());
      content.insert(content.end(), contentboundary.begin(), contentboundary.end());
      content.insert(content.end(), newline.begin(), newline.end());

      const std::string soapheaders = "Content-ID: <soap@soap>\r\nContent-Type: application/xop+xml; charset=UTF-8; type=\"application/soap+xml\"\r\n\r\n";
      content.insert(content.end(), soapheaders.begin(), soapheaders.end());
      const std::string envelope = CreateEnvelope(authentication, event, action, referenceparameters, to, body);
      content.insert(content.end(), envelope.begin(), envelope.end());

      // MTOM data
      for (const auto& mtomdata : mtomdatas)
      {
        content.insert(content.end(), newline.begin(), newline.end());
        content.insert(content.end(), contentboundary.begin(), contentboundary.end());
        content.insert(content.end(), newline.begin(), newline.end());
        const std::string binaryheaders = std::string("Content-ID: <") + mtomdata.first + std::string(">\r\nContent-Transfer-Encoding: binary\r\nContent-Type: application/octet-stream\r\n\r\n");
        content.insert(content.end(), binaryheaders.begin(), binaryheaders.end());
        content.insert(content.end(), mtomdata.second.begin(), mtomdata.second.end());
      }

      content.insert(content.end(), contentboundary.begin(), contentboundary.end());
      content.push_back('-');
      content.push_back('-');
      content.insert(content.end(), newline.begin(), newline.end());

      // Headers
      request->headers_ = curl_slist_append(request->headers_, (std::string("Content-Type: multipart/related; type=\"application/xop+xml\"; start=\"<soap@soap>\"; boundary=\"" + boundary + "\"; start-info=\"application/soap+xml\"; action=\"") + action + std::string("\"")).c_str());
      if (!request->headers_)
      {
        delete request;
        return nullptr;
      }

      request->headers_ = curl_slist_append(request->headers_, (std::string("Content-length: ") + std::to_string(content.size())).c_str());
      if (!request->headers_)
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_POSTFIELDSIZE, static_cast<long>(content.size())))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_COPYPOSTFIELDS, content.data()))
      {
        delete request;
        return nullptr;
      }
    }

    request->headers_ = curl_slist_append(request->headers_, "Expect:");
    if (!request->headers_)
    {
      delete request;
      return nullptr;
    }

    request->headers_ = curl_slist_append(request->headers_, "Accept: */*");
    if (!request->headers_)
    {
      delete request;
      return nullptr;
    }

    if (forbidreuse_)
    {
      request->headers_ = curl_slist_append(request->headers_, "Connection: Close");
      if (!request->headers_)
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_FORBID_REUSE, 1))
      {
        delete request;
        return nullptr;
      }
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_TCP_NODELAY, 1))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_HTTPHEADER, request->headers_))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_ACCEPT_ENCODING, "gzip, deflate"))
    {
      delete request;
      return nullptr;
    }

    if (proxyparams_.GetType() == sock::PROXYTYPE_HTTP)
    {
      if (curl_easy_setopt(request->handle_, CURLOPT_PROXY, proxyparams_.ToString().c_str()))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_HTTPPROXYTUNNEL, 1))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_SUPPRESS_CONNECT_HEADERS, 1))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_PROXYAUTH, CURLAUTH_ANYSAFE))
      {
        delete request;
        return nullptr;
      }
    }
    else if (proxyparams_.GetType() == sock::PROXYTYPE_SOCKS5)
    {
      if (curl_easy_setopt(request->handle_, CURLOPT_PROXY, proxyparams_.ToString().c_str()))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_HTTPPROXYTUNNEL, 0))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_PROXYAUTH, CURLAUTH_ANY))
      {
        delete request;
        return nullptr;
      }
    }
    else
    {
      if (curl_easy_setopt(request->handle_, CURLOPT_PROXY, ""))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_HTTPPROXYTUNNEL, 0))
      {
        delete request;
        return nullptr;
      }

      if (curl_easy_setopt(request->handle_, CURLOPT_PROXYAUTH, CURLAUTH_ANY))
      {
        delete request;
        return nullptr;
      }
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_HTTPAUTH, CURLAUTH_ANY))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_USERNAME, username_.c_str()))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_PASSWORD, password_.c_str()))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_HEADERFUNCTION, SendCommandHeaderCallback))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_HEADERDATA, &request->responseheaders_))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_WRITEFUNCTION, SendCommandBodyCallback))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_WRITEDATA, &request->responsebody_))
    {
      delete request;
      return nullptr;
    }

    if (curl_easy_setopt(request->handle_, CURLOPT_PRIVATE, request))
    {
      delete request;
      return nullptr;
    }

    request->operation_ = operation;
    request->latency_ = std::chrono::high_resolution_clock::now();

    if (curl_multi_add_handle(curlm_, request->handle_))
    {
      delete request;
      return nullptr;
    }
    ++numconcurrentrequests_;

    return request->handle_;
  }

  std::string CreateEnvelope(bool authentication, const bool event, const std::string& action, const std::vector<Element>& referenceparameters, const std::string& to, const std::string& body)
  {
    return std::string("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:a=\"http://www.w3.org/2005/08/addressing\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:media=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:deviceio=\"http://www.onvif.org/ver10/deviceIO/wsdl\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" xmlns:ptz=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" xmlns:trv=\"http://www.onvif.org/ver10/receiver/wsdl\" xmlns:trp1=\"http://www.onvif.org/ver10/replay/wsdl\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:tns1=\"http://www.onvif.org/ver10/topics\">" + CreateHeader(authentication, event, action, referenceparameters, to) + body + "</s:Envelope>");
  }

  std::string CreateHeader(const bool authentication, const bool event, const std::string& action, const std::vector<Element>& referenceparameters, const std::string& to)
  {
    if (!authentication)
    {

      return std::string();
    }

    if (forcehttpauthentication_)
    {

      return std::string();
    }

    std::string header;
    if (username_.size())
    {
      const int64_t utctimemilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - offset_;
      const int64_t utctimeseconds = utctimemilliseconds / 1000;
      tm* utcunixtime = gmtime(reinterpret_cast<const time_t*>(&utctimeseconds));
      if (utcunixtime == nullptr)
      {
        time_t t = time(NULL);
        utcunixtime = gmtime(&t);
      }
      
      std::stringstream month; month << std::setfill('0') << std::setw(2) << utcunixtime->tm_mon + 1;
      std::stringstream day; day << std::setfill('0') << std::setw(2) << utcunixtime->tm_mday;
      std::stringstream hours; hours << std::setfill('0') << std::setw(2) << utcunixtime->tm_hour;
      std::stringstream minutes; minutes << std::setfill('0') << std::setw(2) << utcunixtime->tm_min;
      std::stringstream seconds; seconds << std::setfill('0') << std::setw(2) << utcunixtime->tm_sec;
      std::stringstream milliseconds; milliseconds << std::setfill('0') << std::setw(2) << (utctimemilliseconds % 1000);
      const std::string time = std::to_string(utcunixtime->tm_year + 1900) + '-' + month.str() + '-' + day.str() + 'T' + hours.str() + ':' + minutes.str() + ':' + seconds.str() + '.' + milliseconds.str().substr(0, 3) + 'Z';
      
      const std::vector<char> nonce = utility::GenerateRandomData<char>(20);
      std::vector<char> passwordstring(nonce.begin(), nonce.end());
      passwordstring.insert(passwordstring.end(), time.begin(), time.end());
      passwordstring.insert(passwordstring.end(), password_.begin(), password_.end());

      unsigned char passwordhash[20];
      SHA1(reinterpret_cast<const unsigned char*>(passwordstring.data()), passwordstring.size(), passwordhash);
      
      std::string eventheader;
      if (event)
      {
        eventheader = "<a:Action s:mustUnderstand=\"1\">" + action + "</a:Action><a:MessageID>urn:uuid:" + boost::uuids::to_string(randomgenerator_()) + "</a:MessageID><a:ReplyTo><a:Address>http://www.w3.org/2005/08/addressing/anonymous</a:Address></a:ReplyTo>";
        for (const auto& referenceparameter : referenceparameters)
        {
          eventheader += referenceparameter.ToXml();

        }
        if (to.size())
        {
          eventheader += "<a:To s:mustUnderstand=\"1\">" + to + "</a:To>";

        }
      }

      header = "<s:Header>" + eventheader +
                 "<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">"
                   "<UsernameToken>"
                     "<Username>" + username_ + "</Username>"
                     "<Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">" + utility::Base64Encode(passwordhash, 20) + "</Password>"
                     "<Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">" + utility::Base64Encode(reinterpret_cast<const unsigned char*>(nonce.data()), nonce.size()) + "</Nonce>"
                     "<Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">" + time + "</Created>"
                   "</UsernameToken>"
                 "</Security>"
               "</s:Header>";
    }
    return header;
  }

  std::recursive_mutex& mutex_;

  const boost::regex contenttypemultipart_;
  const boost::regex contenttypeparameters_;
  const boost::regex contenttype_;
  const boost::regex contentid_;

  boost::signals2::signal<void(const boost::posix_time::time_duration&)> latencysignal_;

  std::vector<QUEUEREQUEST> queue_;

  CURLM* curlm_;

  unsigned int maxconcurrentrequests_; // Zero represents unlimited concurrent requests
  unsigned int numconcurrentrequests_;

  bool forcehttpauthentication_;
  bool forbidreuse_;
  
  sock::ProxyParams proxyparams_;
  std::string address_;
  std::string username_;
  std::string password_;

  boost::uuids::random_generator randomgenerator_;

};

}

#pragma warning(pop)

#endif
