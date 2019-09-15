// paritybitlist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

ParityBitList::ParityBitList()
{

}

ParityBitList::ParityBitList(const std::vector<PARITYBIT>& items) :
  items_(items)
{

}

ParityBitList::ParityBitList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Items")
    {
      const boost::optional<PARITYBIT> item = GetParityBit(element.text().get());
      if (item.is_initialized())
      {
        items_.push_back(*item);

      }
    }
  }
}

std::string ParityBitList::ToXml(const std::string& name) const
{
  std::string items;
  for (const auto& item : items_)
  {
    items += onvif::ToXmlEnum("Items", item);

  }

  return ("<" + name + ">" + items + "</" + name + ">");
}

bool ParityBitList::operator==(const ParityBitList& rhs) const
{
  return (std::is_permutation(items_.begin(), items_.end(), rhs.items_.begin(), rhs.items_.end()));
}

}

}
