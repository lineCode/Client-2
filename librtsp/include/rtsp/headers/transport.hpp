// transport.hpp
//

#ifndef IDLJUBEOB22Y3UDQU7E5H2LSYX86RIABSK
#define IDLJUBEOB22Y3UDQU7E5H2LSYX86RIABSK

///// Includes /////

#include <boost/optional.hpp>
#include <string>
#include <vector>

#include "rtsp/headers/transportdata.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

class Transport
{
public:

  Transport();
  Transport(const std::string& text);
  Transport(const std::vector<TransportData>& transports);
  ~Transport();

  void Clear();
  bool IsValid() const;
  std::string ToString() const;

  bool operator==(const Transport& rhs) const;

  std::vector<TransportData> transports_;

};

}

}

#endif
