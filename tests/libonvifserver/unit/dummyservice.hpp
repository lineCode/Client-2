// dummyservice.hpp
//

#ifndef IDQX3LKJLHT4JJVXXNNY32HOPPMMPVB9OI
#define IDQX3LKJLHT4JJVXXNNY32HOPPMMPVB9OI

///// Includes /////

#include <onvifserver/service.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Classes /////

class DummyService : public Service
{
public:

  DummyService();
  virtual ~DummyService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override;
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& nc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const override;

  virtual boost::shared_future<ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  void SetRestricted(bool restricted) { restricted_ = restricted; }
  bool GetRestricted() const { return restricted_; }

  void SetResponse(const std::string& response) { response_ = response; }
  const std::string& GetResponse() const { return response_; }

  void SetUsername(const std::string& username) { username_ = username; }
  const std::string& GetUsername() const { return username_; }

  void SetPassword(const std::string& password) { password_ = password; }
  const std::string& GetPassword() const { return password_; }

private:

  bool restricted_;
  std::string response_;

  std::string username_;
  std::string password_;

};

}

}

}

#endif
