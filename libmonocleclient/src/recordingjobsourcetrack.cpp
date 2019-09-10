// recordingjobsourcetrack.cpp
//

///// Includes /////

#include "monocleclient/recordingjobsourcetrack.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "monocleclient/view.h"

///// Declarations /////

namespace monocle
{

enum class RecordingJobState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Globals /////

const QString PROFILE_TOKEN_PARAMETER_NAME = "ProfileToken";
const QString SOURCE_TAG_PARAMETER_NAME = "SourceTag";
const QString ROTATION_PARAMETER_NAME = "Rotation";

///// Methods /////

RecordingJobSourceTrack::RecordingJobSourceTrack(const uint64_t token, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters, const QSharedPointer<client::RecordingTrack>& track) :
  token_(token),
  parameters_(parameters),
  state_(state),
  error_(error),
  activeparameters_(activeparameters),
  track_(track)
{

}

RecordingJobSourceTrack::~RecordingJobSourceTrack()
{

}

void RecordingJobSourceTrack::SetState(const monocle::RecordingJobState state, const QString& error)
{
  const monocle::RecordingJobState prevstate = state_;
  state_ = state;
  error_ = error;
  emit StateChanged(state, error, prevstate);
}

boost::optional<QString> RecordingJobSourceTrack::GetSourceTag() const
{
  return GetParameter(SOURCE_TAG_PARAMETER_NAME);
}

boost::optional<QString> RecordingJobSourceTrack::GetProfileToken() const
{
  return GetParameter(PROFILE_TOKEN_PARAMETER_NAME);
}

boost::optional<ROTATION> RecordingJobSourceTrack::GetRotation() const
{
  const boost::optional<QString> rotation = GetParameter(ROTATION_PARAMETER_NAME);
  if (!rotation.is_initialized())
  {

    return boost::none;
  }

  if (*rotation == "0")
  {

    return ROTATION::_0;
  }
  else if (*rotation == "90")
  {

    return ROTATION::_90;
  }
  else if (*rotation == "180")
  {

    return ROTATION::_180;
  }
  else if (*rotation == "270")
  {

    return ROTATION::_270;
  }
  else
  {

    return boost::none;
  }
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveProfileToken() const
{
  return GetActiveParameter(PROFILE_TOKEN_PARAMETER_NAME);
}

boost::optional<QString> RecordingJobSourceTrack::GetParameter(const QString& name) const
{
  const std::string tmp = name.toStdString() + "=";
  std::vector<QString>::const_iterator parameter = std::find_if(parameters_.cbegin(), parameters_.cend(), [&tmp](const QString& parameter) { return boost::istarts_with(parameter.toStdString(), tmp); });
  if (parameter == parameters_.cend())
  {

    return boost::none;
  }
  return parameter->mid(static_cast<int>(tmp.size()), -1);
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveParameter(const QString& name) const
{
  const std::string tmp = name.toStdString() + "=";
  std::vector<QString>::const_iterator activeparameter = std::find_if(activeparameters_.cbegin(), activeparameters_.cend(), [&tmp](const QString& activeparameter) { return boost::istarts_with(activeparameter.toStdString(), tmp); });
  if (activeparameter == activeparameters_.cend())
  {

    return boost::none;
  }
  return activeparameter->mid(static_cast<int>(tmp.size()), -1);
}

}
