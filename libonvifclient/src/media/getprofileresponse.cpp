// getprofileresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetProfileResponse::GetProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message), token_(token)
{

}

GetProfileResponse::GetProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<Profile>& profile) :
  Response(client, localendpoint, latency, message),
  token_(token),
  profile_(profile)
{

}

GetProfileResponse::~GetProfileResponse()
{

}

}

}
