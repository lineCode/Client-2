// registry.cpp
//

#ifdef _WIN32

///// Includes /////

#include "utility/registry.hpp"

#include <memory>

///// Namespaces /////

namespace utility
{

///// Methods /////

RegistryKey::RegistryKey() :
  key_(nullptr)
{

}

RegistryKey::~RegistryKey()
{
  Destroy();

}

int RegistryKey::Init(HKEY key, const std::string& subkey, REGSAM sam)
{
  Destroy();

  if (RegOpenKeyEx(key, subkey.c_str(), 0, sam, &key_) != ERROR_SUCCESS)
  {

    return 1;
  }

  return 0;
}

void RegistryKey::Destroy()
{
  if (key_)
  {
    RegCloseKey(key_);
    key_ = nullptr;
  }
}

DWORD RegistryKey::QueryValueType(const std::string& name) const
{
  if (!key_)
  {

    return 0;
  }

  DWORD type;
  if (RegQueryValueEx(key_, name.c_str(), nullptr, &type, nullptr, nullptr) != ERROR_SUCCESS)
  {

    return 0;
  }

  return type;
}

std::string RegistryKey::GetString(const std::string& name) const
{
  if (!key_)
  {

    std::string();
  }

  DWORD type = REG_NONE;
  DWORD size = 0;
  if (RegQueryValueEx(key_, name.c_str(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS)
  {

    std::string();
  }

  std::unique_ptr<BYTE[]> buffer = std::make_unique<BYTE[]>(size);
  std::fill(buffer.get(), buffer.get() + size, '\0');
  if (RegQueryValueEx(key_, name.c_str(), NULL, NULL, buffer.get(), &size) != ERROR_SUCCESS)
  {

    std::string();
  }

  std::string result = std::string(reinterpret_cast<char*>(buffer.get()));
  size_t pos = result.find_first_of('\0');
  if (pos != std::string::npos)
  {
    result.resize(pos);

  }

  return result;
}

}

#endif
