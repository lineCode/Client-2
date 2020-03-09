// licensesdk.hpp
//

#ifndef IDZT4PMLP91GGMN35HZVIHZE2B0VYX6BDG
#define IDZT4PMLP91GGMN35HZVIHZE2B0VYX6BDG

///// Includes /////

#include <array>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <stdint.h>
#include <string>
#include <utility/privatekey.hpp>
#include <utility/publickey.hpp>
#include <vector>

#include "license.hpp"
#include "productkey.hpp"

///// Namespaces /////

namespace license
{

///// Globals /////

extern const std::string ADDITIONALDEVICES;
extern const std::array<std::string, 1> LICENSES;

///// Classes /////

class LicenseKey
{
 public:

  LicenseKey();
  ~LicenseKey();
  
  int Load(const boost::filesystem::path& path);
  int Init(const std::string& licensekey);
  int Init(const ProductKey& productkey, const boost::gregorian::date& expires, const std::vector<License>& licenses, const utility::PrivateKey& privatekey);
  void Destroy();

  const ProductKey& GetProductKey() const { return productkey_; }
  void SetProductKey(const ProductKey& productkey) { productkey_ = productkey; }

  std::string GetExpiryDate() const;

  const std::vector<License>& GetLicenses() const { return licenses_; }
  std::vector<License>& GetLicenses() { return licenses_; }
  void SetLicenses(const std::vector<License>& licenses) { licenses_ = licenses; }

  const std::string& GetSignature() const { return signature_; }
  void SetSignature(const std::string& signature) { signature_ = signature; }

  boost::optional<unsigned int> GetLicenseValue(const std::string& license) const;

  std::string ToString() const; // Converts the product key, licenses and signature into something ready for the user

  bool IsExpired() const;
  bool IsValid(const utility::PublicKey& publickey) const;

  bool operator==(const LicenseKey& rhs) const;

 private:

  std::string MessageToString() const; // Used to create and compare the signature, only convert productkey and licenses

  ProductKey productkey_;
  boost::gregorian::date expires_;
  std::vector<License> licenses_;
  std::string signature_;

};

}

#endif