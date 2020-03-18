// layout.cpp
//

///// Includes /////

#include "monocleclient/layout.h"

///// Namespaces /////

namespace client
{

///// Methods /////

LayoutView::LayoutView(const uint64_t token, const int32_t x, const int32_t y, const int32_t width, const int32_t height) :
  token_(token),
  x_(x),
  y_(y),
  width_(width),
  height_(height)
{

}

LayoutWindow::LayoutWindow(const boost::shared_ptr<Device>& device, const uint64_t token, const bool mainwindow, const bool maximised, const int32_t screenx, const int32_t screeny, const int32_t screenwidth, const int32_t screenheight, const int32_t x, const int32_t y, const int32_t width, const int32_t height, const uint32_t gridwidth, const uint32_t gridheight, const std::vector< QSharedPointer<LayoutView> >& maps, const std::vector< QSharedPointer<LayoutView> >& recordings) :
  device_(device),
  token_(token),
  mainwindow_(mainwindow),
  maximised_(maximised),
  screenx_(screenx),
  screeny_(screeny),
  screenwidth_(screenwidth),
  screenheight_(screenheight),
  x_(x),
  y_(y),
  width_(width),
  height_(height),
  gridwidth_(gridwidth),
  gridheight_(gridheight),
  maps_(maps),
  recordings_(recordings)
{

}

Layout::Layout(const boost::shared_ptr<Device>& device, const monocle::LAYOUT& layout) :
  device_(device),
  token_(layout.token_),
  name_(QString::fromStdString(layout.name_)),
  windows_(GetWindows(layout.windows_))
{

}

Layout::~Layout()
{

}

void Layout::SetConfiguration(const monocle::LAYOUT& layout)
{
  token_ = layout.token_;
  name_ = QString::fromStdString(layout.name_);
  windows_ = GetWindows(layout.windows_);
}

std::vector< QSharedPointer<LayoutWindow> > Layout::GetWindows(const std::vector<monocle::LAYOUTWINDOW>& layoutwindows) const
{
  std::vector< QSharedPointer<LayoutWindow> > windows;
  windows.reserve(windows.size());
  for (const monocle::LAYOUTWINDOW& window : layoutwindows)
  {
    std::vector< QSharedPointer<LayoutView> > maps;
    for (const monocle::LAYOUTVIEW& map : window.maps_)
    {
      maps.push_back(QSharedPointer<LayoutView>::create(map.token_, map.x_, map.y_, map.width_, map.height_));

    }

    std::vector< QSharedPointer<LayoutView> > recordings;
    for (const monocle::LAYOUTVIEW& recording : window.recordings_)
    {
      recordings.push_back(QSharedPointer<LayoutView>::create(recording.token_, recording.x_, recording.y_, recording.width_, recording.height_));

    }

    windows.push_back(QSharedPointer<LayoutWindow>::create(device_, window.token_, window.mainwindow_, window.maximised_, window.screenx_, window.screeny_, window.screenwidth_, window.screenheight_, window.x_, window.y_, window.width_, window.height_, window.gridwidth_, window.gridheight_, maps, recordings));
  }
  return windows;
}

}
