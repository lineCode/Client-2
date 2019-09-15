// floatattrlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

FloatAttrList::FloatAttrList()
{

}

FloatAttrList::FloatAttrList(const std::vector<float>& items) :
  items_(items)
{
  
}

FloatAttrList::FloatAttrList(const std::string& items)
{
  std::vector<std::string> floats;
  boost::algorithm::split(floats, items, boost::is_any_of("\t\n "),boost::token_compress_on); 

  for (const auto& f : floats)
  {
    try
    {
      items_.push_back(std::stof(f));

    }
    catch (...)
    {


    }
  }
}

std::string FloatAttrList::ToString() const
{
  std::vector<std::string> floats;
  for (const float f : items_)
  {
    floats.push_back(std::to_string(f));

  }
  return boost::join(floats, std::string(" "));
}

bool FloatAttrList::operator==(const FloatAttrList& rhs) const
{
  return std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end());
}

}
