// productkey.hpp
//

#ifndef IDGG2P94L2T0DDDTSMH7HFFA4O2W1BXNKM
#define IDGG2P94L2T0DDDTSMH7HFFA4O2W1BXNKM

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

///// Namespaces /////

namespace license
{

///// Enumerations /////

enum PROCESSORTYPE
{
  PROCESSORTYPE_AMD64 = 0,
  PROCESSORTYPE_INTEL = 1,
  PROCESSORTYPE_ARM = 2,
  PROCESSORTYPE_IA64 = 3,
  PROCESSORTYPE_UNKNOWN = 4
};

///// Declarations /////

class ProductKey;

///// Prototypes /////

bool FuzzyMatch(const ProductKey& lhs, const ProductKey& rhs);

///// Classes /////

class ProductKey
{
 public:

  ProductKey();
  ProductKey(uint64_t memorysize, unsigned int numlogicalprocessors, const std::vector<std::string>& macaddresses, const std::vector<std::string>& harddiskserials, PROCESSORTYPE processortype, const std::string& motherboardserial, const std::vector<uint32_t>& videoram, unsigned int operatingsystemversion, const std::string& productid, unsigned int numcddvddrives);
  ProductKey(uint64_t memorysize, unsigned int numlogicalprocessors, const std::vector<std::string>& macaddresses);
  ~ProductKey();

  int Init();
  int Init(const std::string& productkey);
  void Destroy();

  uint64_t GetMemorySize() const { return memorysize_; }
  void SetMemorySize(uint64_t memorysize) { memorysize_ = memorysize; }

  unsigned int GetNumLogicalProcessors() const { return numlogicalprocessors_; }
  void SetNumLogicalProcessors(unsigned int numlogicalprocessors) { numlogicalprocessors_ = numlogicalprocessors; }

  const std::vector<std::string>& GetMacAddresses() const { return macaddresses_; }
  void SetMacAddresses(const std::vector<std::string>& macaddresses) { macaddresses_ = macaddresses; }

  const std::vector<std::string>& GetHardDiscSerials() const { return harddiskserials_; }
  void SetHardDiscSerials(const std::vector<std::string>& harddiskserials) { harddiskserials_ = harddiskserials; }

  PROCESSORTYPE GetProcessorType() const { return processortype_; }
  void SetProcessorType(PROCESSORTYPE processortype) { processortype_ = processortype; }

  const std::string& GetMotherboardSerial() const { return motherboardserial_; }
  void SetMotherboardSerial(const std::string& motherboardserial) { motherboardserial_ = motherboardserial; }

  const std::vector<uint32_t>& GetVideoRam() const { return videoram_; }
  void SetVideoRam(const std::vector<uint32_t>& videoram) { videoram_ = videoram; }

  unsigned int GetOperationSystemVersion() const { return operatingsystemversion_; }
  void SetOperationSystemVersion(unsigned int operatingsystemversion) { operatingsystemversion_ = operatingsystemversion; }

  const std::string& GetProductId() const { return productid_; }
  void SetProductId(const std::string& productid) { productid_ = productid; }

  unsigned int GetNumCdDvdDrives() const { return numcddvddrives_; }
  void SetNumCdDvdDrives(unsigned int numcddvddrives) { numcddvddrives_ = numcddvddrives; }

  std::string ToString() const;

  bool operator==(const ProductKey& rhs) const;

 private:

  std::vector<std::string> Convert(const std::vector<uint32_t>& vec) const;

  uint64_t memorysize_;
  unsigned int numlogicalprocessors_;
  std::vector<std::string> macaddresses_;
  std::vector<std::string> harddiskserials_;
  PROCESSORTYPE processortype_;
  std::string motherboardserial_;
  std::vector<uint32_t> videoram_;
  unsigned int operatingsystemversion_;
  std::string productid_;
  unsigned int numcddvddrives_;

};

}

#endif
