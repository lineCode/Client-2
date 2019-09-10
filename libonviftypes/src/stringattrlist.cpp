// stringattrlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

StringAttrList::StringAttrList()
{

}

StringAttrList::StringAttrList(const std::vector<std::string>& items) :
  items_(items)
{
  
}

StringAttrList::StringAttrList(const std::string& items)
{
  boost::algorithm::split(items_, items, boost::is_any_of("\t\n "),boost::token_compress_on); 

}

std::string StringAttrList::ToString() const
{
  return boost::join(items_, std::string(" "));
}

bool StringAttrList::operator==(const StringAttrList& rhs) const
{
  return std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end());
}

}
