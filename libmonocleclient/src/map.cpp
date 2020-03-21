// map.cpp
//

///// Includes /////

#include "monocleclient/map.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Map::Map(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& name, const QString& location, const QString& imagemd5, const uint64_t guiorder) :
  device_(device),
  token_(token),
  name_(name),
  location_(location),
  imagemd5_(imagemd5),
  guiorder_(guiorder)
{

}

Map::~Map()
{

}

}
