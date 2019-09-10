// deleteprofileresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

DeleteProfileResponse::DeleteProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

DeleteProfileResponse::~DeleteProfileResponse()
{

}

}

}
