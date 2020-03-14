// managelayoutwindow.cpp
//

///// Includes /////

#include "monocleclient/managelayoutwindow.h"

#include <QMessageBox>
#include <QScreen>
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

ManageLayoutWindow::~ManageLayoutWindow()
{

}

uint64_t ManageLayoutWindow::GetWindowToken(const std::vector<monocle::LAYOUTWINDOW>& windows) const
{
  std::mt19937 gen(rd_());
  std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<uint64_t>::max());

  while (true)
  {
    const uint64_t token = dist(gen);
    if (std::find_if(windows.cbegin(), windows.cend(), [token](const monocle::LAYOUTWINDOW& window) { return (window.token_ == token); }) == windows.cend())
    {

      return token;
    }
  }
}

void ManageLayoutWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please fill in a name for the layout"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const uint64_t token = MainWindow::Instance()->GetDeviceMgr().GetUniqueLayoutToken();
  std::vector< std::pair< boost::shared_ptr<Device>, monocle::LAYOUT> > layouts;

  for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    std::vector<monocle::LAYOUTWINDOW> windows;
    windows.reserve(MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets().size());
    for (VideoWidget* videowidget : MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets())
    {
      QWidget* window = static_cast<QWidget*>(videowidget->parent()->parent());
      QScreen* screen = window->screen();

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
        const QRect geometry = screen->geometry();
        windows.push_back(monocle::LAYOUTWINDOW(GetWindowToken(windows), geometry.x(), geometry.y(), geometry.width(), geometry.height(), window->x(), window->y(), window->width(), window->height(), videowidget->GetWidth(), videowidget->GetHeight(), mapviews, videoviews));
      }
    }

    if (windows.size()) // If this device contains contains some views
    {
      layouts.push_back(std::make_pair(device, monocle::LAYOUT(token, ui_.editname->text().toStdString(), windows)));

    }
  }

  for (const std::pair< boost::shared_ptr<Device>, monocle::LAYOUT>& layout : layouts)
  {


    //TODO send away, shared_ptr the result until we have all of them, then disappear or error message...


  }


}

}

QT_END_NAMESPACE
