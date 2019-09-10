// playresponse.hpp
//

#ifndef IDJRXXL4FLGJCJJVQ78DU3XRDLAOTKFQC4
#define IDJRXXL4FLGJCJJVQ78DU3XRDLAOTKFQC4

///// Includes /////

#include <string>

#include "response.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class PlayResponse : public Response
{
 public:

  PlayResponse();
  PlayResponse(uint64_t latency);
  ~PlayResponse();


};

}

#endif
