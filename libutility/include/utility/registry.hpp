// registry.hpp
//

#ifndef IDF6ON78IC3U3GT2FKYEE9TOW7ESYFAJC1
#define IDF6ON78IC3U3GT2FKYEE9TOW7ESYFAJC1

#ifdef _WIN32

///// Includes /////

#include <string>
#include <windows.h>

///// Namespaces /////

namespace utility
{

///// Classes /////

class RegistryKey
{
 public:

  RegistryKey();
  ~RegistryKey();

  int Init(HKEY key, const std::string& subkey, REGSAM sam);
  void Destroy();

  DWORD QueryValueType(const std::string& name) const;

  std::string GetString(const std::string& name) const;

 private:

  HKEY key_;

};

}

#endif

#endif
