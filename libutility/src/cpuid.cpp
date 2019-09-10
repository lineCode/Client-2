// cpuid.cpp
//

///// Includes /////

#include "utility/cpuid.hpp"

#include <cstring>

#ifdef _WIN32
  #include <intrin.h>
#elif __arm__

#else
  #include <cpuid.h>
#endif

///// Namespaces /////

namespace utility
{

///// Methods /////

CPUID::CPUID() :
  hypervisor_(false)
{
#ifdef __arm__
  vendor_ = "ARM";
#else
  std::array<uint32_t, 4> result = CPUIDCall(0);

  char vendor[sizeof(uint32_t) * 3];
  memset(vendor, 0, sizeof(vendor));
  memcpy(vendor, &result[1], sizeof(uint32_t));
  memcpy(vendor + sizeof(uint32_t), &result[3], sizeof(uint32_t));
  memcpy(vendor + sizeof(uint32_t) * 2, &result[2], sizeof(uint32_t));
  vendor_ = std::string(vendor, sizeof(vendor));

  if (result[0] < 1)
  {

    return;
  }

  result = CPUIDCall(1);
  if (result[2] & (1 << 31))
  {
    hypervisor_ = true;

  }
#endif
}

std::array<uint32_t, 4> CPUID::CPUIDCall(const int op)
{
  std::array<uint32_t, 4> regs;
  regs.fill(0);
#ifdef _WIN32
  __cpuidex(reinterpret_cast<int*>(regs.data()), op, 0);
#elif __arm__
  regs.fill(0);
#elif __linux__
  __get_cpuid(op, &regs[0], &regs[1], &regs[2], &regs[3]);
#endif
  return regs;
}

}
