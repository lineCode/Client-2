// managerecordingtrackswindow.h
//

#ifndef ID2VVC23424324324324KDHLAJD9WKDN9G
#define ID2VVC23424324324324KDHLAJD9WKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managerecordingtrackswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Recording;
class RecordingTrack;

///// Classes /////

class ManageRecordingTracksWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingTracksWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording);
  ~ManageRecordingTracksWindow();

 private:

  void AddTrack(const QSharedPointer<client::RecordingTrack>& track);

  Ui::ManageRecordingTracksWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;

  monocle::client::Connection removetrackconnection_;

 private slots:

  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void on_tabletracks_itemSelectionChanged();
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
