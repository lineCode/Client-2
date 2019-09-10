// main.cpp
//

///// Includes /////

#include "main.h"

#include <boost/predef.h>
#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <onvifclient/onvifclient.hpp>
#include <memory>
#include <socket/socket.hpp>
#include <stdint.h>
#include <thread>
#include <vector>

///// Functions /////

int main(int argc, char** argv)
{
  // Get the input options
  std::string address;
  std::string username;
  std::string password;

  std::string proxytype;
  std::string proxyhostname;
  uint16_t proxyport = 0;
  bool proxyremotedns = false;
  std::string proxyusername;
  std::string proxypassword;

  bool ipv4dhcpenabled = false;

  boost::program_options::variables_map vm;
  boost::program_options::options_description description("Allowed options");
  description.add_options()
  ("help", "Help message")
  ("address", boost::program_options::value<std::string>(&address)->required(), "Hostname of camera")
  ("proxytype", boost::program_options::value<std::string>(&proxytype), "Proxy type to use(default is \"none\", \"socks5\")")
  ("proxyhostname", boost::program_options::value<std::string>(&proxyhostname), "Hostname of proxy")
  ("proxyport", boost::program_options::value<uint16_t>(&proxyport), "Port of proxy")
  ("proxyremotedns", boost::program_options::value<bool>(&proxyremotedns), "Whether remote dns is required by the proxy")
  ("proxyusername", boost::program_options::value<std::string>(&proxyusername), "Proxy username")
  ("proxypassword", boost::program_options::value<std::string>(&proxypassword), "Proxy password")
  ("username", boost::program_options::value<std::string>(&username), "Username to use")
  ("password", boost::program_options::value<std::string>(&password), "Password to use")
  ("GetNetworkInterfaces", "GetNetworkInterfaces")
  ("GetSystemDateAndTime", "GetSystemDateAndTime")
  ("GetVideoSources", "GetVideoSources")
  ("GetVideoSourceConfiguration", boost::program_options::value<std::string>(), "GetVideoSourceConfiguration")
  ("SetNetworkInterfaces", boost::program_options::value<std::string>(), "SetNetworkInterfaces")
  ("ipv4dhcpenabled", boost::program_options::value<bool>(&ipv4dhcpenabled), "ipv4dhcpenabled")
  ("SystemReboot", "SystemReboot");

  try
  {
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
    if (vm.count("help"))
    {
      std::cout << description << std::endl;
      return 0;
    }

    boost::program_options::notify(vm);
  }
  catch(boost::program_options::error& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }

  const sock::ProxyParams proxyparams = sock::ProxyParams(sock::FromString(proxytype), proxyhostname, proxyport, proxyremotedns, proxyusername, proxypassword);
  if (!proxyparams.IsValid())
  {
    std::cout << "Invalid proxy parameters" << std::endl;
    return 1;
  }

  // Setup the device
  if (onvif::Init())
  {
    std::cout << "onvif::Init() failed" << std::endl;
    return 1;
  }

  onvif::ClientThread<onvif::device::DeviceClient> device;
  onvif::ClientThread<onvif::media::MediaClient> media;
  if (device.Init(proxyparams, address, username, password))
  {
    std::cout << "onvif::ClientThread<onvif::device::DeviceClient>::Init() failed" << std::endl;
    return 1;
  }

  if (media.Init(proxyparams, address, username, password))
  {
    std::cout << "onvif::ClientThread<onvif::device::MediaClient>::Init() failed" << std::endl;
    return 1;
  }

  // Get the time and set the offset for authentication
  const onvif::device::GetSystemDateAndTimeResponse getsystemdateandtimeresponse = device->GetSystemDateAndTimeFuture().get();
  if (getsystemdateandtimeresponse.Error() || !getsystemdateandtimeresponse.systemdatetime_.is_initialized())
  {
    std::cout << getsystemdateandtimeresponse.Message() << std::endl;
    return 1;
  }

  const boost::optional<boost::posix_time::ptime> datetime = getsystemdateandtimeresponse.systemdatetime_->GetUtcDateTime();
  if (!datetime.is_initialized())
  {
    std::cout << std::string("GetSystemDateAndTime::UtcDateTime not initialised") << std::endl;
    return 1;
  }

  const auto offset = getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset();
  device->SetTimeOffset(offset);
  media->SetTimeOffset(offset);

  // Send the command
  if (vm.count("GetNetworkInterfaces"))
  {
    const onvif::device::GetNetworkInterfacesResponse getnetworkinterfacesresponse = device->GetNetworkInterfacesFuture().get();
    if (getnetworkinterfacesresponse.Error())
    {
      std::cout << getnetworkinterfacesresponse.Message() << std::endl;
      return 1;
    }

    for (const auto& networkinterface : getnetworkinterfacesresponse.networkinterfaces_)
    {
      if (networkinterface.token_.is_initialized())
      {
        std::cout << *networkinterface.token_ << std::endl;
      
      }
    }
  }
  else if (vm.count("GetSystemDateAndTime"))
  {
    std::cout << datetime->date().day() << std::string("/") << datetime->date().month().as_short_string() << std::string("/") << datetime->date().year() << std::string(" ") << datetime->time_of_day().hours() << std::string(":") << datetime->time_of_day().minutes() << std::string(":") << datetime->time_of_day().seconds() << std::endl;

  }
  else if (vm.count("GetVideoSources"))
  {
    const onvif::media::GetVideoSourcesResponse getvideosourcesresponse = media->GetVideoSourcesFuture().get();
    if (getvideosourcesresponse.Error())
    {
      std::cout << getvideosourcesresponse.Message() << std::endl;
      return 1;
    }

    for (const auto& videosource : getvideosourcesresponse.videosources_)
    {
      if (videosource.token_.is_initialized())
      {
        std::cout << *videosource.token_ << std::endl;;
        
      }
    }
  }
  else if (vm.count("GetVideoSourceConfiguration"))
  {
    const std::string token = vm["GetVideoSourceConfiguration"].as<std::string>();
    if (token.empty())
    {
      std::cout << description << std::endl;
      return 1;
    }
    
    const onvif::media::GetVideoSourceConfigurationResponse getvideosourceconfigurationresponse = media->GetVideoSourceConfigurationFuture(token).get();
    if (getvideosourceconfigurationresponse.Error())
    {
      std::cout << getvideosourceconfigurationresponse.Message() << std::endl;
      return 1;
    }

    if (getvideosourceconfigurationresponse.videosourceconfiguration_.is_initialized())
    {
      if (getvideosourceconfigurationresponse.videosourceconfiguration_->token_.is_initialized())
      {
        std::cout << "Token: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->token_ << std::endl;

      }

      if (getvideosourceconfigurationresponse.videosourceconfiguration_->name_.is_initialized())
      {
        std::cout << "Name: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->name_ << std::endl;

      }

      if (getvideosourceconfigurationresponse.videosourceconfiguration_->usecount_.is_initialized())
      {
        std::cout << "UseCount: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->usecount_ << std::endl;

      }

      if (getvideosourceconfigurationresponse.videosourceconfiguration_->sourcetoken_.is_initialized())
      {
        std::cout << "SourceToken: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->sourcetoken_ << std::endl;

      }

      if (getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_.is_initialized())
      {
        std::cout << "Bounds" << std::endl;
        if (getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->x_.is_initialized())
        {
          std::cout << "  x: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->x_ << std::endl;

        }

        if (getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->y_.is_initialized())
        {
          std::cout << "  y: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->y_ << std::endl;

        }

        if (getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->width_.is_initialized())
        {
          std::cout << "  width: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->width_ << std::endl;

        }

        if (getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->height_.is_initialized())
        {
          std::cout << "  height: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->bounds_->height_ << std::endl;

        }
      }

      if (getvideosourceconfigurationresponse.videosourceconfiguration_->rotate_.is_initialized())
      {
        std::cout << "Rotate:" << std::endl;
        if (getvideosourceconfigurationresponse.videosourceconfiguration_->rotate_->rotatemode_.is_initialized())
        {
          std::cout << "  Mode: " << onvif::ToString(*getvideosourceconfigurationresponse.videosourceconfiguration_->rotate_->rotatemode_) << std::endl;

        }

        if (getvideosourceconfigurationresponse.videosourceconfiguration_->rotate_->degree_.is_initialized())
        {
          std::cout << "  Degree: " << *getvideosourceconfigurationresponse.videosourceconfiguration_->rotate_->degree_ << std::endl;

        }
      }
    }
  }
  else if (vm.count("SetNetworkInterfaces"))
  {
    const std::string token = vm["SetNetworkInterfaces"].as<std::string>();
    if (token.empty())
    {
      std::cout << description << std::endl;
      return 1;
    }
    
    boost::optional<bool> ipv4dhcpenabled;
    if (!vm["ipv4dhcpenabled"].empty())
    {
      ipv4dhcpenabled = vm["ipv4dhcpenabled"].as<bool>();

    }
    
    const onvif::NetworkInterfaceSetConfiguration networkinterfacesetconfiguration(boost::none, boost::none, boost::none, onvif::IPv4NetworkInterfaceSetConfiguration(boost::none, { }, ipv4dhcpenabled), boost::none, { });
    const onvif::device::SetNetworkInterfacesResponse setnetworkinterfacesresponse = device->SetNetworkInterfacesFuture(token, networkinterfacesetconfiguration).get();
    if (setnetworkinterfacesresponse.Error())
    {
      std::cout << setnetworkinterfacesresponse.Message() << std::endl;
      return 1;
    }
  }
  else if (vm.count("SystemReboot"))
  {
    const onvif::device::SystemRebootResponse systemreboot = device->SystemRebootFuture().get();
    if (systemreboot.Error())
    {

      return 1;
    }

    std::cout << systemreboot.message_ << std::endl;
  }
  else
  {
    std::cout << "Invalid command" << std::endl;
    return 1;
  }
  
  media.Destroy();
  device.Destroy();
  onvif::Destroy();

  return 0;
}
