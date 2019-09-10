// version.hpp
//

#ifndef IDENFCXUZG1ONK7XGSJVXMHAU6WYBM77DT
#define IDENFCXUZG1ONK7XGSJVXMHAU6WYBM77DT

///// Includes /////

#include <string>

///// Namespaces /////

namespace utility
{

///// Classes /////

class Version
{
 public:

  Version();
  Version(unsigned int major, unsigned int minor, unsigned int build);
  ~Version();

  std::string ToString() const;

  inline unsigned int GetMajor() const { return major_; }
  inline unsigned int GetMinor() const { return minor_; }
  inline unsigned int GetBuild() const { return build_; }
  
  bool operator==(const Version& rhs) const;
  bool operator!=(const Version& rhs) const;
  Version& operator=(const Version& rhs);
  bool operator>(const Version& rhs) const;
  bool operator<(const Version& rhs) const;
  bool operator>=(const Version& rhs) const;
  bool operator<=(const Version& rhs) const;

 private:

  unsigned int major_;
  unsigned int minor_;
  unsigned int build_;



};

}

#endif
