// clientrequest.hpp
//

#ifndef IDBY8AKXICG9SXRVDHEAN64F222RP55SAY
#define IDBY8AKXICG9SXRVDHEAN64F222RP55SAY

///// Includes /////

#include <boost/asio.hpp>

#include <chrono>
#include <memory>
#include <string>

#include "rtsp/rtsprequest.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

struct ClientRequest // This structure contains a few bits specific to the client
{
  ClientRequest(const ClientRequest&) = delete;
  ClientRequest(ClientRequest&& rhs) noexcept;
  ClientRequest(const RtspRequest& request, uint64_t id, const std::chrono::high_resolution_clock::time_point& latency, boost::shared_ptr<boost::asio::deadline_timer>&& deadlinetimer);
  ClientRequest& operator=(ClientRequest&& rhs) noexcept;

  RtspRequest request_;
  uint64_t id_;
  std::chrono::high_resolution_clock::time_point latency_;
  boost::shared_ptr<boost::asio::deadline_timer> deadlinetimer_;

};

}

#endif
