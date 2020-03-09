// database.hpp
//

#ifndef IDKPYK79OFWI3ZVT0S2VO8P4P31AY8YUI8
#define IDKPYK79OFWI3ZVT0S2VO8P4P31AY8YUI8

///// Includes /////

//#include <soci/soci.h>
#include <string>

#include "user.hpp"

///// Namespaces /////

namespace license
{

///// Classes /////

class Database
{
 public:

  Database();
  ~Database();
  
  int Init(const std::string& sqlpassword);
  void Destroy();

  User GetUser(const std::string& email);
  void AddLicense(const int userid, const std::string& licensekey);

 private:
  
  //soci::session database_;

};

}

#endif