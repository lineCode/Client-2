// relayoutputoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

RelayOutputOptions::RelayOutputOptions()
{

}

RelayOutputOptions::RelayOutputOptions(const boost::optional<std::string>& token, const std::vector<RELAYMODE>& mode, const std::vector<int>& delaytimes, const boost::optional<bool>& discrete) :
  token_(token),
  mode_(mode),
  delaytimes_(delaytimes),
  discrete_(discrete)
{

}

RelayOutputOptions::RelayOutputOptions(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<RELAYMODE> relaymode = GetRelayMode(element.text().get());
      if (relaymode.is_initialized())
      {
        mode_.push_back(*relaymode);

      }
    }
    else if (name == "DelayTimes")
    {
      std::vector<std::string> durations;
      const std::string text(element.text().get());
      boost::split(durations, text, boost::is_any_of(" \r\n\t"), boost::algorithm::token_compress_on);
      for (const auto& duration : durations)
      {
        if (duration.empty())
        {

          continue;
        }

        try
        {
          delaytimes_.push_back(std::stoi(duration));

        }
        catch (...)
        {

        }
      }
    }
    else if (name == "Discrete")
    {
      discrete_ = element.text().as_bool();

    }
  }
}

std::string RelayOutputOptions::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("Mode", mode);

  }

  std::vector<std::string> delaytimes;
  for (const auto& delaytime : delaytimes_)
  {
    delaytimes.push_back(std::to_string(delaytime));

  }

  return ("<" + name + onvif::ToXmlAttribute("token", token_) + ">" + modes + onvif::ToXml("DelayTimes", delaytimes) + onvif::ToXml("Discrete", discrete_) + "</" + name + ">");
}

bool RelayOutputOptions::operator==(const RelayOutputOptions& rhs) const
{
  return ((token_ == rhs.token_) && std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && std::is_permutation(delaytimes_.begin(), delaytimes_.end(), rhs.delaytimes_.begin(), rhs.delaytimes_.end()) && (discrete_ == rhs.discrete_));
}

}

}
