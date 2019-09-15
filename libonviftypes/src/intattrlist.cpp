// intattrlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IntAttrList::IntAttrList()
{

}

IntAttrList::IntAttrList(const std::vector<int>& items) :
  items_(items)
{
  
}

IntAttrList::IntAttrList(const std::string& items)
{
  std::vector<std::string> tmp;
  boost::algorithm::split(tmp, items, boost::is_any_of("\t\n "), boost::token_compress_on); 
  for (const std::string& t : tmp)
  {
    try
    {
      items_.push_back(boost::lexical_cast<int>(t));
      
    }
    catch (...)
    {
      
    }
  }
}

std::string IntAttrList::ToString() const
{
  std::vector<std::string> tmp;
  tmp.reserve(items_.size());
  for (const int item : items_)
  {
    tmp.push_back(std::to_string(item));
    
  }
  return boost::join(tmp, std::string(" "));
}

bool IntAttrList::operator==(const IntAttrList& rhs) const
{
  return std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end());
}

}
