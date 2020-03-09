// productkey.hpp
//

///// Includes /////

#include "license/productkey.hpp"

#ifdef _WIN32
  #include <WinSock2.h>
#endif

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/locale/encoding_utf.hpp>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <pugixml.hpp>
#include <random>
#include <regex>
#include <sstream>
#include <thread>
#include <utility/utility.hpp>
#include <vector>

#ifdef _WIN32
  #include <boost/scope_exit.hpp>
  #include <comdef.h>
  #include <Iphlpapi.h>
  #include <lm.h>
  #include <Tchar.h>
  #include <Wbemidl.h>
#endif

#ifdef __linux__
  #include <ifaddrs.h>
  #include <linux/if_packet.h>
  #include <mntent.h>
  #include <sys/socket.h>
  #include <sys/sysinfo.h>
  #include <sys/types.h>
#endif

///// Namespaces /////

namespace license
{

///// Functions /////

bool Match(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs) // Find one occurence of an item in one container with another
{
  for (const auto& item : lhs)
  {
    if (std::find(rhs.begin(), rhs.end(), item) == rhs.end())
    {

      return false;
    }
  }

  return true;
}

bool FuzzyMatch(const ProductKey& lhs, const ProductKey& rhs)
{
  if (lhs.GetProcessorType() != rhs.GetProcessorType())
  {

    return false;
  }

  if (lhs.GetMotherboardSerial() != rhs.GetMotherboardSerial())
  {

    return false;
  }

  // We must find a single matching MAC address for the product key to match
  bool foundsinglematch = false;
  for (const auto& item : lhs.GetMacAddresses())
  {
    if (std::find(rhs.GetMacAddresses().begin(), rhs.GetMacAddresses().end(), item) != rhs.GetMacAddresses().end())
    {
      foundsinglematch = true;
      break;
    }
  }

  if (!foundsinglematch)
  {

    return false;
  }

  unsigned int diff = 0; // Number of significant differences between product keys
  if (lhs.GetMemorySize() != rhs.GetMemorySize())
  {
    ++diff;
    
  }

  if (lhs.GetNumLogicalProcessors() != rhs.GetNumLogicalProcessors())
  {
    ++diff;

  }

  if (!Match(lhs.GetHardDiscSerials(), rhs.GetHardDiscSerials()))
  {
    ++diff;

  }

  if (!std::is_permutation(lhs.GetVideoRam().begin(), lhs.GetVideoRam().end(), rhs.GetVideoRam().begin(), rhs.GetVideoRam().end()))
  {
    ++diff;

  }

  if (lhs.GetOperationSystemVersion() != rhs.GetOperationSystemVersion())
  {
    ++diff;

  }

  if (lhs.GetProductId() != rhs.GetProductId())
  {
    ++diff;

  }

  if (lhs.GetNumCdDvdDrives() != rhs.GetNumCdDvdDrives())
  {
    ++diff;

  }

  if (diff > 3)
  {

    return false;
  }

  return true;
}

///// Methods /////

ProductKey::ProductKey() :
  memorysize_(0),
  numlogicalprocessors_(0),
  processortype_(PROCESSORTYPE_UNKNOWN),
  operatingsystemversion_(0),
  numcddvddrives_(0)
{

}

ProductKey::ProductKey(uint64_t memorysize, unsigned int numlogicalprocessors, const std::vector<std::string>& macaddresses, const std::vector<std::string>& harddiskserials, PROCESSORTYPE processortype, const std::string& motherboardserial, const std::vector<uint32_t>& videoram, unsigned int operatingsystemversion, const std::string& productid, unsigned int numcddvddrives) :
  memorysize_(memorysize),
  numlogicalprocessors_(numlogicalprocessors),
  macaddresses_(macaddresses),
  harddiskserials_(harddiskserials),
  processortype_(processortype),
  motherboardserial_(motherboardserial),
  videoram_(videoram),
  operatingsystemversion_(operatingsystemversion),
  productid_(productid),
  numcddvddrives_(numcddvddrives)
{

}

ProductKey::ProductKey(uint64_t memorysize, unsigned int numlogicalprocessors, const std::vector<std::string>& macaddresses) :
  memorysize_(memorysize),
  numlogicalprocessors_(numlogicalprocessors),
  macaddresses_(macaddresses),
  processortype_(PROCESSORTYPE_UNKNOWN),
  operatingsystemversion_(0),
  numcddvddrives_(0)
{

}

ProductKey::~ProductKey()
{

}

int ProductKey::Init()
{
  Destroy();

#ifdef _WIN32
  // Memory
  if (!GetPhysicallyInstalledSystemMemory(&memorysize_))
  {

    return 1;
  }

  if (memorysize_ == 0)
  {

    return 1;
  }

  // Number of processors and their type
  SYSTEM_INFO systeminfo;
  GetSystemInfo(&systeminfo);
  switch (systeminfo.wProcessorArchitecture)
  {
    case PROCESSOR_ARCHITECTURE_AMD64:
    {
      processortype_ = PROCESSORTYPE_AMD64;
      break;
    }
    case PROCESSOR_ARCHITECTURE_INTEL:
    {
      processortype_ = PROCESSORTYPE_INTEL;
      break;
    }
    case PROCESSOR_ARCHITECTURE_ARM:
    {
      processortype_ = PROCESSORTYPE_ARM;
      break;
    }
    case PROCESSOR_ARCHITECTURE_IA64:
    {
      processortype_ = PROCESSORTYPE_IA64;
      break;
    }
    default:
    {
      processortype_ = PROCESSORTYPE_UNKNOWN;
      break;
    }
  }

  numlogicalprocessors_ = static_cast<unsigned int>(systeminfo.dwNumberOfProcessors);

  // Motherboard ID
  IWbemLocator* locator = nullptr;
  if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator)))
  {

    return 1;
  }

  BOOST_SCOPE_EXIT(locator)
  {
    locator->Release();
  } BOOST_SCOPE_EXIT_END

  IWbemServices* service = nullptr;
  if (FAILED(locator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &service)))
  {

    return 1;
  }

  BOOST_SCOPE_EXIT(service)
  {
    service->Release();
  } BOOST_SCOPE_EXIT_END

  if (FAILED(CoSetProxyBlanket(service, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
  {

    return 1;
  }

  IEnumWbemClassObject* enumerator = nullptr;
  if (FAILED(service->ExecQuery(bstr_t("WQL"), bstr_t("SELECT SerialNumber FROM Win32_Baseboard"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator)))
  {

    return 1;
  }

  BOOST_SCOPE_EXIT(enumerator)
  {
    enumerator->Release();
  } BOOST_SCOPE_EXIT_END

  std::vector<std::wstring> serialnumbers;
  while (enumerator)
  {
    IWbemClassObject* valueobject = nullptr;
    ULONG returnvalue = 0;
    if (FAILED(enumerator->Next(WBEM_INFINITE, 1, &valueobject, &returnvalue)))
    {
      
      return 1;
    }

    if (returnvalue == 0)
    {

      break;
    }

    BOOST_SCOPE_EXIT(valueobject)
    {
      valueobject->Release();
    } BOOST_SCOPE_EXIT_END

    VARIANT value;
    if (FAILED(valueobject->Get(L"SerialNumber", 0, &value, 0, 0)))
    {

      return 1;
    }
    serialnumbers.push_back(std::wstring(value.bstrVal, SysStringLen(value.bstrVal)));
    VariantClear(&value);
  }

  if (serialnumbers.size() > 1)
  {

    return 1;
  }

  motherboardserial_ = boost::locale::conv::utf_to_utf<char>(serialnumbers.at(0));

  // Graphics Card RAM
  if (FAILED(service->ExecQuery(bstr_t("WQL"), bstr_t("SELECT AdapterRAM FROM Win32_VideoController"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator)))
  {

    return 1;
  }

  while (enumerator)
  {
    IWbemClassObject* valueobject = nullptr;
    ULONG returnvalue = 0;
    if (FAILED(enumerator->Next(WBEM_INFINITE, 1, &valueobject, &returnvalue)))
    {

      return 1;
    }

    if (returnvalue == 0)
    {

      break;
    }

    VARIANT value;
    if (FAILED(valueobject->Get(L"AdapterRAM", 0, &value, 0, 0)))
    {

      return 1;
    }
    videoram_.push_back(value.uintVal);
    VariantClear(&value);
    valueobject->Release();
  }

  // Operating system
  LPBYTE data = nullptr;
  if (NetWkstaGetInfo(NULL, 100, &data) != NERR_Success)
  {

    return 1;
  }

  operatingsystemversion_ = reinterpret_cast<WKSTA_INFO_100*>(data)->wki100_ver_major;
  NetApiBufferFree(data);

  // Product ID
  HKEY key = NULL;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
  {

    return 1;
  }

  DWORD type = 0;
  if (RegQueryValueEx(key, "ProductId", NULL, &type, NULL, nullptr) != ERROR_SUCCESS)
  {
    RegCloseKey(key);
    return 1;
  }

  if (type != REG_SZ)
  {
    RegCloseKey(key);
    return 1;
  }

  DWORD valuesize = 0;
  if (RegQueryValueEx(key, "ProductId", nullptr, nullptr, nullptr, &valuesize) != ERROR_SUCCESS)
  {
    RegCloseKey(key);
    return 1;
  }

  BYTE* productid = new BYTE[valuesize];
  BOOST_SCOPE_EXIT(productid)
  {
    delete[] productid;
  } BOOST_SCOPE_EXIT_END
  if (RegQueryValueEx(key, "ProductId", nullptr, nullptr, productid, &valuesize) != ERROR_SUCCESS)
  {
    RegCloseKey(key);
    return 1;
  }

  productid_ = std::string(reinterpret_cast<char*>(productid), valuesize);
  if (productid_.empty())
  {
    return 1;
  }

  RegCloseKey(key);

  // MAC Addresses
  DWORD size = 0;
  if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, nullptr, &size) != ERROR_BUFFER_OVERFLOW)
  {
    
    return 1;
  }

  IP_ADAPTER_ADDRESSES* adapters = (IP_ADAPTER_ADDRESSES*)malloc(size);
  BOOST_SCOPE_EXIT(adapters)
  {
    free(adapters);
  } BOOST_SCOPE_EXIT_END
  if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapters, &size) != ERROR_SUCCESS)
  {

    return 1;
  }

  for (const IP_ADAPTER_ADDRESSES* adapter = adapters; adapter != NULL; adapter = adapter->Next)
  {
    if (adapter->PhysicalAddressLength != 6) // Ignore any weird mac addresses
    {

      continue;
    }

    std::string macaddress;
    for (DWORD i = 0; i < adapter->PhysicalAddressLength; ++i)
    {
      std::stringstream stream;
      stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(adapter->PhysicalAddress[i]);
      macaddress.append(stream.str());
    }

    if (std::find(macaddresses_.begin(), macaddresses_.end(), macaddress) != macaddresses_.end())
    {

      continue;
    }

    macaddresses_.push_back(macaddress);
  }

  if (macaddresses_.empty())
  {

    return 1;
  }

  // Hard Drives
  char name[MAX_PATH];
  HANDLE volume = FindFirstVolume(name, sizeof(name));
  do
  {
    size_t len = strlen(name);
    if (name[len - 1] == '\\')
    {
      name[len - 1] = '\0';

    }

    HANDLE file = CreateFile(name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {

      continue;
    }

    STORAGE_DEVICE_NUMBER sdn;
    DWORD bytes;
    if (!DeviceIoControl(file, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &bytes, NULL))
    {
      CloseHandle(file);
      continue;
    }

    CloseHandle(file);

    if (sdn.DeviceType != FILE_DEVICE_DISK)
    {

      continue;
    }

    HANDLE device = CreateFile((std::string("\\\\.\\PhysicalDrive") + std::to_string(sdn.DeviceNumber)).c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (!device)
    {

      continue;
    }

    BOOST_SCOPE_EXIT(device)
    {
      CloseHandle(device);
    } BOOST_SCOPE_EXIT_END

    STORAGE_PROPERTY_QUERY spq;
    STORAGE_DESCRIPTOR_HEADER sdh;
    DWORD size = 0;

    ZeroMemory(&spq, sizeof(STORAGE_PROPERTY_QUERY));
    spq.PropertyId = StorageDeviceProperty;
    spq.QueryType = PropertyStandardQuery;

    if (!DeviceIoControl(device, IOCTL_STORAGE_QUERY_PROPERTY, &spq, sizeof(STORAGE_PROPERTY_QUERY), &sdh, sizeof(STORAGE_DESCRIPTOR_HEADER), &size, NULL))
    {

      continue;
    }

    BYTE* buf = new BYTE[sdh.Size];
    BOOST_SCOPE_EXIT(buf)
    {
      delete[] buf;
    } BOOST_SCOPE_EXIT_END
    ZeroMemory(buf, sdh.Size);

    if (!DeviceIoControl(device, IOCTL_STORAGE_QUERY_PROPERTY, &spq, sizeof(STORAGE_PROPERTY_QUERY), buf, sdh.Size, &size, NULL))
    {

      continue;
    }

    if (reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buf)->SerialNumberOffset != 0)
    {
      harddiskserials_.push_back(boost::trim_copy(std::string(reinterpret_cast<char*>(buf + reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buf)->SerialNumberOffset))));

    }
  } while (FindNextVolume(volume, name, sizeof(name)));

  FindVolumeClose(volume);

  if (harddiskserials_.empty())
  {

    return 1;
  }

  // CD/DVD drives
  DWORD length = GetLogicalDriveStrings(0, nullptr);
  if (length == 0)
  {

    return 1;
  }

  TCHAR* drives = new TCHAR[length + 1];
  BOOST_SCOPE_EXIT(drives)
  {
    delete[] drives;
  } BOOST_SCOPE_EXIT_END
  const DWORD ret = GetLogicalDriveStrings(length + 1, drives);
  if ((ret == 0) || (ret > length + 1))
  {

    return 1;
  }

  TCHAR* drive = drives;
  while (*drive)
  {
    if (GetDriveType(drive) == DRIVE_CDROM)
    {
      ++numcddvddrives_;

    }

    drive += std::string(drive).length() + 1;
  }
#else
  // Memory and processors
  struct sysinfo info;
  if (sysinfo(&info))
  {

    return 1;
  }

  memorysize_ = info.totalram;

  numlogicalprocessors_ = std::thread::hardware_concurrency();

  // Mac address
  struct ifaddrs* ifaddrs = nullptr;
  if (getifaddrs(&ifaddrs))
  {

    return 1;
  }

  for (struct ifaddrs* ifaddr = ifaddrs; ifaddr != NULL; ifaddr = (ifaddr)->ifa_next)
  {
    if (ifaddr->ifa_flags & IFF_LOOPBACK)
    {

      continue;
    }

    std::stringstream ss;
    if (ifaddr->ifa_addr->sa_family == AF_PACKET)
    {
      const struct sockaddr_ll* s = reinterpret_cast<const sockaddr_ll*>(ifaddr->ifa_addr);
      for (int i = 0; i < 6; ++i)
      {
        ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<uint16_t>(s->sll_addr[i]);

      }

      macaddresses_.push_back(ss.str());
    }
  }

  freeifaddrs(ifaddrs);

  if (macaddresses_.empty())
  {

    return 1;
  }
#endif
  return 0;
}

int ProductKey::Init(const std::string& productkey)
{
  Destroy();

  if (productkey.empty())
  {

    return 1;
  }

  if (productkey.size() > 1000) // Don't want to kill ourselves trying to decode an obviously illegal product key
  {

    return 1;
  }

  // Decode the product key
  auto data = utility::Base64Decode(productkey);
  if (data.empty())
  {

    return 1;
  }

  const std::vector< std::pair<unsigned int, unsigned int> > sequence = utility::GenerateRandomSequence(static_cast<unsigned int>(data.size()), 5000, static_cast<unsigned int>((data.size() - 1) * sizeof(std::string::value_type) * 8));
  for (auto i = sequence.rbegin(); i != sequence.rend(); ++i)
  {
    utility::SwapBit(data.data(), i->first, i->second);
    
  }

  // Parse the product key
  std::vector<std::string> parameters;
  std::string tmpdata(reinterpret_cast<const char*>(data.data()), data.size());
  boost::split(parameters, tmpdata, boost::is_any_of("\n"));
  if (parameters.size() != 10)
  {

    return 1;
  }

  static std::regex memorysizeregex("[0-9]+");
  if (!std::regex_match(parameters.at(0), memorysizeregex))
  {

    return 1;
  }
  memorysize_ = std::strtoull(parameters.at(0).c_str(), nullptr, 10);

  static std::regex numlogicalprocessorsregex("[0-9]+");
  if (!std::regex_match(parameters.at(1), numlogicalprocessorsregex))
  {

    return 1;
  }
  numlogicalprocessors_ = std::strtoul(parameters.at(1).c_str(), nullptr, 10);

  std::vector<std::string> macaddresses;
  boost::split(macaddresses, parameters.at(2), boost::is_any_of(" "));
  for (const auto& macaddress : macaddresses)
  {
    macaddresses_.push_back(macaddress);

  }

  std::vector<std::string> harddiskserials;
  boost::split(harddiskserials, parameters.at(3), boost::is_any_of(" "));
  for (const auto& harddiskserial : harddiskserials)
  {
    if (harddiskserial.empty())
    {
    
      continue;
    }

    harddiskserials_.push_back(harddiskserial);
  }

  static std::regex processortyperegex("[0-4]");
  if (!std::regex_match(parameters.at(4), processortyperegex))
  {

    return 1;
  }
  processortype_ = static_cast<PROCESSORTYPE>(std::strtoul(parameters.at(4).c_str(), nullptr, 10));

  motherboardserial_ = parameters.at(5);

  std::vector<std::string> videorams;
  boost::split(videorams, parameters.at(6), boost::is_any_of(" "));
  for (const auto& videoram : videorams)
  {
    if (videoram.empty())
    {
    
      continue;
    }

    static std::regex videoramregex("[0-9]+");
    if (!std::regex_match(videoram, videoramregex))
    {

      return 1;
    }
    videoram_.push_back(std::strtoul(videoram.c_str(), nullptr, 10));
  }

  static std::regex operatingsystemversionregex("[0-9]+");
  if (!std::regex_match(parameters.at(7), operatingsystemversionregex))
  {

    return 1;
  }
  operatingsystemversion_ = std::strtoul(parameters.at(7).c_str(), nullptr, 10);

  productid_ = parameters.at(8);

  static std::regex numcddvddrivesregex("[0-9]+");
  if (!std::regex_match(parameters.at(9), numcddvddrivesregex))
  {

    return 1;
  }
  numcddvddrives_ = std::strtoul(parameters.at(9).c_str(), nullptr, 10);

  return 0;
}

void ProductKey::Destroy()
{
  memorysize_ = 0;
  numlogicalprocessors_ = 0;
  macaddresses_.clear();
  harddiskserials_.clear();
  processortype_ = PROCESSORTYPE_UNKNOWN;
  motherboardserial_.clear();
  videoram_.clear();
  operatingsystemversion_ = 0;
  productid_.clear();
  numcddvddrives_ = 0;
}

std::string ProductKey::ToString() const
{
  std::string result;
  result += std::to_string(memorysize_) + '\n';
  result += std::to_string(numlogicalprocessors_) + '\n';
  result += boost::join(macaddresses_, std::string(" ")) + '\n';
  result += boost::join(harddiskserials_, std::string(" ")) + '\n';
  result += std::to_string(processortype_) + '\n';
  result += motherboardserial_ + '\n';
  result += boost::join(Convert(videoram_), std::string(" ")) + '\n';
  result += std::to_string(operatingsystemversion_) + '\n';
  result += productid_ + '\n';
  result += std::to_string(numcddvddrives_);

  // Copy to a data array
  std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(result.length());
  std::copy(result.begin(), result.end(), data.get());

  // Jumble the data
  const std::vector< std::pair<unsigned int, unsigned int> > sequence = utility::GenerateRandomSequence(static_cast<unsigned int>(result.length()), 5000, static_cast<unsigned int>((result.length() - 1) * sizeof(std::string::value_type) * 8));
  for (const auto& pair : sequence)
  {
    utility::SwapBit(data.get(), pair.first, pair.second);

  }

  return utility::Base64Encode(data.get(), result.length());
}

bool ProductKey::operator==(const ProductKey& rhs) const
{
  return ((memorysize_ == rhs.memorysize_) && (numlogicalprocessors_ == rhs.numlogicalprocessors_) && (macaddresses_ == rhs.macaddresses_) && (harddiskserials_ == rhs.harddiskserials_) && (processortype_ == rhs.processortype_) && (motherboardserial_ == rhs.motherboardserial_) && (videoram_ == rhs.videoram_) && (operatingsystemversion_ == rhs.operatingsystemversion_) && (numcddvddrives_ == rhs.numcddvddrives_));
}

std::vector<std::string> ProductKey::Convert(const std::vector<uint32_t>& vec) const
{
  std::vector<std::string> result;
  for (uint32_t element : vec)
  {
    result.push_back(std::to_string(element));

  }
  return result;
}

}
