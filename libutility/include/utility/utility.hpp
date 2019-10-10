// utility.hpp
//

#ifndef IDNQ1HH3ANB0P17XQFDHC05BRASATRZN4J
#define IDNQ1HH3ANB0P17XQFDHC05BRASATRZN4J

///// Includes /////

#include <boost/asio.hpp>

#include <algorithm>
#include <array>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <chrono>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <random>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <zlib.h>

#ifdef _WIN32
  #include <corecrt_malloc.h>
  #include <handleapi.h>
  #include <minwindef.h>
  #include <WinBase.h>
#else
  #include <arpa/inet.h>
#endif

///// Declarations /////

struct sockaddr_in6;
struct sockaddr_in;

///// Defines /////

#ifndef PAGE_SIZE
  #define PAGE_SIZE 4096
#endif

#ifdef _WIN32
  #define FILE_HANDLE HANDLE
  #define CLOSE_FILE_HANDLE CloseHandle
  #define INVALID_FILE_HANDLE INVALID_HANDLE_VALUE
#else
  #define FILE_HANDLE int
  #define CLOSE_FILE_HANDLE close
  #define INVALID_FILE_HANDLE -1
#endif

#define PAGE_ROUND_DOWN(x) (((size_t)(x)) & (~(PAGE_SIZE-1)))
#define PAGE_ROUND_UP(x) ((((size_t)(x)) + PAGE_SIZE-1)  & (~(PAGE_SIZE-1))) 

#define htonll(x) (((uint64_t)htonl((uint32_t)x) << 32) + (uint64_t)htonl((uint32_t)(x >> 32)))
#define ntohll(x) (((uint64_t)htonl((uint32_t)x) << 32) + (uint64_t)htonl((uint32_t)(x >> 32)))

///// Namespaces /////

namespace utility
{

///// Enumerations /////

enum class OPERATINGSYSTEM : int
{
  WINDOWS10,
  WINDOWS8,
  WINDOWS7,
  WINDOWSVISTA,
  WINDOWSXP,
  WINDOWS,
  LINUX
};

///// Structures /////

struct ADDRESS
{
  ADDRESS(const std::string& name, const std::string& address);

  std::string name_;
  std::string address_;
};

///// Functions /////

// Network
std::pair<int, std::vector<ADDRESS> > GetAddresses(); // <error code, addresses>
bool IsValidSubnetMask(const uint32_t subnetmask);
int SubnetMaskToPrefix(const uint32_t subnetmask);
std::string PrefixToSubnetMask(const int prefix);
bool IsIPV4InRange(const uint32_t ip, const uint32_t network, const uint32_t netmask);

// Containers
template<class T, class U>
static bool Intersects(const T& lhs, const U& rhs)
{
  for (const auto& element : lhs)
  {
    if (std::find(rhs.cbegin(), rhs.cend(), element) != rhs.cend())
    {

      return true;
    }
  }
  return false;
}

template<class T, class Pred>
static bool Unique(const T& vector, Pred pred) // Returns whether the container has any duplicate elements
{
  for (auto i = vector.cbegin(); i != vector.cend(); ++i)
  {
    for (auto j = i + 1; j != vector.cend(); ++j)
    {
      if (pred(*i, *j))
      {

        return false;
      }
    }
  }
  return true;
}

template<class T>
static bool Contains(const T& vector, const typename T::value_type& element)
{
  for(const auto& i : vector)
  {
    if(i == element)
    {

      return true;
    }
  }
  return false;
}

template<class T>
static bool Contains(const T& vector1, const T& vector2) // Intersection
{
  for(const auto& i : vector2)
  {
    if(!Contains(vector1, i))
    {

      return false;
    }
  }
  return true;
}

template<class T>
static std::vector< std::vector<T> > Split(const std::vector<T>& input, const std::vector<T>& sequence)
{
  if (sequence.empty())
  {

    return{ input };
  }

  std::vector< std::vector<T> > result;

  typename std::vector<T>::const_iterator prev = input.begin();
  typename std::vector<T>::const_iterator next = input.begin();
  while ((next = std::search(prev, input.end(), sequence.begin(), sequence.end())) != input.end())
  {
    result.push_back(std::vector<T>(prev, next));
    prev = next + sequence.size();
  }

  if (prev != input.end())
  {
    result.push_back(std::vector<T>(prev, input.end()));

  }

  return result;
}

template<class T, class Pred>
typename std::vector<T>::iterator InsertSorted(std::vector<T>& vec, const T& item, Pred pred)
{
  return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
}

template<class T, class Pred>
typename std::vector<T>::iterator EmplaceSorted(std::vector<T>& vec, T&& item, Pred pred)
{
  return vec.emplace(std::upper_bound(vec.begin(), vec.end(), item, pred), std::move(item));
}

template<class T>
std::vector< std::unique_ptr<T> > Copy(const std::vector< std::unique_ptr<T> >& vec)
{
  std::vector< std::unique_ptr<T> > result;
  result.reserve(vec.size());
  for (const std::unique_ptr<T>& e : vec)
  {
    result.emplace_back(std::make_unique<T>(*e));

  }
  return result;
}

// Strings
void ToString60Padding(const int num, std::vector<char>& buffer); // Converts values 0-60 to 2 character string without allocating a new string(useful for hours, minutes and seconds)
void ToString999(const int num, std::vector<char>& buffer);
void ToString999Padding(const int num, std::vector<char>& buffer); // Converts values 0-999 to a 3 character string without allocating a new string(useful for milliseconds)
void ToString4999(const int num, std::vector<char>& buffer); // Converts values 0-4999 to a 4 character string without allocating a new string(useful for years)
void ToString4999Padding(const int num, std::vector<char>& buffer); // Converts values 0-4999 to a character string without allocating a new string(useful for years)
bool IsDigits(const std::string& text);
bool StringToBool(const std::string& text);
const std::string& BoolToString(const bool value);
std::vector<uint8_t> HexToData(const std::string& text);
std::string GenerateRandomString(unsigned int length);
std::wstring GenerateRandomWString(unsigned int length);
std::vector<std::string> SplitString(const std::string& text, const std::vector<char>& delimeters);
std::vector<std::string> SplitString(const std::string& text, char delimeter);
std::string RemoveChars(const std::string& text, const std::vector<char> &chars);
std::string ToString(const std::wstring& text); // Returns "?" if unable to convert

template<class T>
std::vector<T> GenerateRandomData(unsigned int length)
{
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  thread_local std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

  std::vector<T> result;
  result.reserve(length);
  for (unsigned int i = 0; i < length; ++i)
  {
    result.push_back(dis(gen));

  }
  return result;
}

template<>
std::vector<unsigned char> GenerateRandomData(unsigned int length); // Silly specialisation because there currently is no uniform_int_distribution for unsigned char

template<>
std::vector<char> GenerateRandomData(unsigned int length); // Silly specialisation because there currently is no uniform_int_distribution for unsigned char

template<class T, unsigned int S>
std::array<T, S> GenerateRandomArray()
{
  const auto vec = GenerateRandomData<T>(S);
  std::array<T, S> tmp;
  std::memcpy(tmp.data(), vec.data(), S);
  return tmp;
}

// Mathematics
uint64_t FactorialDivide(unsigned int min, unsigned int max);
uint64_t Factorial(unsigned int value);
std::vector<int> SplitDigits(int number); // Split an int into its individual digits

template <typename T>
T Clip(const T& value, const T& min, const T& max)
{
  return std::max(min, std::min(value, max));
}

template<typename T>
T Min(T&& t)
{
  return std::forward<T>(t);
}

template<typename T0, typename T1, typename... Ts>
typename std::common_type<T0, T1, Ts...>::type Min(T0&& val1, T1&& val2, Ts&&... vs)
{
  if (val1 < val2)
    return Min(val1, std::forward<Ts>(vs)...);
  else
    return Min(val2, std::forward<Ts>(vs)...);
}

template<typename T>
T Max(T&& t)
{
  return std::forward<T>(t);
}

template<typename T0, typename T1, typename... Ts>
typename std::common_type<T0, T1, Ts...>::type Max(T0&& val1, T1&& val2, Ts&&... vs)
{
  if (val1 > val2)
    return Max(val1, std::forward<Ts>(vs)...);
  else
    return Max(val2, std::forward<Ts>(vs)...);
}

// Encoding
std::array<unsigned char, 32> Sha256(const std::string& text);
std::array<unsigned char, 32> Sha256(unsigned char const* data, const size_t length);
std::string Md5(const std::string& text);
std::string Md5(const void* data, size_t size);
bool IsBase64(unsigned char c);
std::string Base64Encode(const std::string& text);
std::string Base64Encode(unsigned char const* data, const size_t length);
std::vector<unsigned char> Base64Decode(std::string input);
std::string XMLEscape(const std::string& xml);

// Compiler/OS/Architecture
const std::string& GetArchitecture();
OPERATINGSYSTEM GetOperatingSystem();
bool IsLinux(const OPERATINGSYSTEM operatingsystem);
bool IsWindows(const OPERATINGSYSTEM operatingsystem);
boost::optional<OPERATINGSYSTEM> GetOperatingSystem(const int operatingsystem);
const std::string& ToString(const OPERATINGSYSTEM operatingsystem);
const std::string& GetCompiler();
std::vector<std::string> GetEnvironmentVariables();
std::string GetComputerName();
std::string GetUsername();
bool IsBigEndian();

// Misc
void Sleep(const std::chrono::milliseconds duration);
void SwapBit(unsigned char* data, unsigned int lhs, unsigned int rhs); // Swaps the bits located at lhs and rhs
std::vector< std::pair<unsigned int, unsigned int> > GenerateRandomSequence(unsigned int seed, unsigned int miniterations, unsigned int max); // The ProductKey and LicenseKey classes use this and will always generate the same data if the same seed is used
size_t RoundPage(const size_t value);
uint64_t RoundUp(const uint64_t value, const uint64_t multiple);

template<typename T>
T RandomNumber()
{
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  thread_local std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  return dis(gen);
}

// Address
std::string ToString(sockaddr_in* addr);
std::string ToString(sockaddr_in6* addr);

// AES
int AESEncrypt(const std::array<unsigned char, 32>& key, const std::array<unsigned char, 16>& iv, const unsigned char* data1, const int size1, const unsigned char* data2, const int size2, const unsigned char* data3, const int size3, std::vector<unsigned char>& cipher);
int AESDecrypt(const std::array<unsigned char, 32>& key, const unsigned char* iv, const unsigned char* cipher, const int size, std::vector<unsigned char>& data);

// Files
std::string MIMEType(const std::string& extension);
boost::filesystem::path GetMountPoint(const boost::filesystem::path& directory);
boost::filesystem::path GetHomeDirectory();
boost::filesystem::path GetSystemConfigurationDirectory();
bool IsParent(const boost::filesystem::path& parent, const boost::filesystem::path& child); // Will find if any parent of child is parent(it will search up the hierarchy until it matches or fails)
#ifdef _WIN32
  int64_t FiletimeToInt(const FILETIME& filetime);
#endif

// Aligned allocation, this is useful for APIs that require page aligned memory. These functions *may* overallocate
void* AlignedMalloc(const size_t size, const size_t alignment);
void AlignedFree(void* ptr);

template<class T>
struct DeleteAligned
{
  void operator()(T* ptr) const
  {
    ptr->~T();
#ifdef _WIN32
    _aligned_free(ptr);
#else
    AlignedFree(ptr);
#endif
  }
};

template<class T, class... Args>
std::unique_ptr< T, DeleteAligned<T> > AlignedAlloc(const int alignment, Args... args)
{
#ifdef _WIN32
  T* buf = static_cast<T*>(_aligned_malloc(sizeof(T), alignment));
#else
  T* buf = static_cast<T*>(AlignedMalloc(sizeof(T), alignment));
#endif
  T* ptr = new (buf) T(args...);
  return std::unique_ptr< T, DeleteAligned<T> >{ptr};
}

template<class T>
std::unique_ptr< T[], DeleteAligned<T> > AlignedAllocArray(const int alignment, const int length)
{
#ifdef _WIN32
  T* ptr = static_cast<T*>(_aligned_malloc(sizeof(T) * length, alignment));
#else
  T* ptr = static_cast<T*>(AlignedMalloc(sizeof(T) * length, alignment));
#endif
  return std::unique_ptr< T[], DeleteAligned<T> >(ptr);
}

// Chrono
std::string ToISOString(const std::chrono::system_clock::time_point timepoint);

// Compression
int Compress(z_stream& zstream, const char* data, const size_t size, std::vector<char>& buffer);
int Decompress(z_stream& zstream, const char* data, const size_t size, std::vector<char>& buffer);

}

#endif
