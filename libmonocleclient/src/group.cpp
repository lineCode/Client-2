// group.cpp
//

///// Includes /////

#include "monocleclient/group.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Group::Group(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) :
  token_(token),
  name_(name),
  manageusers_(manageusers),
  managerecordings_(managerecordings),
  managemaps_(managemaps),
  managedevice_(managedevice),
  allrecordings_(allrecordings),
  recordings_(recordings)
{
  
}

Group::~Group()
{

}

}
