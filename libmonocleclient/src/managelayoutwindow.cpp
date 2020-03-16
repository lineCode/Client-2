// managelayoutwindow.cpp
//

///// Includes /////

#include "monocleclient/managelayoutwindow.h"

#include <QMessageBox>
#include <QScreen>
#include <random>
#include <utility/utility.hpp>

#include "monocleclient/mainwindow.h"
#include "monocleclient/map.h"
#include "monocleclient/mapview.h"
#include "monocleclient/recording.h"
#include "monocleclient/videowidgetsmgr.h"
#include "monocleclient/videoview.h"
#include "monocleclient/view.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageLayoutWindow::ManageLayoutWindow(QWidget* parent) :
  QDialog(parent)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

}
//TODO remove the checkbox for load on startup
ManageLayoutWindow::~ManageLayoutWindow()
{
  for (monocle::client::Connection& connection : connections_)
  {
    connection.Close();

  }
  connections_.clear();
}

void ManageLayoutWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please fill in a name for the layout"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<uint64_t>::max());

  std::vector< std::pair<uint64_t, VideoWidget*> > videowidgets;

  const uint64_t token = MainWindow::Instance()->GetDeviceMgr().GetUniqueLayoutToken();
  std::vector< std::pair< boost::shared_ptr<Device>, monocle::LAYOUT> > layouts;

  // We want to gather up all the views from all the windows and dispatch them to the relevant devices, but then have the ability to piece it back together once we later select this layout
  for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    std::vector<monocle::LAYOUTWINDOW> windows;
    windows.reserve(MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets().size());
    for (VideoWidget* videowidget : MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets())
    {
      QWidget* window = static_cast<QWidget*>(videowidget->parent()->parent());

      std::vector<monocle::LAYOUTVIEW> videoviews;
      std::vector<monocle::LAYOUTVIEW> mapviews;
      for (const QSharedPointer<View>& view : videowidget->GetViews())
      {
        if (view->GetViewType() == VIEWTYPE_MONOCLE)
        {
          const QSharedPointer<VideoView> videoview = (view.staticCast<VideoView>());
          if (videoview->GetDevice() != device) // Only collecting for this particular device
          {

            continue;
          }
          const QRect rect = view->GetRect();
          videoviews.push_back(monocle::LAYOUTVIEW(videoview->GetRecording()->GetToken(), rect.x(), rect.y(), rect.width(), rect.height()));
        }
        else if (view->GetViewType() == VIEWTYPE_MAP)
        {
          const QSharedPointer<MapView> mapview = (view.staticCast<MapView>());
          if (mapview->GetDevice() != device) // Only collecting for this particular device
          {

            continue;
          }
          const QRect rect = view->GetRect();
          mapviews.push_back(monocle::LAYOUTVIEW(mapview->GetMap()->GetToken(), rect.x(), rect.y(), rect.width(), rect.height()));
        }
      }

      if (videoviews.size() || mapviews.size())
      {
        // Make sure the screen token is the same for every window on every device
        uint64_t windowtoken = 0;
        auto s = std::find_if(videowidgets.cbegin(), videowidgets.cend(), [videowidget](const std::pair<uint64_t, VideoWidget*>& vw) { return (vw.second == videowidget); });
        if (s == videowidgets.cend())
        {
          while (true)
          {
            windowtoken = dist(gen);
            if (std::find_if(windows.cbegin(), windows.cend(), [windowtoken](const monocle::LAYOUTWINDOW& window) { return (window.token_ == windowtoken); }) == windows.cend())
            {
              videowidgets.push_back(std::make_pair(windowtoken, videowidget));
              break;
            }
          }
        }
        else
        {
          windowtoken = s->first;

        }

        const QRect geometry = window->screen()->geometry();
        windows.push_back(monocle::LAYOUTWINDOW(windowtoken, videowidget == MainWindow::Instance()->GetVideoWidget(), window->isMaximized(), geometry.x(), geometry.y(), geometry.width(), geometry.height(), window->x(), window->y(), window->width(), window->height(), videowidget->GetWidth(), videowidget->GetHeight(), mapviews, videoviews));
      }
    }

    if (windows.size()) // If this device contains contains some views
    {
      layouts.push_back(std::make_pair(device, monocle::LAYOUT(token, ui_.editname->text().toStdString(), windows)));

    }
  }

  if (layouts.empty())
  {
    //TODO error message?
    return;
  }

  for (const std::pair< boost::shared_ptr<Device>, monocle::LAYOUT>& layout : layouts)
  {
    connections_.push_back(layout.first->AddLayout(layout.second, [](const std::chrono::steady_clock::duration latency, const monocle::client::ADDLAYOUTRESPONSE& addlayoutresponse)
    {
      if (addlayoutresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        //TODO accumulate a failure here...
        return;
      }

      //TODO accumulate all the responses here
        //TODO if any of them fail, then delete the ones that didn't fail... or at least attempt too

    }));
  }
}

}

QT_END_NAMESPACE
