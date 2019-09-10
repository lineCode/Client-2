// pipeclient.cpp
//

///// Includes /////

#include "monocleclient/pipeclient.h"

///// Namespaces /////

namespace client
{

///// Methods /////

PipeClient::PipeClient()
{
  
}

PipeClient::~PipeClient()
{

}

void PipeClient::Message(const std::string& message)
{
  emit Message(QString::fromStdString(message));

}

}
