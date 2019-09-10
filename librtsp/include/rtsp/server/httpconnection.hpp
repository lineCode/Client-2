// httpconnection.hpp
//

#ifndef ID1L5ZTNNQYKMFJ9GR7C182R5QCG16LE9I
#define ID1L5ZTNNQYKMFJ9GR7C182R5QCG16LE9I

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "connection.hpp"
#include "interleaved.hpp"
#include "rtsp/rtspresponse.hpp"
#include "session.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Session;

///// Classes /////

class HTTPConnection : public Connection
{
 public:

  HTTPConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection);
  virtual ~HTTPConnection();

  virtual void Destroy() override;

  inline virtual CONNECTIONTYPE GetConnectionType() const override { return CONNECTION_HTTP; }

  virtual void Connected() override;

  void HandleHTTPRead(const boost::system::error_code& error, std::size_t size);
  void HandleHTTPIgnore(const boost::system::error_code& error, std::size_t size);
  void HandleHTTPPost(const boost::system::error_code& error, std::size_t size);

  // These override to send the GET connection, instead of directly back down the POST
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 1>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 2>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 3>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 4>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 5>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 6>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 7>& buffers) override;
  virtual boost::system::error_code Write(const std::array<boost::asio::const_buffer, 8>& buffers) override;
  virtual boost::system::error_code Write(const char* data, const size_t size) override;
  virtual boost::system::error_code Write(const std::string& data) override;

  inline const std::string GetSessionCookie() const { std::lock_guard<std::mutex> lock(mutex_); return sessioncookie_; }

  inline boost::shared_ptr<HTTPConnection> GetGetConnection() const { std::lock_guard<std::mutex> lock(mutex_); return getconnection_; }

  std::string rtspbuffer_; // This holds the converted(from base64) text

 private:

  mutable std::mutex mutex_;

  std::string sessioncookie_;
  boost::shared_ptr<HTTPConnection> getconnection_;

};

}

}

#endif
