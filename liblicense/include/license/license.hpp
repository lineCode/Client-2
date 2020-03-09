// license.hpp
//

#ifndef ID7OLM5UZUE87VTD8FE79F7FSFOJSCA4TS
#define ID7OLM5UZUE87VTD8FE79F7FSFOJSCA4TS

///// Includes /////

#include <string>

///// Namespaces /////

namespace license
{

///// Classes /////

class License
{
 public:

  License();
  License(const std::string& license, unsigned int value);
  ~License();

  int Init(const std::string& license);

  std::string ToString() const;

  bool operator==(const License& rhs) const;

  std::string license_;
  unsigned int value_;

};

}

#endif