// ptzspaces.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZSpaces::PTZSpaces()
{

}

PTZSpaces::PTZSpaces(const std::vector<Space2dDescription>& absolutepantiltpositionspace, const std::vector<Space1dDescription>& absolutezoompositionspace, const std::vector<Space2dDescription>& relativepantilttranslationspace, const std::vector<Space1dDescription>& relativezoomtranslationspace, const std::vector<Space2dDescription>& continuouspantiltvelocityspace, const std::vector<Space1dDescription>& continuouszoomvelocityspace, const std::vector<Space1dDescription>& pantiltspeedspace, const std::vector<Space1dDescription>& zoomspeedspace) :
  absolutepantiltpositionspace_(absolutepantiltpositionspace),
  absolutezoompositionspace_(absolutezoompositionspace),
  relativepantilttranslationspace_(relativepantilttranslationspace),
  relativezoomtranslationspace_(relativezoomtranslationspace),
  continuouspantiltvelocityspace_(continuouspantiltvelocityspace),
  continuouszoomvelocityspace_(continuouszoomvelocityspace),
  pantiltspeedspace_(pantiltspeedspace),
  zoomspeedspace_(zoomspeedspace)
{

}

PTZSpaces::PTZSpaces(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AbsolutePanTiltPositionSpace")
    {
      absolutepantiltpositionspace_.push_back(Space2dDescription(element));

    }
    else if (name == "AbsoluteZoomPositionSpace")
    {
      absolutezoompositionspace_.push_back(Space1dDescription(element));

    }
    else if (name == "RelativePanTiltTranslationSpace")
    {
      relativepantilttranslationspace_.push_back(Space2dDescription(element));

    }
    else if (name == "RelativeZoomTranslationSpace")
    {
      relativezoomtranslationspace_.push_back(Space1dDescription(element));

    }
    else if (name == "ContinuousPanTiltVelocitySpace")
    {
      continuouspantiltvelocityspace_.push_back(Space2dDescription(element));

    }
    else if (name == "ContinuousZoomVelocitySpace")
    {
      continuouszoomvelocityspace_.push_back(Space1dDescription(element));

    }
    else if (name == "PanTiltSpeedSpace")
    {
      pantiltspeedspace_.push_back(Space1dDescription(element));

    }
    else if (name == "ZoomSpeedSpace")
    {
      zoomspeedspace_.push_back(Space1dDescription(element));

    }
  }
}

std::string PTZSpaces::ToXml(const std::string& name) const
{
  std::string absolutepantiltpositionspaces;
  for (const auto& absolutepantiltpositionspace : absolutepantiltpositionspace_)
  {
    absolutepantiltpositionspaces += absolutepantiltpositionspace.ToXml("tt:AbsolutePanTiltPositionSpace");

  }

  std::string absolutezoompositionspaces;
  for (const auto& absolutezoompositionspace : absolutezoompositionspace_)
  {
    absolutezoompositionspaces += absolutezoompositionspace.ToXml("tt:AbsoluteZoomPositionSpace");

  }

  std::string relativepantilttranslationspaces;
  for (const auto& relativepantilttranslationspace : relativepantilttranslationspace_)
  {
    relativepantilttranslationspaces += relativepantilttranslationspace.ToXml("tt:RelativePanTiltTranslationSpace");

  }

  std::string relativezoomtranslationspaces;
  for (const auto& relativezoomtranslationspace : relativezoomtranslationspace_)
  {
    relativepantilttranslationspaces += relativezoomtranslationspace.ToXml("tt:RelativeZoomTranslationSpace");

  }

  std::string continuouspantiltvelocityspaces;
  for (const auto& continuouspantiltvelocityspace : continuouspantiltvelocityspace_)
  {
    continuouspantiltvelocityspaces += continuouspantiltvelocityspace.ToXml("tt:ContinuousPanTiltVelocitySpace");

  }

  std::string continuouszoomvelocityspaces;
  for (const auto& continuouszoomvelocityspace : continuouszoomvelocityspace_)
  {
    continuouszoomvelocityspaces += continuouszoomvelocityspace.ToXml("tt:ContinuousZoomVelocitySpace");

  }

  std::string pantiltspeedspaces;
  for (const auto& pantiltspeedspace : pantiltspeedspace_)
  {
    pantiltspeedspaces += pantiltspeedspace.ToXml("tt:PanTiltSpeedSpace");

  }

  std::string zoomspeedspaces;
  for (const auto& zoomspeedspace : zoomspeedspace_)
  {
    zoomspeedspaces += zoomspeedspace.ToXml("tt:ZoomSpeedSpace");

  }

  return ("<"+name+">" + absolutepantiltpositionspaces + absolutezoompositionspaces + relativepantilttranslationspaces + relativezoomtranslationspaces + continuouspantiltvelocityspaces + continuouszoomvelocityspaces + pantiltspeedspaces + zoomspeedspaces + "</"+name+">");
}

bool PTZSpaces::operator==(const PTZSpaces& rhs) const
{
  return (std::is_permutation(absolutepantiltpositionspace_.begin(), absolutepantiltpositionspace_.end(), rhs.absolutepantiltpositionspace_.begin(), rhs.absolutepantiltpositionspace_.end()) && std::is_permutation(absolutezoompositionspace_.begin(), absolutezoompositionspace_.end(), rhs.absolutezoompositionspace_.begin(), rhs.absolutezoompositionspace_.end()) && std::is_permutation(relativepantilttranslationspace_.begin(), relativepantilttranslationspace_.end(), rhs.relativepantilttranslationspace_.begin(), rhs.relativepantilttranslationspace_.end()) && std::is_permutation(continuouspantiltvelocityspace_.begin(), continuouspantiltvelocityspace_.end(), rhs.continuouspantiltvelocityspace_.begin(), rhs.continuouspantiltvelocityspace_.end()) && std::is_permutation(continuouszoomvelocityspace_.begin(), continuouszoomvelocityspace_.end(), rhs.continuouszoomvelocityspace_.begin(), rhs.continuouszoomvelocityspace_.end()) && std::is_permutation(pantiltspeedspace_.begin(), pantiltspeedspace_.end(), rhs.pantiltspeedspace_.begin(), rhs.pantiltspeedspace_.end()) && std::is_permutation(zoomspeedspace_.begin(), zoomspeedspace_.end(), rhs.zoomspeedspace_.begin(), rhs.zoomspeedspace_.end()));
}

}
