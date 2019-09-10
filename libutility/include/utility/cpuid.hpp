// cpuid.hpp
//

#ifndef IDENFCXUZG1ONK73GSJVXMHAU6WYBM77DZ
#define IDENFCXUZG1ONK73GSJVXMHAU6WYBM77DZ

///// Includes /////

#include <array>
#include <stdint.h>
#include <string>

///// Namespaces /////

namespace utility
{

///// Classes /////

class CPUID
{
 public:

  CPUID();

  inline const std::string& GetVendor() const { return vendor_; }
  inline bool IsHypervisor() const { return hypervisor_; }

 private:

  std::array<uint32_t, 4> CPUIDCall(const int op);

  std::string vendor_;
  bool hypervisor_;

};

}

#endif
