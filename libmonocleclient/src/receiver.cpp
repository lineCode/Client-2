// receiver.cpp
//

///// Includes /////

#include "monocleclient/receiver.h"

#include <boost/algorithm/string.hpp>
#include <monocleprotocol/streamingprotocol_generated.h>
#include <QUrl>

///// Namespaces /////

namespace client
{

///// Globals /////

const QString STREAMING_PROTOCOL_PARAMETER_NAME = "StreamingProtocol";

///// Methods /////

Receiver::Receiver(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters, const monocle::ReceiverState state) :
  token_(token),
  mode_(mode),
  mediauri_(mediauri),
  autocreated_(autocreated),
  username_(username),
  password_(password),
  parameters_(parameters),
  state_(state)
{

}

Receiver::~Receiver()
{

}

void Receiver::SetConfiguration(const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters)
{
  mode_ = mode;
  mediauri_ = mediauri;
  autocreated_ = autocreated;
  username_ = username;
  password_ = password;
  parameters_ = parameters;
}

boost::optional<monocle::StreamingProtocol> Receiver::GetStreamingProtocol() const
{
  const boost::optional<QString> parameter = GetParameter(STREAMING_PROTOCOL_PARAMETER_NAME);
  if (!parameter.is_initialized())
  {

    return boost::none;
  }
  
  for (int i = static_cast<int>(monocle::StreamingProtocol::MIN); i <= static_cast<int>(monocle::StreamingProtocol::MAX); ++i)
  {
    if ((*parameter) == monocle::EnumNamesStreamingProtocol()[i])
    {

      return static_cast<monocle::StreamingProtocol>(i);
    }
  }
  return boost::none;
}

bool Receiver::IsONVIF() const
{
  const QUrl url(mediauri_);
  if ((url.scheme() == "http") && (url.path() == "/onvif/device_service"))
  {

    return true;
  }
  return false;
}

boost::optional<QString> Receiver::GetParameter(const QString& name) const
{
  const std::string tmp = name.toStdString() + "=";
  std::vector<QString>::const_iterator parameter = std::find_if(parameters_.cbegin(), parameters_.cend(), [&tmp](const QString& parameter) { return boost::istarts_with(parameter.toStdString(), tmp); });
  if (parameter == parameters_.cend())
  {

    return boost::none;
  }
  return parameter->mid(static_cast<int>(tmp.size()), -1);
}

}
