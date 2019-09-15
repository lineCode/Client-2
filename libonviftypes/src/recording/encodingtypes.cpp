// encodingtypes.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"


#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

EncodingTypes::EncodingTypes()
{

}

EncodingTypes::EncodingTypes(const std::vector<std::string>& encoding) :
  encoding_(encoding)
{

}

EncodingTypes::EncodingTypes(const std::string& encoding)
{
  boost::algorithm::split(encoding_, encoding, boost::is_any_of("\t\n "),boost::token_compress_on); 
}

std::string EncodingTypes::ToString() const
{
  return boost::join(encoding_, std::string(" "));
}

bool EncodingTypes::operator==(const EncodingTypes& rhs) const
{
  return std::is_permutation(encoding_.begin(), encoding_.end(), rhs.encoding_.begin(), rhs.encoding_.end());
}

}

}
