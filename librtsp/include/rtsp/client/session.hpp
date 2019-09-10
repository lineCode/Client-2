// session.hpp
//

#ifndef IDKV95ZNWP88ZY49UUUW92UJXNWQU07212
#define IDKV95ZNWP88ZY49UUUW92UJXNWQU07212

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

#include "interleavedrtpclient.hpp"
#include "rtpclient.hpp"
#include "udpmulticastrtpclient.hpp"
#include "udpunicastrtpclient.hpp"

///// Namespaces /////

namespace rtsp
{

namespace client
{

///// Classes /////

template<class T>
class Session : public boost::enable_shared_from_this< Session<T> >
{
 public:

  Session(uint64_t id, boost::asio::io_service& io) :
    id_(id),
    keepalive_(boost::make_shared<boost::asio::deadline_timer>(io))
  {

  }

  Session(const Session&) = delete;

  Session(Session&& rhs) noexcept :
    id_(rhs.id_),
    keepalive_(std::move(rhs.keepalive_)),
    session_(std::move(rhs.session_)),
    udpunicastrtpclients_(std::move(rhs.udpunicastrtpclients_)),
    udpmulticastcastrtpclient_(std::move(rhs.udpmulticastcastrtpclient_))
  {

  }

  ~Session()
  {
    Destroy();

  }

  void Destroy()
  {
    session_.clear();
    keepalive_.reset();
    interleavedchannels_.clear();

    for (auto& udpunicastrtpclient : udpunicastrtpclients_)
    {
      udpunicastrtpclient->Destroy();

    }
    udpunicastrtpclients_.clear();

    if (udpmulticastcastrtpclient_)
    {
      udpmulticastcastrtpclient_->Destroy();
      udpmulticastcastrtpclient_.reset();
    }
  }

  inline uint64_t GetID() const { return id_; }

  inline void SetSession(const std::string& session) { session_ = session; }
  inline const std::string& GetSession() const { return session_; }

  const uint64_t id_;

  std::string session_;

  boost::shared_ptr<boost::asio::deadline_timer> keepalive_;

  std::vector< std::pair<uint8_t, uint8_t> > interleavedchannels_; // Rtp and Rtcp channels
  std::vector< boost::shared_ptr< UDPUnicastRtpClient<T> > > udpunicastrtpclients_;
  boost::shared_ptr< UDPMulticastRtpClient<T> > udpmulticastcastrtpclient_;

};

}

}

#endif
