// user.cpp
//

///// Includes /////

#include "monocleclient/user.h"

///// Namespaces /////

namespace client
{

///// Methods /////

User::User(const uint64_t token, const QString& username, const uint64_t grouptoken) :
  token_(token),
  username_(username),
  grouptoken_(grouptoken)
{
  
}

User::~User()
{

}

}
