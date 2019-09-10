// timing.hpp
//

#ifndef ID73Q8TNP6YRAJVHHVKZ1ZXW5C5LI6LDBK
#define ID73Q8TNP6YRAJVHHVKZ1ZXW5C5LI6LDBK

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Timing
{
 public:

  Timing();
  Timing(const std::vector< std::pair<uint64_t, uint64_t> >& timings);
  Timing(const std::vector<std::string>& timings);
  ~Timing();

  void AddTiming(const std::string& timing);

  bool IsPermanent() const;
  bool IsUnbounded() const;

  void Clear();
  std::vector<std::string> ToString() const;

  bool operator==(const Timing& rhs) const;

  std::vector< std::pair<uint64_t, uint64_t> > timings_;

};

}

}

#endif
