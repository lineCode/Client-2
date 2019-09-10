// main.cpp
//

///// Includes /////

#include "main.hpp"

#include <boost/program_options.hpp>
#include <stdint.h>

///// Globals /////

sock::ProxyParams g_proxyparams;
std::string g_address;
std::string g_username;
std::string g_password;

///// Prototypes /////

void Help();

///// Functions /////

GTEST_API_ int main(int argc, char** argv)
{
  // Sort out program options
  testing::InitGoogleTest(&argc, argv);
  
  boost::program_options::variables_map vm;
  boost::program_options::options_description description("Allowed options");
  std::string proxytype;
  std::string proxyhostname;
  uint16_t proxyport = 0;
  bool proxyremotedns = false;
  std::string proxyusername;
  std::string proxypassword;
  description.add_options()
  ("help,h", "Help message")
  ("address,a", boost::program_options::value<std::string>(&g_address)->required(), "Hostname of camera")
  ("proxytype", boost::program_options::value<std::string>(&proxytype), "Proxy type to use(none, socks5)")
  ("proxyhostname", boost::program_options::value<std::string>(&proxyhostname), "Hostname of proxy")
  ("proxyport", boost::program_options::value<uint16_t>(&proxyport), "Port of proxy")
  ("proxyremotedns", boost::program_options::value<bool>(&proxyremotedns), "Whether remote dns is required by the proxy")
  ("proxyusername", boost::program_options::value<std::string>(&proxyusername), "Proxy username")
  ("proxypassword", boost::program_options::value<std::string>(&proxypassword), "Proxy password")
  ("username,u", boost::program_options::value<std::string>(&g_username), "Username to use")
  ("password,p", boost::program_options::value<std::string>(&g_password), "Password to use");

  try
  {
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);

    if (vm.count("help"))
    {
      Help();
      return 0;
    }

    boost::program_options::notify(vm);
  }
  catch(boost::program_options::error& e)
  {
    std::cout << e.what() << std::endl;
    Help();
    return 1;
  }

  g_proxyparams = sock::ProxyParams(sock::FromString(proxytype), proxyhostname, proxyport, proxyremotedns, proxyusername, proxypassword);
  if (!g_proxyparams.IsValid())
  {
    std::cout << "Invalid proxy parameters" << std::endl;
    return 1;
  }

  return RUN_ALL_TESTS();
}

void Help()
{
  std::cout << "Example call:" << std::endl;
  std::cout << "./integration --address http://localhost/onvif/deviceclient_service/ --username chris --password pass" << std::endl;
}
