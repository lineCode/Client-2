// onvifuser.cpp
//

///// Includes /////

#include "monocleclient/onvifuser.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ONVIFUser::ONVIFUser(const uint64_t token, const QString& username, const monocle::ONVIFUserlevel userlevel) :
  token_(token),
  username_(username),
  userlevel_(userlevel)
{
  
}

ONVIFUser::~ONVIFUser()
{

}

}
