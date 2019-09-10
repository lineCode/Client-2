// getprofilesresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetProfilesResponse::GetProfilesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetProfilesResponse::GetProfilesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Profile>& profiles) :
  Response(client, localendpoint, latency, message),
  profiles_(profiles)
{

}

GetProfilesResponse::~GetProfilesResponse()
{

}

}

}
