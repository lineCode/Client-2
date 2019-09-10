// createprofileresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

CreateProfileResponse::CreateProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name, const boost::optional<std::string>& token) :
  Response(client, localendpoint, latency, message),
  name_(name),
  token_(token)
{

}

CreateProfileResponse::CreateProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name, const boost::optional<std::string>& token, const boost::optional<Profile>& profile) :
  Response(client, localendpoint, latency, message),
  name_(name),
  token_(token),
  profile_(profile)
{

}

CreateProfileResponse::~CreateProfileResponse()
{

}

}

}
