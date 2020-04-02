// trackstatisticsstream.cpp
//

///// Includes /////

#include "monocleclient/trackstatisticsstream.h"

///// Namespaces /////

namespace client
{

///// Methods /////

TrackStatisticsStream::TrackStatisticsStream(const uint64_t token, CONTROLTRACKSTATISTICSSTREAMEND controltrackstatisticsstreamendcallback, CONTROLTRACKSTATISTICSSTREAMRESULT controltrackstatisticsstreamresultcallback, void* callbackdata) :
  token_(token),
  controltrackstatisticsstreamendcallback_(controltrackstatisticsstreamendcallback),
  controltrackstatisticsstreamresultcallback_(controltrackstatisticsstreamresultcallback),
  callbackdata_(callbackdata)
{
  
}

TrackStatisticsStream::TrackStatisticsStream(const TrackStatisticsStream& rhs) :
  token_(rhs.token_),
  controltrackstatisticsstreamendcallback_(rhs.controltrackstatisticsstreamendcallback_),
  controltrackstatisticsstreamresultcallback_(rhs.controltrackstatisticsstreamresultcallback_),
  callbackdata_(rhs.callbackdata_)
{

}

TrackStatisticsStream::~TrackStatisticsStream()
{

}

void TrackStatisticsStream::ControlStreamEnd(const uint64_t requestindex, const monocle::ErrorCode error)
{
  if (controltrackstatisticsstreamendcallback_)
  {
    controltrackstatisticsstreamendcallback_(token_, requestindex, error, callbackdata_);

  }
}

void TrackStatisticsStream::ControlStreamResult(const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results)
{
  if (controltrackstatisticsstreamresultcallback_)
  {
    controltrackstatisticsstreamresultcallback_(token_, requestindex, starttime, endtime, results, callbackdata_);

  }
}

TrackStatisticsStream& TrackStatisticsStream::operator=(const TrackStatisticsStream& rhs)
{
  token_ = rhs.token_;
  controltrackstatisticsstreamendcallback_ = rhs.controltrackstatisticsstreamendcallback_;
  controltrackstatisticsstreamresultcallback_ = rhs.controltrackstatisticsstreamresultcallback_;
  callbackdata_ = rhs.callbackdata_;
  return *this;
}

}
