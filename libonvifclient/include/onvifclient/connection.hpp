// connection.hpp
//

#ifndef IDH8UPDD7K46FTXJ45DSPTHX2NB18J2WX2
#define IDH8UPDD7K46FTXJ45DSPTHX2NB18J2WX2

///// Includes /////

#include "connectionblock.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

class Connection
{
 public:

  Connection();
  Connection(const Connection&) = delete;
  Connection(Connection&& connection) noexcept;
  Connection(boost::shared_ptr<ConnectionBlock>& connectionblock);
  ~Connection();

  void Close();

  bool IsConnected() const;

  Connection& operator=(Connection&& connection) noexcept;

 private:

  boost::weak_ptr<ConnectionBlock> connectionblock_;

};

}

#endif
