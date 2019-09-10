// httpconnection.cpp
//

///// Includes /////

#include "rtsp/server/httpconnection.hpp"

#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <utility/utility.hpp>

#include "rtsp/server/server.hpp"
#include "rtsp/server/userconnection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

HTTPConnection::HTTPConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection) :
  Connection(io, server, userconnection)
{
  
}

HTTPConnection::~HTTPConnection()
{

}

void HTTPConnection::Destroy()
{
  if (getconnection_)
  {
    getconnection_->Destroy();
    getconnection_ = nullptr;
  }
  Connection::Destroy();
}

void HTTPConnection::Connected()
{
  if (userconnection_)
  {
    userconnection_->Connected();

  }

  boost::asio::async_read_until(socket_, buffer_, std::string("\r\n\r\n"), boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void HTTPConnection::HandleHTTPRead(const boost::system::error_code& error, std::size_t size)
{
  if (error)
  {
    Destroy();
    return;
  }

  std::vector<std::string> headers;
  std::string http = std::string(boost::asio::buffer_cast<const char*>(buffer_.data()), size);
  static const boost::regex newlineregex("\r\n");
  boost::regex_split(std::back_inserter(headers), http, newlineregex);
  if (headers.empty())
  {
    Write(BadHTTPRequest());
    boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return;
  }

  static const boost::regex headerregex("(GET|POST)[\\s]+(.*)[\\s]+HTTP\\/1\\.(?:0|1)");
  boost::smatch match;
  if (!boost::regex_match(headers.front(), match, headerregex))
  {
    Write(BadHTTPRequest());
    boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return;
  }
  const std::string method = match[1].str();
  const std::string path = match[2].str();
  headers.erase(headers.begin());

  std::vector< std::pair<std::string, std::string> > splitheaders;
  for (const std::string& header : headers)
  {
    static const boost::regex splitheaderregex("(.*):[\\s]+(.*)");
    boost::smatch match;
    if (!boost::regex_match(header, match, splitheaderregex))
    {

      continue;
    }
    splitheaders.push_back(std::make_pair(match[1].str(), match[2].str()));
  }

  {
    boost::shared_ptr<Stream> stream = server_->GetStream(path);
    if (stream == nullptr)
    {
      Write(BadHTTPRequest());
      boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
      return;
    }

    if (method == "GET")
    {
      std::vector< std::pair<std::string, std::string> >::const_iterator splitheader = std::find_if(splitheaders.cbegin(), splitheaders.cend(), [](const std::pair<std::string, std::string>& splitheader) { return (splitheader.first == XSESSIONCOOKIE); });
      if (splitheader == splitheaders.cend())
      {
        Write(BadHTTPRequest());//TODO I think we need to destroy ourselves on these failures, maybe add a void Write() which does that?
        boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        return;
      }

      {
        std::lock_guard<std::mutex> lock(mutex_);
        sessioncookie_ = splitheader->second;
      }

      if (!server_->AddHTTPConnection(boost::static_pointer_cast<HTTPConnection>(shared_from_this())))
      {
        Write(BadHTTPRequest());
        boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        return;
      }

      const std::string response = "HTTP/1.0 200 OK\r\n"
                                   "Connection: close\r\n"
                                   "Cache-Control: no-store\r\n"
                                   "Pragma: no-cache\r\n"
                                   "Server: " + server_->GetRealm() + "\r\n"
                                   "X-Server-IP-Address: " + GetLocalAddress() + "\r\n"
                                   "Content-Type: application/x-rtsp-tunnelled\r\n\r\n";
      boost::system::error_code err;
      boost::asio::write(socket_, boost::asio::buffer(response), boost::asio::transfer_all(), err);
      if (err)
      {
        server_->DestroyHTTPConnection(boost::static_pointer_cast<HTTPConnection>(shared_from_this()));
        return;
      }

      boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPIgnore, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else if (method == "POST")
    {
      std::vector< std::pair<std::string, std::string> >::const_iterator splitheader = std::find_if(splitheaders.cbegin(), splitheaders.cend(), [](const std::pair<std::string, std::string>& splitheader) { return (splitheader.first == XSESSIONCOOKIE); });
      if (splitheader == splitheaders.cend())
      {
        Write(BadHTTPRequest());
        boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        return;
      }

      boost::shared_ptr<HTTPConnection> getconnection = server_->TakeHTTPConnection(splitheader->second);
      if (getconnection == nullptr)
      {
        Write(BadHTTPRequest());
        boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        return;
      }

      {
        std::lock_guard<std::mutex> lock(mutex_);
        getconnection_ = getconnection;
      }
      boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(1), boost::bind(&HTTPConnection::HandleHTTPPost, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      Write(BadHTTPRequest());
      boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPRead, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
      return;
    }
  }
}

void HTTPConnection::HandleHTTPIgnore(const boost::system::error_code& error, std::size_t size)
{
  if (error)
  {
    
    return;
  }
  boost::asio::async_read(socket_, buffer_, boost::bind(&HTTPConnection::HandleHTTPIgnore, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void HTTPConnection::HandleHTTPPost(const boost::system::error_code& error, std::size_t size)
{
  if (error)
  {
    Destroy();
    return;
  }

  rtspbuffer_ += std::string(boost::asio::buffer_cast<const char*>(buffer_.data() + (buffer_.in_avail() - size)), size);
  const std::vector<unsigned char> rtspdata = utility::Base64Decode(rtspbuffer_);
  if (rtspdata.empty())
  {
    // If we are unable to decode this, it's probably because we don't have all the data yet
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(1), boost::bind(&HTTPConnection::HandleHTTPPost, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return;
  }

  const std::string rtsp = std::string(reinterpret_cast<const char*>(rtspdata.data()), rtspdata.size());
  const size_t newlines = rtsp.find("\r\n\r\n");
  if (newlines == std::string::npos)
  {
    // Looks incomplete
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(1), boost::bind(&HTTPConnection::HandleHTTPPost, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return;
  }

  rtspbuffer_.clear();//TODO but what if we received half of another request... there isn't a realiable and/or sensible way to do this, so just clear it all up???

  //TODO we need to parse the content possibly?

  ParseRequest(RtspRequest(rtsp));
  boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(1), boost::bind(&HTTPConnection::HandleHTTPPost, boost::static_pointer_cast<HTTPConnection>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 1>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 2>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 3>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 4>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 5>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 6>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 7>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const std::array<boost::asio::const_buffer, 8>& buffers)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(buffers);
  }
  else
  {

    return Connection::Write(buffers);
  }
}

boost::system::error_code HTTPConnection::Write(const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(data, size);
  }
  else
  {

    return Connection::Write(data, size);
  }
}

boost::system::error_code HTTPConnection::Write(const std::string& data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (getconnection_)
  {
    std::lock_guard<std::mutex> lock(writemutex_);
    return getconnection_->Write(data);
  }
  else
  {

    return Connection::Write(data);
  }
}

}

}
