// user.hpp
//

///// Includes /////

#include "license/user.hpp"

///// Namespaces /////

namespace license
{

///// Methods /////

User::User()
{

}

User::User(const int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& password, const int verified) :
  id_(id),
  firstname_(firstname),
  lastname_(lastname),
  email_(email),
  password_(password),
  verified_(verified)
{
  
}

User::~User()
{

}

}
