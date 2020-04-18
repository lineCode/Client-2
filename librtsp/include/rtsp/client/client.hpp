// client.hpp
//

#ifndef IDG0ZY44MANDKVU72JDZAN9KA8VENUP10I
#define IDG0ZY44MANDKVU72JDZAN9KA8VENUP10I

///// Defines /////

#ifdef _WIN32
  #pragma warning(disable : 4503)
#endif

///// Includes /////

#include <boost/asio.hpp>

#include <algorithm>
#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include <boost/signals2.hpp>
#include <chrono>
#include <deque>
#include <future>
#include <memory>
#include <mutex>
#include <openssl/md5.h>
#include <socket/connection.hpp>
#include <socket/proxyparams.hpp>
#include <socket/socket.hpp>
#include <socket/tcpsocket.hpp>
#include <stdint.h>
#include <string>
#include <thread>
#include <utility>
#include <utility/utility.hpp>
#include <vector>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <mstcpip.h>
#endif

#include "rtsp/client/clientrequest.hpp"
#include "rtsp/client/session.hpp"
#include "rtsp/client/signal.hpp"
#include "rtsp/describeresponse.hpp"
#include "rtsp/getparameterresponse.hpp"
#include "rtsp/headers/authenticationtype.hpp"
#include "rtsp/headers/interleaved.hpp"
#include "rtsp/headers/parameters.hpp"
#include "rtsp/headers/range.hpp"
#include "rtsp/headers/rtpinfo.hpp"
#include "rtsp/optionsresponse.hpp"
#include "rtsp/pauseresponse.hpp"
#include "rtsp/playresponse.hpp"
#include "rtsp/rtsprequest.hpp"
#include "rtsp/rtspresponse.hpp"
#include "rtsp/sdp/sdp.hpp"
#include "rtsp/setparameterresponse.hpp"
#include "rtsp/setupresponse.hpp"
#include "rtsp/teardownresponse.hpp"

///// Declarations /////

namespace sock
{

class TcpSocket;

}

///// Namespaces /////

namespace rtsp
{

///// Classes /////

template<class T>
class Client : public boost::enable_shared_from_this< Client<T> >
{
 public:

  friend class client::Signal<Client, DescribeResponse>;
  friend class client::Signal<Client, GetParameterResponse>;
  friend class client::Signal<Client, OptionsResponse>;
  friend class client::Signal<Client, PauseResponse>;
  friend class client::Signal<Client, PlayResponse>;
  friend class client::Signal<Client, SetParameterResponse>;
  friend class client::Signal<Client, SetupResponse>;
  friend class client::Signal<Client, TeardownResponse>;

  Client(const boost::shared_ptr<T>& rtpcallbackdataobject, const boost::shared_ptr<std::recursive_mutex>& mutex, boost::asio::io_service& io, const boost::posix_time::seconds& timeout, const boost::posix_time::seconds& maxkeepalive) :
    rtpcallbackdataobject_(rtpcallbackdataobject),
    mutex_(mutex),
    asyncread_("(?:\\$|RTSP\\/1\\.0)"),
    ssrc_(rand()),
    io_(io),
    strand_(io_),
    timeout_(timeout),
    maxkeepalive_(maxkeepalive),
    socket_(boost::make_shared<sock::TcpSocket>(io_)),
    port_(554),
    authenticationtype_(headers::AUTHENTICATIONTYPE_INVALID),
    currentcseq_(0)
  {

  }

  ~Client()
  {
    Destroy();

  }

  void Init(const sock::ProxyParams& proxyparams, const std::string& host, uint16_t port, const std::string& username, const std::string& password)
  {
    proxyparams_ = proxyparams;
    host_ = host;
    port_ = port;

    username_ = username;
    password_ = password;
    realm_.clear();
    nonce_.clear();

    describesignal_ = std::make_unique< client::Signal<Client, DescribeResponse> >(shared_from_this());
    getparametersignal_ = std::make_unique< client::Signal<Client, GetParameterResponse> >(shared_from_this());
    optionssignal_ = std::make_unique< client::Signal<Client, OptionsResponse> >(shared_from_this());
    pausesignal_ = std::make_unique< client::Signal<Client, PauseResponse> >(shared_from_this());
    playsignal_ = std::make_unique< client::Signal<Client, PlayResponse> >(shared_from_this());
    setparametersignal_ = std::make_unique< client::Signal<Client, SetParameterResponse> >(shared_from_this());
    setupsignal_ = std::make_unique< client::Signal<Client, SetupResponse> >(shared_from_this());
    teardownsignal_ = std::make_unique< client::Signal<Client, TeardownResponse> >(shared_from_this());
  }

  void Destroy()
  {
    Disconnect();

    {
      // Kill all requests while notifying all listeners that it has been aborted
      std::lock_guard<std::recursive_mutex> lock(*mutex_);

      if (describesignal_)
      {
        describesignal_->Destroy();
        describesignal_.reset();
      }
      if (getparametersignal_)
      {
        getparametersignal_->Destroy();
        getparametersignal_.reset();
      }
      if (optionssignal_)
      {
        optionssignal_->Destroy();
        optionssignal_.reset();
      }
      if (pausesignal_)
      {
        pausesignal_->Destroy();
        pausesignal_.reset();
      }
      if (playsignal_)
      {
        playsignal_->Destroy();
        playsignal_.reset();
      }
      if (setparametersignal_)
      {
        setparametersignal_->Destroy();
        setparametersignal_.reset();
      }
      if (setupsignal_)
      {
        setupsignal_->Destroy();
        setupsignal_.reset();
      }
      if (teardownsignal_)
      {
        teardownsignal_->Destroy();
        teardownsignal_.reset();
      }

      proxyparams_.Clear();
      host_.clear();
      port_ = 0;

      authenticationtype_ = headers::AUTHENTICATIONTYPE_INVALID;
      username_.clear();
      password_.clear();
      realm_.clear();
      nonce_.clear();

      currentcseq_ = 0;

      interleavedrtpclients_.fill(nullptr);
    }

    {
      std::lock_guard<std::mutex> lock(clientrequestsmutex_);
      clientrequests_.clear();
    }

    {
      std::lock_guard<std::mutex> sessionsmutex(sessionsmutex_);
      for (auto& session : sessions_)
      {
        session->Destroy();

      }
      sessions_.clear();
    }

    latencysignal_.disconnect_all_slots();
  }

  const sock::ProxyParams& GetProxyParams() const { return proxyparams_; }
  const std::string& GetHost() const { return host_; }
  uint16_t GetPort() const { return port_; }

  sock::Connection Connect(const boost::function<void(const boost::system::error_code&)> connectcallback, const boost::function<void()> disconnectcallback)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);

    disconnectcallback_ = disconnectcallback;

    currentcseq_ = 0;
    return socket_->Connect(proxyparams_, host_, port_, [this, connectcallback](const boost::system::error_code& err, const boost::shared_ptr<sock::ConnectionBlock>&)
    {
      connectcallback(err);

      if (err)
      {
        Disconnect();
        Destroy();
      }
      else
      {
        boost::system::error_code er;
        socket_->GetSocket().set_option(boost::asio::socket_base::keep_alive(true), er);
        if (er)
        {
          disconnectcallback_();
          return;
        }

        // Setup TCP keepalive
        const unsigned int timeout = 1000;
        const unsigned int interval = 1000;
#if defined _WIN32
        DWORD ret = 0;
        struct tcp_keepalive alive =
        {
          TRUE,
          timeout,
          interval
        };
        WSAIoctl(socket_->GetSocket().native_handle(), SIO_KEEPALIVE_VALS, &alive, sizeof(alive), NULL, 0, &ret, NULL, NULL);
#else
        int keepcnt = 5;
        int keepidle = timeout;
        int keepintvl = interval;
        setsockopt(socket_->GetSocket().native_handle(), IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
        setsockopt(socket_->GetSocket().native_handle(), IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
        setsockopt(socket_->GetSocket().native_handle(), IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));
#endif
        response_.consume(response_.size());
        boost::asio::async_read_until(socket_->GetSocket(), response_, asyncread_, strand_.wrap(boost::bind(&Client::HandleRead, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
      }
    });
  }

  void Disconnect()
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);

    boost::system::error_code err;
    disconnectcallback_.clear();
    socket_->GetSocket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
    socket_->GetSocket().close(err);
  }

  inline void SetTimeout(const boost::posix_time::seconds& timeout) { timeout_ = timeout; }
  inline const boost::posix_time::seconds& GetTimeout() const { return timeout_; }

  boost::asio::io_service& GetIoService() { return io_; }
  boost::asio::io_service::strand& GetIoStrand() { return strand_; }

  void Describe(const std::string& url)
  {
    Request(DescribeRequest(url));
  }

  void GetParameter(const std::string& url, const std::string& session, const std::vector<std::string>& parameters)
  {
    Request(GetParameterRequest(url, session, parameters));
  }

  void Options(const std::string& url)
  {
    Request(OptionsRequest(url));
  }
  void Pause(const std::string& url, const std::string& session)
  {
    Request(PauseRequest(url, session));
  }

  void Play(const std::string& url, const std::string& session, const headers::Range& range)
  {
    Request(PlayRequest(url, session, range));
  }

  void SetParameter(const std::string& url, const std::string& session, const std::vector<headers::Parameter>& parameters)
  {
    Request(SetParameterRequest(url, session, parameters));
  }

  void Setup(const std::string& url, sdp::ADDRESSTYPE addresstype, headers::PROTOCOLTYPE protocoltype, headers::ROUTINGTYPE routingtype, headers::MODE mode, double rtptimestampfrequency, KEEPALIVEMODE keepalivemode, const std::string& keepaliveurl, RtpCallback rtpcallback, RtcpCallback rtcpcallback)
  {
    const auto setuprequest = SetupRequest(url, addresstype, protocoltype, routingtype, mode, rtptimestampfrequency, keepalivemode, keepaliveurl, rtpcallback, rtcpcallback);
    if (setuprequest.is_initialized())
    {
      Request(*setuprequest);

    }
  }

  void Teardown(const std::string& url, const std::string& session)
  {
    Request(TeardownRequest(url, session));
  }

  std::future<DescribeResponse> DescribeFuture(const std::string& url)
  {
    return describesignal_->CreateFuture(DescribeRequest(url));
  }

  std::future<GetParameterResponse> GetParameterFuture(const std::string& url, const std::string& session, const std::vector<std::string>& parameters)
  {
    return getparametersignal_->CreateFuture(GetParameterRequest(url, session, parameters));
  }

  std::future<OptionsResponse> OptionsFuture(const std::string& url)
  {
    return optionssignal_->CreateFuture(OptionsRequest(url));
  }

  std::future<PauseResponse> PauseFuture(const std::string& url, const std::string& session)
  {
    return pausesignal_->CreateFuture(PauseRequest(url, session));
  }

  std::future<PlayResponse> PlayFuture(const std::string& url, const std::string& session, const headers::Range& range)
  {
    return playsignal_->CreateFuture(PlayRequest(url, session, range));
  }

  std::future<SetParameterResponse> SetParameterFuture(const std::string& url, const std::string& session, const std::vector<headers::Parameter>& parameters)
  {
    return setparametersignal_->CreateFuture(SetParameterRequest(url, session, parameters));
  }

  std::future<SetupResponse> SetupFuture(const std::string& url, sdp::ADDRESSTYPE addresstype, headers::PROTOCOLTYPE protocoltype, headers::ROUTINGTYPE routingtype, headers::MODE mode, double rtptimestampfrequency, KEEPALIVEMODE keepalivemode, const std::string& keepaliveurl, RtpCallback rtpcallback, void* rtpcallbackdata, RtcpCallback rtcpcallback)
  {
    const auto setuprequest = SetupRequest(url, addresstype, protocoltype, routingtype, mode, rtptimestampfrequency, keepalivemode, keepaliveurl, rtpcallback, rtpcallbackdata, rtcpcallback);
    if (!setuprequest.is_initialized())
    {
      std::promise<SetupResponse> promise;
      std::future<SetupResponse> future = promise.get_future();
      promise.set_exception(std::make_exception_ptr(boost::system::errc::protocol_error));
      return future;
    }

    return setupsignal_->CreateFuture(*setuprequest);
  }

  std::future<TeardownResponse> TeardownFuture(const std::string& url, const std::string& session)
  {
    return teardownsignal_->CreateFuture(TeardownRequest(url, session));
  }

  client::Connection DescribeCallback(const std::string& url, typename client::Signal<Client<T>, DescribeResponse>::CallbackType callback)
  {
    return describesignal_->CreateCallback(DescribeRequest(url), callback);
  }

  client::Connection GetParameterCallback(const std::string& url, const std::string& session, const std::vector<std::string>& parameters, typename client::Signal<Client<T>, GetParameterResponse>::CallbackType callback)
  {
    return getparametersignal_->CreateCallback(GetParameterRequest(url, session, parameters), callback);
  }

  client::Connection OptionsCallback(const std::string& url, typename client::Signal<Client<T>, OptionsResponse>::CallbackType callback)
  {
    return optionssignal_->CreateCallback(OptionsRequest(url), callback);
  }

  client::Connection PauseCallback(const std::string& url, const std::string& session, typename client::Signal<Client<T>, PauseResponse>::CallbackType callback)
  {
    return pausesignal_->CreateCallback(PauseRequest(url, session), callback);
  }

  client::Connection PlayCallback(const std::string& url, const std::string& session, const headers::Range& range, typename client::Signal<Client<T>, PlayResponse>::CallbackType callback)
  {
    return playsignal_->CreateCallback(PlayRequest(url, session, range), callback);
  }

  client::Connection SetParameterCallback(const std::string& url, const std::string& session, const std::vector<headers::Parameter>& parameters, typename client::Signal<Client<T>, SetParameterResponse>::CallbackType callback)
  {
    return setparametersignal_->CreateCallback(SetParameterRequest(url, session, parameters), callback);
  }

  client::Connection SetupCallback(const std::string& url, sdp::ADDRESSTYPE addresstype, headers::PROTOCOLTYPE protocoltype, headers::ROUTINGTYPE routingtype, headers::MODE mode, double rtptimestampfrequency, KEEPALIVEMODE keepalivemode, const std::string& keepaliveurl, RtpCallback rtpcallback, void* rtpcallbackdata, RtcpCallback rtcpcallback, typename client::Signal<Client<T>, SetupResponse>::CallbackType callback)
  {
    const auto setuprequest = SetupRequest(url, addresstype, protocoltype, routingtype, mode, rtptimestampfrequency, keepalivemode, keepaliveurl, rtpcallback, rtpcallbackdata, rtcpcallback);
    if (!setuprequest.is_initialized())
    {
      callback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), SetupResponse());
      return client::Connection();
    }
    else
    {

      return setupsignal_->CreateCallback(*setuprequest, callback);
    }
  }

  client::Connection TeardownCallback(const std::string& url, const std::string& session, typename client::Signal<Client<T>, TeardownResponse>::CallbackType callback)
  {
    return teardownsignal_->CreateCallback(TeardownRequest(url, session), callback);
  }

  void HandleTimeout(const boost::system::error_code& err, uint64_t cseq)
  {
    RtspRequest request;
    uint64_t id = 0;
    std::chrono::high_resolution_clock::time_point latency;
    {
      std::lock_guard<std::mutex> lock(clientrequestsmutex_);
      auto clientrequest = std::find_if(clientrequests_.begin(), clientrequests_.end(), [&cseq](const ClientRequest& clientrequest) { return (cseq == clientrequest.request_.cseq_); });
      if (clientrequest == clientrequests_.end())
      {

        return;
      }
      request = clientrequest->request_;
      id = clientrequest->id_;
      latency = clientrequest->latency_;
      clientrequests_.erase(clientrequest);
    }

    if (!err)
    {
      Error(request, id, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - latency).count(), boost::system::errc::make_error_code(boost::system::errc::stream_timeout));

    }
  }

  void HandleSessionKeepalive(const boost::system::error_code& err, boost::shared_ptr< client::Session<T> > session, KEEPALIVEMODE keepalivemode, std::string url, unsigned int timeout)
  {
    if (err)
    {

      return;
    }

    if (keepalivemode == KEEPALIVEMODE_GETPARAMETER)
    {
      GetParameter(url, session->GetSession(), std::vector<std::string>());

    }
    else if (keepalivemode == KEEPALIVEMODE_SETPARAMETER)
    {
      SetParameter(url, session->GetSession(), std::vector<headers::Parameter>());

    }
    else
    {
      // Just in case...
      return;
    }

    if (session->keepalive_)
    {
      session->keepalive_->expires_from_now(boost::posix_time::seconds(timeout));
      session->keepalive_->async_wait(strand_.wrap(boost::bind(&Client::HandleSessionKeepalive, this->shared_from_this(), boost::asio::placeholders::error, session, keepalivemode, url, timeout)));
    }
  }

  void SetUsernamePassword(const std::string& username, const std::string& password)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    username_ = username;
    password_ = password;
  }

  inline const std::string& GetUsername() const { return username_; }
  inline const std::string& GetPassword() const { return password_; }

  sdp::ADDRESSTYPE GetSocketAddressType()
  {
    boost::system::error_code err;
    if (socket_->GetSocket().local_endpoint(err).address().is_v6())
    {

      return sdp::ADDRESSTYPE_IP6;
    }
    else
    {

      return sdp::ADDRESSTYPE_IP4;
    }
  }

  void HandleRead(const boost::system::error_code& err, std::size_t size)
  {
    if (err)
    {
      DisconnectCallback();
      Destroy();
      return;
    }
    
    if ((response_.in_avail() >= 1) && (*(boost::asio::buffer_cast<const char*>(response_.data() + (size - 1))) == '$'))
    {
      response_.consume(size);
      boost::asio::async_read(socket_->GetSocket(), response_, boost::asio::transfer_exactly(3), strand_.wrap(boost::bind(&Client::HandleRtpHeader, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
    }
    else if ((response_.in_avail() >= 8) && (std::string(boost::asio::buffer_cast<const char*>(response_.data() + (size - 8)), 8) == "RTSP/1.0"))
    {
      static const boost::regex asyncresponse("\r\n\r\n");
      boost::asio::async_read_until(socket_->GetSocket(), response_, asyncresponse, strand_.wrap(boost::bind(&Client::HandleResponse, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
      DisconnectCallback();
      Destroy();
      return;
    }
  }

  void HandleResponse(const boost::system::error_code& err, std::size_t size)
  {
    if (err)
    {
      DisconnectCallback();
      Destroy();
      return;
    }

    RtspResponse response(std::string(boost::asio::buffer_cast<const char*>(response_.data()), size));
    response_.consume(size);

    // Read any content if Content-Length header says too
    if (response.contentlength_.is_initialized() && (*response.contentlength_ > 0))
    {
      // Pull any extra bytes from the stream if we need too, otherwise just parse the response now
      const int readlength = static_cast<int>(*response.contentlength_) - static_cast<int>(response_.in_avail());
      if (readlength > 0)
      {
        boost::asio::async_read(socket_->GetSocket(), response_, boost::asio::transfer_exactly(readlength), strand_.wrap(boost::bind(&Client::HandleContent, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, response)));

      }
      else
      {
        response.ParseContent(std::string(boost::asio::buffer_cast<const char*>(response_.data()), *response.contentlength_));
        response_.consume(*response.contentlength_);

        ParseResponse(response);
      }
    }
    else
    {
      ParseResponse(response);

    }
  }

  void HandleContent(const boost::system::error_code& err, std::size_t, RtspResponse response)
  {
    if (err)
    {
      DisconnectCallback();
      Destroy();
      return;
    }

    response.ParseContent(std::string(boost::asio::buffer_cast<const char*>(response_.data()), *response.contentlength_));
    response_.consume(*response.contentlength_);

    ParseResponse(response);
  }

  void HandleRtpHeader(const boost::system::error_code& err, std::size_t)
  {
    if (err)
    {
      DisconnectCallback();
      Destroy();
      return;
    }

    uint8_t channel = 0;
    response_.sgetn(reinterpret_cast<char*>(&channel), 1);
    uint16_t numbytes = 0;
    response_.sgetn(reinterpret_cast<char*>(&numbytes), 2);
    numbytes = ntohs(numbytes);

    boost::asio::async_read(socket_->GetSocket(), response_, boost::asio::transfer_exactly(numbytes), strand_.wrap(boost::bind(&Client::HandleRtpData, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, channel, numbytes)));
  }

  void HandleRtpData(const boost::system::error_code& err, std::size_t, uint8_t channel, uint16_t rtpdatasize)
  {
    if (err)
    {
      DisconnectCallback();
      Destroy();
      return;
    }

    response_.sgetn(reinterpret_cast<char*>(buffer_.data()), rtpdatasize);

    const boost::shared_ptr< InterleavedRtpClient<T> > interleavedrtpclient = interleavedrtpclients_.at(channel);
    if (interleavedrtpclient)
    {
      if (channel & 0b1) // RTCP
      {
        rtcp::Packets response;
        unsigned int offset = 0;
        while (offset < rtpdatasize)
        {
          rtcp::Packet rtcppacket;
          if (rtcppacket.Init(buffer_.data() + offset, rtpdatasize - offset))
          {

            break;
          }

          boost::system::error_code err;
          boost::optional<rtcp::Packet> rtcpresponse = interleavedrtpclient->ParseRtcpPacket(socket_->GetSocket().remote_endpoint(err).address(), rtcppacket);
          if (rtcpresponse.is_initialized())
          {
            response.rtcppackets_.push_back(*rtcpresponse);

          }

          offset += (rtcppacket.length_ + 1) * 4;
        }

        if (response.rtcppackets_.size())
        {
          SendInterleavedRtcpPackets(channel, response);

        }
      }
      else // RTP
      {
        boost::system::error_code errc;
        interleavedrtpclient->ParseRtpPacket(err, socket_->GetSocket().remote_endpoint(errc).address(), buffer_.data(), rtpdatasize);
      }
    }

    boost::asio::async_read_until(socket_->GetSocket(), response_, asyncread_, strand_.wrap(boost::bind(&Client::HandleRead, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
  }

  boost::signals2::signal<void(const boost::posix_time::time_duration&)>& GetLatencySignal() { return latencysignal_; }

 private:

  void DisconnectCallback()
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    if (disconnectcallback_)
    {
      disconnectcallback_();

    }
  }

  std::pair<uint64_t, RtspRequest> DescribeRequest(const std::string& url)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_DESCRIBE, url, ++currentcseq_, headers::CONTENTTYPE_APPLICATIONSDP, std::string(), std::string(), boost::none, boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> GetParameterRequest(const std::string& url, const std::string& session, const std::vector<std::string>& parameters)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_GETPARAMETER, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), session, boost::none, boost::none, boost::none, headers::CONTENTTYPE_TEXTPARAMETERS, boost::none, parameters, headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> OptionsRequest(const std::string& url)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_OPTIONS, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), std::string(), boost::none, boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> PauseRequest(const std::string& url, const std::string& session)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_PAUSE, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), session, headers::Range(), boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> PlayRequest(const std::string& url, const std::string& session, const headers::Range& range)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_PLAY, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), session, range, boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> SetParameterRequest(const std::string& url, const std::string& session, const std::vector<headers::Parameter>& parameters)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_SETPARAMETER, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), session, boost::none, boost::none, boost::none, headers::CONTENTTYPE_TEXTPARAMETERS, boost::none, std::vector<std::string>(), parameters, KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  boost::optional< std::pair<uint64_t, RtspRequest> > SetupRequest(const std::string& url, sdp::ADDRESSTYPE addresstype, headers::PROTOCOLTYPE protocoltype, headers::ROUTINGTYPE routingtype, headers::MODE mode, double rtptimestampfrequency, KEEPALIVEMODE keepalivemode, const std::string& keepaliveurl, RtpCallback rtpcallback, void* rtpcallbackdata, RtcpCallback rtcpcallback)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);

    const uint64_t id = ++currentcseq_;
    boost::shared_ptr< client::Session<T> > session = boost::make_shared< client::Session<T> >(id, io_);

    headers::Interleaved interleave;
    std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> > clientport;
    if (protocoltype == headers::PROTOCOLTYPE_TCP)
    {
      boost::shared_ptr< InterleavedRtpClient<T> > rtpclient = CreateInterleavedRtpClient(rtptimestampfrequency, rtpcallback, rtpcallbackdata, rtcpcallback);
      if (!rtpclient)
      {

        return boost::none;
      }

      session->interleavedchannels_.push_back(std::make_pair(rtpclient->GetRtpChannel(), rtpclient->GetRtcpChannel()));
      interleave = headers::Interleaved(rtpclient->GetRtpChannel(), rtpclient->GetRtcpChannel());
    }
    else // (protocoltype == headers::PROTOCOLTYPE_UDP)
    {
      if (routingtype == headers::ROUTINGTYPE_UNICAST)
      {
        boost::shared_ptr< UDPUnicastRtpClient<T> > rtpclient = boost::make_shared< UDPUnicastRtpClient<T> >(ssrc_, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject_, rtpcallbackdata, rtcpcallback, io_, strand_);
        if (rtpclient->Init(addresstype))
        {

          return boost::none;
        }

        session->udpunicastrtpclients_.push_back(rtpclient);
        clientport = std::make_pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(rtpclient->GetRtpPort(), rtpclient->GetRtcpPort());
      }
      else if (routingtype == headers::ROUTINGTYPE_MULTICAST)
      {
        session->udpmulticastcastrtpclient_ = boost::make_shared< UDPMulticastRtpClient<T> >(ssrc_, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject_, rtpcallbackdata, rtcpcallback, io_, strand_);

      }
      else
      {

        return boost::none;
      }
    }

    {
      std::lock_guard<std::mutex> lock(sessionsmutex_);
      sessions_.emplace_back(std::move(session));
    }

    return std::make_pair(id, RtspRequest(headers::REQUESTTYPE_SETUP, url, id, headers::CONTENTTYPE_INVALID, headers::Transport({ headers::TransportData(std::string("RTP"), std::string("AVP"), protocoltype, routingtype, interleave, boost::asio::ip::address(), std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), clientport, std::pair< boost::optional<uint16_t>, boost::optional<uint16_t> >(boost::none, boost::none), std::string(), mode) }), std::string(), boost::none, boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), keepalivemode, keepaliveurl, authenticationtype_, url, username_, realm_, nonce_));
  }

  std::pair<uint64_t, RtspRequest> TeardownRequest(const std::string& url, const std::string& session)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    std::lock_guard<std::mutex> sessionslock(sessionsmutex_);

    // Destroy the session that matches this id
    auto i = std::find_if(sessions_.begin(), sessions_.end(), [session](const boost::shared_ptr< client::Session<T> >& s) { return (s->GetSession() == session); });
    if (i != sessions_.end())
    {
      for (const auto& udpunicastrtpclient : (*i)->udpunicastrtpclients_)
      {
        if (udpunicastrtpclient->GetSsrc().ssrc_.is_initialized())
        {
          udpunicastrtpclient->SendRtcpPackets(rtcp::Packets({ rtcp::Packet(false,{ *udpunicastrtpclient->GetSsrc().ssrc_ },{ 'e', 'n', 'd', ' ', 'o', 'f', ' ', 's', 't', 'r', 'e', 'a', 'm' }) }));

        }
      }

      // Clear interleaved channels of this session
      {
        for (const auto& interleavedchannel : (*i)->interleavedchannels_)
        {
          if (!interleavedrtpclients_.at(interleavedchannel.second))
          {

            continue;
          }

          auto ssrc = interleavedrtpclients_.at(interleavedchannel.second)->GetSsrc().ssrc_;
          if (ssrc.is_initialized()) // We may not have received the ssrc from the device yet
          {
            SendInterleavedRtcpPackets(interleavedchannel.second, rtcp::Packets({ rtcp::Packet(false,{ *interleavedrtpclients_.at(interleavedchannel.second)->GetSsrc().ssrc_ },{ 'e', 'n', 'd', ' ', 'o', 'f', ' ', 's', 't', 'r', 'e', 'a', 'm' }) }));

          }

          interleavedrtpclients_.at(interleavedchannel.first).reset();
          interleavedrtpclients_.at(interleavedchannel.second).reset();
        }
      }

      (*i)->Destroy();
      sessions_.erase(i);
    }

    return std::make_pair(currentcseq_, RtspRequest(headers::REQUESTTYPE_TEARDOWN, url, ++currentcseq_, headers::CONTENTTYPE_INVALID, std::string(), session, boost::none, boost::none, boost::none, headers::CONTENTTYPE_INVALID, boost::none, std::vector<std::string>(), headers::Parameters(), KEEPALIVEMODE_NONE, std::string(), authenticationtype_, url, username_, realm_, nonce_));
  }

  int Request(const std::pair<uint64_t, const RtspRequest>& request)
  {
    if (!socket_->GetSocket().is_open())
    {

      return 1;
    }

    {
      // Send the request
      std::lock_guard<std::mutex> lock(writemutex_);
      boost::system::error_code err;
      boost::asio::write(socket_->GetSocket(), boost::asio::buffer(request.second.ToString(password_)), err);
      if (err)
      {

        return 1;
      }
    }

    {
      std::lock_guard<std::mutex> lock(clientrequestsmutex_);
      clientrequests_.emplace_back(ClientRequest(request.second, request.first, std::chrono::high_resolution_clock::now(), std::make_unique<boost::asio::deadline_timer>(io_, timeout_)));
      clientrequests_.back().deadlinetimer_->async_wait(strand_.wrap(boost::bind(&Client::HandleTimeout, this->shared_from_this(), boost::asio::placeholders::error, *request.second.cseq_)));
    }

    return 0;
  }

  void ParseResponse(const RtspResponse& response)
  {
    // We have now finished reading, so lets async read any more data that may come our way
    boost::asio::async_read_until(socket_->GetSocket(), response_, asyncread_, strand_.wrap(boost::bind(&Client::HandleRead, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));

    if (!response.cseq_.is_initialized())
    {

      return;
    }

    // Find the original request
    clientrequestsmutex_.lock();
    auto i = std::find_if(clientrequests_.begin(), clientrequests_.end(), [&response](const ClientRequest& clientrequest) { return (*response.cseq_ == clientrequest.request_.cseq_); });
    if (i == clientrequests_.end())
    {
      clientrequestsmutex_.unlock();
      return;
    }

    // We're now dealing with the request, so lets remove it from the list and and the deadline_timer will die with the destructor of request
    ClientRequest clientrequest(std::move(*i));
    clientrequest.deadlinetimer_.reset();
    clientrequests_.erase(i);
    clientrequestsmutex_.unlock();

    const int64_t latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - clientrequest.latency_).count();

    latencysignal_(boost::posix_time::time_duration(boost::posix_time::milliseconds(latency)));

    // Check response code
    if (!response.code_.is_initialized())
    {
      Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::permission_denied));
      return;
    }

    if (*response.code_ == 401) // Unauthorized
    {
      const headers::WWWAuthenticate authentication = response.GetStrongestAuthentication();

      // If the response did not indicate any way to authenticate, fail now
      if (authentication.authenticationtype_ == headers::AUTHENTICATIONTYPE_INVALID)
      {
        Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::permission_denied));
        return;
      }

      // If we had already tried to authenticate and failed, fail instead of retrying. If the nonce is stale, we can retry again
      if ((authenticationtype_ == headers::AUTHENTICATIONTYPE_BASIC) || ((authenticationtype_ == headers::AUTHENTICATIONTYPE_DIGEST) && (!authentication.stale_.is_initialized() || (authentication.stale_ == false))))
      {
        authenticationtype_ = headers::AUTHENTICATIONTYPE_INVALID; // Reset authentication type so future calls are required to repeat the authentication process
        Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::permission_denied));
        return;
      }

      authenticationtype_ = authentication.authenticationtype_;
      realm_ = authentication.realm_;
      nonce_ = authentication.nonce_;

      // Resend the command
      if (Request(std::make_pair(clientrequest.id_, RtspRequest(clientrequest.request_.requesttype_, clientrequest.request_.url_, ++currentcseq_, clientrequest.request_.accepttype_, clientrequest.request_.transport_, clientrequest.request_.session_, clientrequest.request_.range_, clientrequest.request_.ratecontrol_, clientrequest.request_.immediate_, clientrequest.request_.contenttype_, clientrequest.request_.contentlength_, clientrequest.request_.getparameters_, clientrequest.request_.setparameters_, clientrequest.request_.keepalivemode_, clientrequest.request_.keepaliveurl_, authenticationtype_, clientrequest.request_.url_, username_, realm_, nonce_))))
      {
        DisconnectCallback();
        Destroy();
      }
      return;
    }
    else if (*response.code_ != 200 && *response.code_ != 201 && *response.code_ != 250) // 200, 201 and 250 are the only successful responses for RTSP
    {
      Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
      return;
    }

    // Signal to the listeners what happened
    switch (clientrequest.request_.requesttype_)
    {
      case headers::REQUESTTYPE_DESCRIBE:
      {
        if (!response.sdp_.is_initialized())
        {
          Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
          break;
        }

        describesignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success), response.contentbase_, *response.sdp_);
        break;
      }
      case headers::REQUESTTYPE_GETPARAMETER:
      {
        getparametersignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success), response.parameters_.parameters_);
        break;
      }
      case headers::REQUESTTYPE_OPTIONS:
      {
        optionssignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success), response.options_);
        break;
      }
      case headers::REQUESTTYPE_PAUSE:
      {
        pausesignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success));
        break;
      }
      case headers::REQUESTTYPE_PLAY:
      {
        playsignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success));
        break;
      }
      case headers::REQUESTTYPE_SETPARAMETER:
      {
        setparametersignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success), response.parameters_.parameters_);
        break;
      }
      case headers::REQUESTTYPE_SETUP:
      {
        if (response.session_.empty())
        {
          Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
          break;
        }

        // Make sure we have matching transports between request and response
        if (!std::is_permutation(clientrequest.request_.transport_.transports_.begin(), clientrequest.request_.transport_.transports_.end(), response.transport_.transports_.begin(), response.transport_.transports_.end(), [](const headers::TransportData& request, const headers::TransportData& response)
        {
          return ((request.transportprotocol_ == response.transportprotocol_) &&
            (request.profile_ == response.profile_) &&
            (request.GetProtocolType() == response.GetProtocolType()) &&
            (request.GetRoutingType() == response.GetRoutingType()) &&
            (request.interleaved_.rtp_.is_initialized() == response.interleaved_.rtp_.is_initialized()) &&
            (request.interleaved_.rtcp_.is_initialized() == response.interleaved_.rtcp_.is_initialized()) &&
            (request.clientport_ == response.clientport_) &&
            (request.GetMode() == response.GetMode()));
        }))
        {
          Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
          break;
        }

        // Set the session for all the rtp clients
        auto session = GetSession(clientrequest.id_);
        if (session)
        {
          session->SetSession(response.session_);

          // Setup the the keepalive
          if (clientrequest.request_.keepalivemode_ != KEEPALIVEMODE_NONE)
          {
            long tmp = static_cast<long>(response.timeout_ * 0.8);
            if (maxkeepalive_.total_seconds() < tmp)
            {
              tmp = static_cast<long>(maxkeepalive_.total_seconds());

            }
            session->keepalive_->expires_from_now(boost::posix_time::seconds(tmp));
            session->keepalive_->async_wait(strand_.wrap(boost::bind(&Client::HandleSessionKeepalive, this->shared_from_this(), boost::asio::placeholders::error, session, clientrequest.request_.keepalivemode_, clientrequest.request_.keepaliveurl_, tmp)));
          }

          if (AdjustInterleavedChannels(clientrequest.request_.transport_.transports_, response.transport_.transports_))
          {
            Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
            break;
          }

          for (auto& udpunicastrtpclient : session->udpunicastrtpclients_) // Find the matching udp unicast client, to then set the server_port if it exists
          {
            auto i = std::find_if(response.transport_.transports_.begin(), response.transport_.transports_.end(), [&udpunicastrtpclient](const headers::TransportData& transportdata)
            {
              return ((transportdata.transportprotocol_ == std::string("RTP")) &&
                (transportdata.profile_ == std::string("AVP")) &&
                (transportdata.GetProtocolType() == headers::PROTOCOLTYPE_UDP) &&
                (transportdata.GetRoutingType() == headers::ROUTINGTYPE_UNICAST) &&
                transportdata.clientport_.first.is_initialized() &&
                transportdata.clientport_.second.is_initialized() &&
                (*transportdata.clientport_.first == udpunicastrtpclient->GetRtpPort()) &&
                (*transportdata.clientport_.second == udpunicastrtpclient->GetRtcpPort()));
            });
            if (i == response.transport_.transports_.end())
            {

              continue;
            }

            if (i->ssrc_.ssrc_.is_initialized()) // RTSP and ONVIF say this MUST be initialised, but not all devices do, so we make the best of it
            {
              udpunicastrtpclient->SetSsrc(i->ssrc_);

            }

            if (i->serverport_.first.is_initialized() && i->serverport_.second.is_initialized())
            {
              boost::system::error_code err;
              udpunicastrtpclient->SetServerEndpoints(socket_->GetSocket().remote_endpoint(err).address(), *i->serverport_.first, *i->serverport_.second);
            }
          }

          // Sort out the multicast rtp clients
          for (const auto& transport : clientrequest.request_.transport_.transports_)
          {
            // Multicast
            if (transport.routingtype_ != headers::ROUTINGTYPE_MULTICAST)
            {

              continue;
            }

            auto responsetransport = std::find_if(response.transport_.transports_.begin(), response.transport_.transports_.end(), [](const headers::TransportData& transportdata) { return (transportdata.routingtype_ == headers::ROUTINGTYPE_MULTICAST); });
            if (responsetransport == response.transport_.transports_.end())
            {
              Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
              break;
            }

            if (responsetransport->destination_.is_unspecified() || !responsetransport->destination_.is_multicast() || !responsetransport->port_.first.is_initialized() || !responsetransport->port_.second.is_initialized())
            {
              Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
              break;
            }

            boost::system::error_code err;
            if (session->udpmulticastcastrtpclient_->Init(socket_->GetSocket().local_endpoint(err).address(), responsetransport->destination_, *responsetransport->port_.first, *responsetransport->port_.second))
            {
              Error(clientrequest.request_, clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::protocol_error));
              break;
            }
          }
        }

        // Retrieve the RtpClient in transit to becoming part of a session
        setupsignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success), response.session_, response.timeout_);
        break;
      }
      case headers::REQUESTTYPE_TEARDOWN:
      {
        teardownsignal_->Emit(clientrequest.id_, latency, boost::system::errc::make_error_code(boost::system::errc::success));
        break;
      }
      default:
      {
        assert(false);
        break;
      }
    }
  }

  boost::shared_ptr< InterleavedRtpClient<T> > CreateInterleavedRtpClient(double rtptimestampfrequency, RtpCallback rtpcallback, void* rtpcallbackdata, RtcpCallback rtcpcallback)
  {
    // Find two free adjacent channels
    for (int i = 0; i < interleavedrtpclients_.size(); i += 2)
    {
      if (!interleavedrtpclients_.at(i) && !interleavedrtpclients_.at(i + 1))
      {
        boost::shared_ptr< InterleavedRtpClient<T> > rtpclient = boost::make_shared< InterleavedRtpClient<T> >(ssrc_, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject_, rtpcallbackdata, rtcpcallback, i, i + 1);
        interleavedrtpclients_.at(i) = rtpclient;
        interleavedrtpclients_.at(i + 1) = rtpclient;
        return rtpclient;
      }
    }

    return nullptr;
  }

  void Error(const RtspRequest& rtsprequest, uint64_t id, uint64_t latency, const boost::system::error_code& error)
  {
    switch (rtsprequest.requesttype_)
    {
      case headers::REQUESTTYPE_DESCRIBE:
      {
        describesignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_GETPARAMETER:
      {
        getparametersignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_OPTIONS:
      {
        optionssignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_PAUSE:
      {
        pausesignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_PLAY:
      {
        playsignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_SETPARAMETER:
      {
        setparametersignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_SETUP:
      {
        {
          std::lock_guard<std::mutex> lock(sessionsmutex_);
          auto session = std::find_if(sessions_.begin(), sessions_.end(), [id](const boost::shared_ptr< client::Session<T> >& session) { return (session->GetID() == id); });
          if (session != sessions_.end())
          {
            sessions_.erase(session);

          }
        }

        setupsignal_->Emit(id, latency, error);
        break;
      }
      case headers::REQUESTTYPE_TEARDOWN:
      {
        teardownsignal_->Emit(id, latency, error);
        break;
      }
    }
  }

  int AdjustInterleavedChannels(std::vector<headers::TransportData> requesttransports, std::vector<headers::TransportData> responsetransports)
  {
    // Remove non interleaved transports
    requesttransports.erase(std::remove_if(requesttransports.begin(), requesttransports.end(), [](const headers::TransportData& transportdata) { return (!transportdata.interleaved_.rtp_.is_initialized() || !transportdata.interleaved_.rtcp_.is_initialized()); }), requesttransports.end());
    responsetransports.erase(std::remove_if(responsetransports.begin(), responsetransports.end(), [](const headers::TransportData& transportdata) { return (!transportdata.interleaved_.rtp_.is_initialized() || !transportdata.interleaved_.rtcp_.is_initialized()); }), responsetransports.end());

    // Clear any transports that match already
    for (auto request = requesttransports.begin(); request != requesttransports.end(); )
    {
      auto response = std::find_if(responsetransports.begin(), responsetransports.end(), [&request](const headers::TransportData& transportdata)
      {
        return ((request->transportprotocol_ == transportdata.transportprotocol_) &&
          (request->profile_ == transportdata.profile_) &&
          (request->GetProtocolType() == transportdata.GetProtocolType()) &&
          (request->GetRoutingType() == transportdata.GetRoutingType()) &&
          (request->interleaved_.rtp_ == transportdata.interleaved_.rtp_) &&
          (request->interleaved_.rtcp_ == transportdata.interleaved_.rtcp_) &&
          (request->clientport_ == transportdata.clientport_) &&
          (request->GetMode() == transportdata.GetMode()));
      });
      if (response == responsetransports.end())
      {
        ++request;

      }
      else
      {
        if (response->ssrc_.ssrc_.is_initialized())
        {
          interleavedrtpclients_.at(*response->interleaved_.rtp_)->SetSsrc(response->ssrc_);

        }

        request = requesttransports.erase(request);
        responsetransports.erase(response);
      }
    }

    for (const auto& request : requesttransports)
    {
      auto responsetransport = std::find_if(responsetransports.begin(), responsetransports.end(), [&request](const headers::TransportData& transportdata) // Find a corresponding response transport that appears reasonable
      {
        return ((request.transportprotocol_ == transportdata.transportprotocol_) &&
          (request.profile_ == transportdata.profile_) &&
          (request.GetProtocolType() == transportdata.GetProtocolType()) &&
          (request.GetRoutingType() == transportdata.GetRoutingType()) &&
          (request.interleaved_.rtp_.is_initialized() == transportdata.interleaved_.rtp_.is_initialized()) &&
          (request.interleaved_.rtcp_.is_initialized() == transportdata.interleaved_.rtcp_.is_initialized()) &&
          (request.clientport_ == transportdata.clientport_) &&
          (request.GetMode() == transportdata.GetMode()));
      });
      if (responsetransport == responsetransports.end())
      {

        return 1;
      }

      if (interleavedrtpclients_.at(*responsetransport->interleaved_.rtp_) || interleavedrtpclients_.at(*responsetransport->interleaved_.rtcp_)) // If the requested channel is already occupied
      {

        return 1;
      }

      if (responsetransport->ssrc_.ssrc_.is_initialized())
      {
        interleavedrtpclients_.at(*request.interleaved_.rtp_)->SetSsrc(responsetransport->ssrc_);

      }

      interleavedrtpclients_.at(*request.interleaved_.rtp_)->SetRtpChannel(*responsetransport->interleaved_.rtp_);
      interleavedrtpclients_.at(*request.interleaved_.rtcp_)->SetRtcpChannel(*responsetransport->interleaved_.rtcp_);
      interleavedrtpclients_.at(*responsetransport->interleaved_.rtp_) = interleavedrtpclients_.at(*request.interleaved_.rtp_);
      interleavedrtpclients_.at(*responsetransport->interleaved_.rtcp_) = interleavedrtpclients_.at(*request.interleaved_.rtcp_);
      interleavedrtpclients_.at(*request.interleaved_.rtp_).reset();
      interleavedrtpclients_.at(*request.interleaved_.rtcp_).reset();

      responsetransports.erase(responsetransport);
    }

    return 0;
  }

  int SendInterleavedRtcpPackets(uint8_t channel, const rtcp::Packets& rtcppackets)
  {
    std::lock_guard<std::mutex> lock(writemutex_);

    auto data = rtcppackets.ToData();
    const uint16_t size = htons(static_cast<uint16_t>(data.size()));
    data.insert(data.begin(), { '$', static_cast<char>(channel), reinterpret_cast<const char*>(&size)[0], reinterpret_cast<const char*>(&size)[1] });
    boost::system::error_code err;
    boost::asio::write(socket_->GetSocket(), boost::asio::buffer(data), err);
    if (err)
    {
      Destroy();
      return 1;
    }

    return 0;
  }

  boost::shared_ptr< client::Session<T> > GetSession(uint64_t id)
  {
    std::lock_guard<std::mutex> lock(sessionsmutex_);
    auto session = std::find_if(sessions_.begin(), sessions_.end(), [id](const boost::shared_ptr< client::Session<T> >& session) { return (session->GetID() == id); });
    if (session == sessions_.end())
    {

      return nullptr;
    }

    return (*session);
  }

  boost::shared_ptr<std::recursive_mutex>& GetMutex() { return mutex_; }

  const boost::regex asyncread_;
  const uint32_t ssrc_;

  boost::shared_ptr<T> rtpcallbackdataobject_;
  boost::shared_ptr<std::recursive_mutex> mutex_;

  boost::asio::io_service& io_;
  boost::asio::io_service::strand strand_;

  boost::posix_time::seconds timeout_;
  boost::posix_time::seconds maxkeepalive_;

  boost::shared_ptr<sock::TcpSocket> socket_;

  boost::asio::streambuf response_;
  std::array<char, std::numeric_limits<uint16_t>::max()> buffer_;

  boost::function<void()> disconnectcallback_;
  
  sock::ProxyParams proxyparams_;
  std::string host_;
  uint16_t port_;

  headers::AUTHENTICATIONTYPE authenticationtype_;
  std::string username_;
  std::string password_;
  std::string realm_;
  std::string nonce_;

  uint64_t currentcseq_;

  std::unique_ptr< client::Signal<Client, DescribeResponse> > describesignal_;
  std::unique_ptr< client::Signal<Client, GetParameterResponse >> getparametersignal_;
  std::unique_ptr< client::Signal<Client, OptionsResponse> > optionssignal_;
  std::unique_ptr< client::Signal<Client, PauseResponse> > pausesignal_;
  std::unique_ptr< client::Signal<Client, PlayResponse> > playsignal_;
  std::unique_ptr< client::Signal<Client, SetParameterResponse> > setparametersignal_;
  std::unique_ptr< client::Signal<Client, SetupResponse> > setupsignal_;
  std::unique_ptr< client::Signal<Client, TeardownResponse> > teardownsignal_;

  std::mutex clientrequestsmutex_;
  std::vector<ClientRequest> clientrequests_;

  std::array< boost::shared_ptr<InterleavedRtpClient<T> >, 256> interleavedrtpclients_;

  std::mutex sessionsmutex_;
  std::vector< boost::shared_ptr< client::Session<T> > > sessions_;

  std::mutex writemutex_;

  boost::signals2::signal<void(const boost::posix_time::time_duration&)> latencysignal_;

};

}

#endif
