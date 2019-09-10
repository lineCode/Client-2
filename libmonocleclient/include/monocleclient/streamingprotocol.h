// streamingprotocol.h
//

#ifndef ID71HKKXG8RO24FK4JFGA0OPDYWR5LQ108
#define ID71HKKXG8RO24FK4JFGA0OPDYWR5LQ108

///// Includes /////

#include "singleton.h"

#include <QString>

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum STREAMINGPROTOCOL
{
  STREAMINGPROTOCOL_UDPUNICAST = 0,
  STREAMINGPROTOCOL_TCPINTERLEAVED = 1,
  STREAMINGPROTOCOL_UDPMULTICAST = 2
};

///// Prototypes /////

QString ToString(const STREAMINGPROTOCOL streamingprotocol);
STREAMINGPROTOCOL FromStringStreamingProtocol(const QString& streamingprotocol);

}

#endif
