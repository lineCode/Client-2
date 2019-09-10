// service.hpp
//

#ifndef ID9VPAH8NR6NH0EC1I8AAG432G8N6602U3
#define ID9VPAH8NR6NH0EC1I8AAG432G8N6602U3

///// Includes /////

#include <boost/thread/future.hpp>
#include <event2/http.h>
#include <string>

#include "serverresponse.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Prototypes /////

std::string ToString(evhttp_cmd_type requesttype);

///// Classes /////

class Service
{
 public:

  Service();
  virtual ~Service();

  virtual void Update();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const = 0;
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const = 0;

  virtual boost::shared_future<ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) = 0;

 protected:

  std::string DigestResponse(const std::string& ha1, evhttp_cmd_type requesttype, const std::string& uri, const std::string& nonce, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop) const;


};

}

}

#endif
