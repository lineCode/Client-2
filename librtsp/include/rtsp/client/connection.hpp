// connection.hpp
//

#ifndef IDR2O07TBVGEBMLTU7VD8BLAMDFI6Y4C4L
#define IDR2O07TBVGEBMLTU7VD8BLAMDFI6Y4C4L

///// Includes /////

#include "connectionblock.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

///// Namespaces /////

namespace rtsp
{

namespace client
{

///// Classes /////

class Connection
{
 public:

  Connection();
  Connection(const Connection&) = delete;
  Connection(Connection&& connection);
  Connection(boost::shared_ptr<ConnectionBlock>& connectionblock);
  ~Connection();

  void Close();

  bool IsConnected() const;

  Connection& operator=(Connection&& connection);

 private:

  boost::weak_ptr<ConnectionBlock> connectionblock_;

};

}

}

#endif
