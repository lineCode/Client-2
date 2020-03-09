// user.hpp
//

#ifndef IDF69P3OAV5ULY5O7254Z6ETBRT0STD5UV
#define IDF69P3OAV5ULY5O7254Z6ETBRT0STD5UV

///// Includes /////

#include <string>

///// Namespaces /////

namespace license
{

///// Classes /////

class User
{
 public:

  User();
  User(const int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& password, const int verified);
  ~User();

  int GetId() const { return id_; }
  const std::string& GetFirstName() const { return firstname_; }
  const std::string& GetLastName() const { return lastname_; }
  const std::string& GetEmail() const { return email_; }
  const std::string& GetPassword() const { return password_; }
  int GetVerified() const { return verified_; }

 private:

  int id_;
  std::string firstname_;
  std::string lastname_;
  std::string email_;
  std::string password_;
  int verified_;

};

}

#endif