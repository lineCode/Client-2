// streamingprotocol.cpp
//

///// Includes /////

#include "monocleclient/streamingprotocol.h"

///// Namespaces /////

namespace client
{

///// Functions /////

QString ToString(const STREAMINGPROTOCOL streamingprotocol)
{
  switch (streamingprotocol)
  {
    case STREAMINGPROTOCOL_UDPUNICAST:
    {

      return QString("UDPUNICAST");
    }
    case STREAMINGPROTOCOL_TCPINTERLEAVED:
    {

      return QString("TCPINTERLEAVED");
    }
    case STREAMINGPROTOCOL_UDPMULTICAST:
    {

      return QString("UDPMULTICAST");
    }
    default:
    {

      return QString("UDPUNICAST");
    }
  }
}

STREAMINGPROTOCOL FromStringStreamingProtocol(const QString& streamingprotocol)
{
  if (streamingprotocol == "UDPUNICAST")
  {

    return STREAMINGPROTOCOL_UDPUNICAST;
  }
  else if (streamingprotocol == "TCPINTERLEAVED")
  {

    return STREAMINGPROTOCOL_TCPINTERLEAVED;
  }
  else if (streamingprotocol == "UDPMULTICAST")
  {

    return STREAMINGPROTOCOL_UDPMULTICAST;
  }
  else
  {

    return STREAMINGPROTOCOL_UDPUNICAST;
  }
}


}
