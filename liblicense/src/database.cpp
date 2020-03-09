// database.hpp
//

///// Includes /////

#include "license/database.hpp"

///// Namespaces /////

namespace license
{

///// Methods /////

Database::Database()
{

}

Database::~Database()
{
  //database_.close();

}

int Database::Init(const std::string& sqlpassword)
{
  /*try
  {
    database_.open("mysql", "host=localhost port=3306 dbname=monoclesecurity user=monoclesecurity password=" + sqlpassword);

  }
  catch (const soci::soci_error&)
  {

    return 1;
  }*/
  
  return 0;
}

void Database::Destroy()
{
  //database_.close();

}

User Database::GetUser(const std::string& email)
{
  int id;
  std::string firstname;
  std::string lastname;
  std::string password;
  int verified;
  //database_ << std::string("SELECT id, firstname, lastname, password, verified FROM users WHERE email = :e"), soci::into(id), soci::into(firstname), soci::into(lastname), soci::into(password), soci::into(verified), soci::use(email, "e");

  return User(id, firstname, lastname, email, password, verified);
}

void Database::AddLicense(const int userid, const std::string& licensekey)
{
  //database_ << std::string("INSERT INTO licenses(userid, licensekey) VALUES (:u, :l)"), soci::use(userid, "u"), soci::use(licensekey, "l");

}

}
