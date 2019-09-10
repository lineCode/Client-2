// clientrequest.cpp
//

///// Includes /////

#include "rtsp/client/clientrequest.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

ClientRequest::ClientRequest(ClientRequest&& rhs) noexcept :
  request_(rhs.request_),
  id_(rhs.id_),
  latency_(rhs.latency_),
  deadlinetimer_(std::move(rhs.deadlinetimer_))
{

}

ClientRequest::ClientRequest(const RtspRequest& request, uint64_t id, const std::chrono::high_resolution_clock::time_point& latency, boost::shared_ptr<boost::asio::deadline_timer>&& deadlinetimer) :
  request_(request),
  id_(id),
  latency_(latency),
  deadlinetimer_(std::move(deadlinetimer))
{

}

ClientRequest& ClientRequest::operator=(ClientRequest&& rhs) noexcept
{
  request_ = rhs.request_;
  id_ = rhs.id_;
  latency_ = rhs.latency_;
  deadlinetimer_ = std::move(rhs.deadlinetimer_);
  return *this;
}

}
