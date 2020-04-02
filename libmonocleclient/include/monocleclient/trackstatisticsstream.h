// trackstatisticsstream.h
//

#ifndef IDFMKGWSDFZXCZXCZXCZXCXZZZ366BC609
#define IDFMKGWSDFZXCZXCZXCZXCXZZZ366BC609

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

///// Declarations /////

namespace monocle
{

enum class ErrorCode : uint16_t;
enum class ObjectClass : uint16_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

typedef void(*CONTROLTRACKSTATISTICSSTREAMEND)(const uint64_t streamtoken, const uint64_t requestindex, const monocle::ErrorCode error, void* callbackdata);
typedef void(*CONTROLTRACKSTATISTICSSTREAMRESULT)(const uint64_t streamtoken, const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results, void* callbackdata);

///// Classes /////

class TrackStatisticsStream
{
 public:

  TrackStatisticsStream(const uint64_t token, CONTROLTRACKSTATISTICSSTREAMEND controltrackstatisticsstreamendcallback, CONTROLTRACKSTATISTICSSTREAMRESULT controltrackstatisticsstreamresultcallback, void* callbackdata);
  TrackStatisticsStream(const TrackStatisticsStream& rhs);
  ~TrackStatisticsStream();

  void ControlStreamEnd(const uint64_t requestindex, const monocle::ErrorCode error);
  void ControlStreamResult(const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results);

  inline uint64_t GetToken() const { return token_; }

  TrackStatisticsStream& operator=(const TrackStatisticsStream& rhs);

 private:

  uint64_t token_;

  CONTROLTRACKSTATISTICSSTREAMEND controltrackstatisticsstreamendcallback_;
  CONTROLTRACKSTATISTICSSTREAMRESULT controltrackstatisticsstreamresultcallback_;
  void* callbackdata_;

};

}

#endif
