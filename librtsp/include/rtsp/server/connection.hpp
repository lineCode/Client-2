// connection.hpp
//

#ifndef IDGVCTDSKFJE3VUASDJKAD9KA8VENUP10I
#define IDGVCTDSKFJE3VUASDJKAD9KA8VENUP10I

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/enable_shared_from_this.hpp>
#include <boost/regex.hpp>
#include <memory>
#include <mutex>
#include <string>

#include "interleaved.hpp"
#include "rtsp/rtsprequest.hpp"
#include "rtsp/rtspresponse.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Enumerations /////

enum CONNECTIONTYPE
{
  CONNECTION_RTSP,
  CONNECTION_HTTP
};

///// Declarations /////

class Server;
class Session;
class UserConnection;

extern const std::string XSESSIONCOOKIE;

///// Classes /////

class Connection : public boost::enable_shared_from_this<Connection>
{
 public:

  Connection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection);
  virtual ~Connection();

  virtual void Destroy();

  virtual CONNECTIONTYPE GetConnectionType() const = 0;

  virtual void Connected();

  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 1>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 2>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 3>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 4>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 5>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 6>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 7>& buffers);
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 8>& buffers);
  virtual boost::system::error_code Write(const char* data, const size_t size);
  virtual boost::system::error_code Write(const std::string& data);

  boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
  boost::asio::streambuf& GetBuffer() { return buffer_; }

  std::string GetNonce() const { return nonce_; };

  inline InterleavedChannel& GetInterleavedChannel(const uint8_t channel) { std::lock_guard<std::mutex> lock(interleavedchannelsmutex_); return interleavedchannels_.at(channel); }
  inline void SetInterleavedChannelInUse(const uint8_t channel, const InterleavedChannel& interleavedchannel) { std::lock_guard<std::mutex> lock(interleavedchannelsmutex_); interleavedchannels_.at(channel) = interleavedchannel; }

  std::string GetLocalAddress() const;
  uint16_t GetLocalPort() const;

 protected:

  void HandleRTSPRead(const boost::system::error_code& error, std::size_t size);
  void HandleRTSPRtpHeader(const boost::system::error_code& err, std::size_t);
  void HandleRTSPRtpData(const boost::system::error_code& err, std::size_t size, uint8_t channel, uint16_t rtpdatasize);
  void HandleRTSPContent(const boost::system::error_code& err, std::size_t, RtspRequest request);

  void ParseRequest(const RtspRequest& request);
  void DestroyConnection();
  std::string BadHTTPRequest() const;

  const boost::regex delimeter_;
  const std::string nonce_;

  boost::shared_ptr<Server> server_;

  boost::shared_ptr<UserConnection> userconnection_;

  std::mutex writemutex_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf buffer_;

  mutable std::mutex interleavedchannelsmutex_;
  std::array<InterleavedChannel, 256> interleavedchannels_;

};

}

}

#endif
