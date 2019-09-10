// ssrc.cpp
//

///// Includes /////

#include "rtsp/headers/ssrc.hpp"

#include <iomanip>
#include <sstream>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

SSrc::SSrc()
{

}

SSrc::SSrc(const std::string& text)
{
  if (!text.empty())
  {
    try
    {
      ssrc_ = std::stoul(text, nullptr, 16);

    }
    catch (...)
    {

    }
  }
}

SSrc::SSrc(const boost::optional<uint32_t>& ssrc) :
  ssrc_(ssrc)
{

}

SSrc::~SSrc()
{

}

void SSrc::Clear()
{
  ssrc_.reset();
}

std::string SSrc::ToString() const
{
  if (!ssrc_.is_initialized())
  {

    return std::string();
  }

  std::stringstream stream;
  stream << std::hex << std::setfill('0') << std::setw(sizeof(ssrc_) * 2) << *ssrc_;
  return stream.str();
}

bool SSrc::operator==(const SSrc& rhs) const
{
  return (ssrc_ == rhs.ssrc_);
}

}

}
