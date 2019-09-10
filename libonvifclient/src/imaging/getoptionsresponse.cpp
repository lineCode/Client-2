// getoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/imagingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Methods /////

GetOptionsResponse::GetOptionsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken) :
  Response(client, localendpoint, latency, message),
  videosourcetoken_(videosourcetoken)
{

}

GetOptionsResponse::GetOptionsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const boost::optional<ImagingOptions20>& imagingoptions) :
  Response(client, localendpoint, latency, message),
  videosourcetoken_(videosourcetoken),
  imagingoptions_(imagingoptions)
{

}

GetOptionsResponse::~GetOptionsResponse()
{

}

}

}
