// proxyparams.hpp
//

#ifndef IDJ9Y5P14GFH8FN1DR7KY9398V6MOFKMO3
#define IDJ9Y5P14GFH8FN1DR7KY9398V6MOFKMO3

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

#include "proxytype.hpp"

///// Namespaces /////

namespace sock
{

///// Classes /////

class ProxyParams
{
 public:
  
  ProxyParams();
  ProxyParams(sock::PROXYTYPE type, const std::string& hostname, uint16_t port, bool remotedns, const std::string& username, const std::string& password);
  virtual ~ProxyParams();

  void Clear();

  bool IsValid() const;

  std::string ToString();

  inline void SetType(sock::PROXYTYPE type) { type_ = type; }
  inline sock::PROXYTYPE GetType() const { return type_; }

  inline void SetHostname(const std::string& hostname) { hostname_ = hostname; }
  inline const std::string& GetHostname() const { return hostname_; }

  inline void SetPort(uint16_t port) { port_ = port; }
  inline uint16_t GetPort() const { return port_; }

  inline void SetRemoteDns(bool remotedns) { remotedns_ = remotedns; }
  inline bool GetRemoteDns() const { return remotedns_; }

  inline void SetUsername(const std::string& username) { username_ = username; }
  inline const std::string& GetUsername() const { return username_; }

  inline void SetPassword(const std::string& password) { password_ = password; }
  inline const std::string& GetPassword() const { return password_; }

  bool operator==(const ProxyParams& rhs) const;

 private:
  
  sock::PROXYTYPE type_;
  std::string hostname_;
  uint16_t port_;
  bool remotedns_;
  std::string username_;
  std::string password_;

};

}

#endif
