// teardownresponse.hpp
//

#ifndef IDHHO3JNCOE5BEJQ1ZHQJYUMA28I72RV88
#define IDHHO3JNCOE5BEJQ1ZHQJYUMA28I72RV88

///// Includes /////

#include "response.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class TeardownResponse : public Response
{
 public:

  TeardownResponse();
  TeardownResponse(uint64_t latency);
  ~TeardownResponse();

};

}

#endif
