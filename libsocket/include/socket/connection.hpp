// connection.hpp
//

#ifndef IDIPCOMXAGJWX2HDGF2XN82SKYO5VNGU0F
#define IDIPCOMXAGJWX2HDGF2XN82SKYO5VNGU0F

///// Includes /////

#include "connectionblock.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

///// Namespaces /////

namespace sock
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

#endif
