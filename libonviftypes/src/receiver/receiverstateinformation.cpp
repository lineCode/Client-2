// receiverstateinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Classes /////

ReceiverStateInformation::ReceiverStateInformation()
{

}

ReceiverStateInformation::ReceiverStateInformation(const boost::optional<RECEIVERSTATE>& state, const boost::optional<bool>& autocreated) :
  state_(state),
  autocreated_(autocreated)
{

}

ReceiverStateInformation::ReceiverStateInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "State")
    {
      state_ = GetReceiverState(element.text().get());

    }
    else if (name == "AutoCreated")
    {
      autocreated_ = element.text().get();

    }
  }
}

std::string ReceiverStateInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:State", state_) + onvif::ToXml("tt:AutoCreated", autocreated_) + "</"+name+">");
}

bool ReceiverStateInformation::operator==(const ReceiverStateInformation& rhs) const
{
  return ((state_ == rhs.state_) && (autocreated_ == rhs.autocreated_));
}

}

}
