// testproxyparams.cpp
//

///// Includes /////

#include <gtest/gtest.h>

#include <socket/proxyparams.hpp>

///// Namespaces /////

namespace sock
{

namespace tests
{

///// Classes /////

TEST(TestProxyParams, IsValid)
{
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_NONE, "hostname", 80, false, "username", "password").IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_NONE, std::string(), 80, true, std::string(), std::string()).IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_NONE, std::string(), 80, false, "username", std::string()).IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_NONE, std::string(), 80, false, std::string(), "password").IsValid());

  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, false, "username", "password").IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, true, std::string(), std::string()).IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, false, "username", std::string()).IsValid());
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, true, std::string(), "password").IsValid());

  ASSERT_FALSE(ProxyParams(sock::PROXYTYPE_SOCKS5, std::string(), 80, false, "username", "password").IsValid());
  ASSERT_FALSE(ProxyParams(sock::PROXYTYPE_SOCKS5, std::string(), 80, true, std::string(), std::string()).IsValid());
  ASSERT_FALSE(ProxyParams(sock::PROXYTYPE_SOCKS5, std::string(), 80, false, "username", std::string()).IsValid());
  ASSERT_FALSE(ProxyParams(sock::PROXYTYPE_SOCKS5, std::string(), 80, true, std::string(), "password").IsValid());
}

TEST(TestProxyParams, ToString)
{
  ASSERT_TRUE(ProxyParams(sock::PROXYTYPE_NONE, "hostname", 80, false, "username", "password").ToString().empty());
  ASSERT_STREQ("socks5://username:password@hostname:80", ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, false, "username", "password").ToString().c_str());
  ASSERT_STREQ("socks5h://username:password@hostname:80", ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, true, "username", "password").ToString().c_str());
  ASSERT_STREQ("socks5h://hostname:80", ProxyParams(sock::PROXYTYPE_SOCKS5, "hostname", 80, true, std::string(), "password").ToString().c_str());
}

}

}
