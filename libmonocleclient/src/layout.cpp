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

Layout::Layout(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& name, const std::vector< QSharedPointer<LayoutWindow> >& windows) :
  device_(device),
  token_(token),
  name_(name),
  windows_(windows)
{

}

Layout::~Layout()
{

}

}
