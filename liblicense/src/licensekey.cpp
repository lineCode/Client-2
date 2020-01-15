// licensesdk.hpp
//

///// Includes /////

#include "license/licensekey.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace license
{

///// Globals /////

const unsigned int NUM_OBFUSCATE_ITERATIONS = 5000;
const std::string ADDITIONALDEVICES("additional_devices");
const std::array<std::string, 1> LICENSES = { ADDITIONALDEVICES };
const std::string UNLIMITEDEXPIRY("unlimited_expiry");

///// Methods /////

LicenseKey::LicenseKey()
{

}

LicenseKey::~LicenseKey()
{

}

int LicenseKey::Load(const boost::filesystem::path& path)
{
  std::ifstream file(path.string());
  if (!file.is_open())
  {
  
    return 1;
  }
  return Init(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
}

int LicenseKey::Init(const std::string& licensekey)
{
  if (licensekey.empty())
  {

    return 1;
  }

  if (licensekey.size() > 3000)
  {

    return 1;
  }

  // Decode the license key
  auto data = utility::Base64Decode(licensekey);
  if (data.empty())
  {

    return 1;
  }
  
  const std::vector< std::pair<unsigned int, unsigned int> > sequence = utility::GenerateRandomSequence(static_cast<unsigned int>(data.size()), NUM_OBFUSCATE_ITERATIONS, static_cast<unsigned int>((data.size() - 1) * sizeof(std::string::value_type) * 8));
  for (auto i = sequence.rbegin(); i != sequence.rend(); ++i)
  {
    utility::SwapBit(data.data(), i->first, i->second);

  }

  // Parse the license key
  static const boost::regex splitregex("\n\n");
  std::vector<std::string> components;
  boost::split_regex(components, std::string(data.begin(), data.end()), splitregex);
  if (components.size() != 4)
  {

    return 1;
  }

  // Product key
  if (productkey_.Init(components.at(0)))
  {

    return 1;
  }

  // Expiry date
  if (components.at(1) == UNLIMITEDEXPIRY)
  {
    expires_ = boost::gregorian::date(boost::gregorian::pos_infin);

  }
  else
  {
    try
    {
      expires_ = boost::gregorian::from_simple_string(components.at(1));
      if (expires_.is_not_a_date())
      {

        return 1;
      }
    }
    catch (...)
    {

      return 1;
    }
  }

  // Licenses
  std::vector<std::string> licenses;
  boost::split(licenses, components.at(2), boost::is_any_of("\n"));

  for (const auto& license : licenses)
  {
    if (license.empty())
    {
    
      continue;
    }

    License lic;
    if (lic.Init(license))
    {

      return 1;
    }

    licenses_.push_back(lic);
  }

  // Signature
  signature_ = components.at(3);
  if (signature_.empty())
  {

    return 1;
  }

  return 0;
}

int LicenseKey::Init(const ProductKey& productkey, const boost::gregorian::date& expires, const std::vector<License>& licenses, const utility::PrivateKey& privatekey)
{
  productkey_ = productkey;
  expires_ = expires;
  licenses_ = licenses;

  const std::string message = MessageToString();
  const boost::optional< std::array<unsigned char, 256> > signature = privatekey.Sign(reinterpret_cast<const unsigned char*>(message.data()), message.size());
  if (!signature.is_initialized())
  {

    return 1;
  }

  signature_ = utility::Base64Encode(signature->data(), signature->size());
  if (signature_.empty())
  {

    return 1;
  }

  return 0;
}

void LicenseKey::Destroy()
{
  productkey_.Destroy();
  expires_ = boost::gregorian::date();
  licenses_.clear();
  signature_.clear();
}

std::string LicenseKey::GetExpiryDate() const
{
  if (expires_.is_pos_infinity())
  {

    return std::string("unlimited");
  }
  else if (!expires_.is_not_a_date())
  {

    return boost::gregorian::to_simple_string(expires_);
  }
  else
  {

    return std::string("invalid");
  }
}

boost::optional<unsigned int> LicenseKey::GetLicenseValue(const std::string& license) const
{
  auto i = std::find_if(licenses_.begin(), licenses_.end(), [license](const License& lic) { return (lic.license_ == license); });
  if (i == licenses_.end())
  {

    return boost::none;
  }

  return i->value_;
}

std::string LicenseKey::ToString() const
{
  const std::string licensekey = (MessageToString() + std::string("\n\n") + signature_);
  std::vector<unsigned char> data(licensekey.begin(), licensekey.end());
  const std::vector< std::pair<unsigned int, unsigned int> > sequence = utility::GenerateRandomSequence(static_cast<unsigned int>(data.size()), NUM_OBFUSCATE_ITERATIONS, static_cast<unsigned int>((data.size() - 1) * sizeof(std::string::value_type) * 8));
  for (const auto& pair : sequence)
  {
    utility::SwapBit(data.data(), pair.first, pair.second);

  }

  return utility::Base64Encode(data.data(), data.size());
}

bool LicenseKey::IsExpired() const
{
  if (expires_.is_not_a_date())
  {

    return true;
  }

  if (boost::gregorian::day_clock::local_day() > expires_)
  {

    return true;
  }

  return false;
}

bool LicenseKey::IsValid(const utility::PublicKey& publickey) const
{
  if (IsExpired())
  {

    return false;
  }

  const std::string message = MessageToString();
  const std::vector<unsigned char> signature = utility::Base64Decode(signature_);
  if (signature.size() < 256)
  {

    return false;
  }
  return publickey.Verify(reinterpret_cast<const unsigned char*>(message.data()), message.size(), signature.data());
}

bool LicenseKey::operator==(const LicenseKey& rhs) const
{
  return ((productkey_ == rhs.productkey_) && (expires_ == rhs.expires_) && (licenses_ == rhs.licenses_) && (signature_ == rhs.signature_));
}

std::string LicenseKey::MessageToString() const
{
  std::vector<std::string> licensesstring;
  for (const auto& license : licenses_)
  {
    licensesstring.push_back(license.ToString());

  }

  std::string date;
  if (expires_.is_pos_infinity())
  {
    date = UNLIMITEDEXPIRY;

  }
  else
  {
    date = boost::gregorian::to_simple_string(expires_);

  }

  return productkey_.ToString() + std::string("\n\n") + date + std::string("\n\n") + boost::join(licensesstring, std::string("\n"));
}

}
