// mediapropertieswindow.h
//

#ifndef ID2VVC12333DJZ43242423423419WKDN9G
#define ID2VVC12333DJZ43242423423419WKDN9G

///// Includes /////

#include <atomic>
#include <boost/optional.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <thread>

#include "ui_mediapropertieswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Media;

///// Classes /////

class MediaPropertiesWindow : public QDialog
{
 Q_OBJECT

 public:

  MediaPropertiesWindow(QWidget* parent, QSharedPointer<Media>& media, const boost::optional< std::pair<uint64_t, uint64_t> >& selectrecording);
  virtual ~MediaPropertiesWindow();

 private:

  QString Size(size_t numbytes) const;

  Ui::MediaPropertiesWindow ui_;

  QSharedPointer<Media> media_;

  std::thread reader_;
  std::atomic<bool> running_;

 signals:

  void Progress(const unsigned int percent, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t failedreadframes, const uint64_t verifiedframes, const uint64_t failedverifiedframes, const uint64_t unverifiedframes, const size_t size, const qulonglong mintime, const qulonglong maxtime);

 private slots:

  void CurrentItemChanged(QTreeWidgetItem* item, QTreeWidgetItem*);
  void ProgressSlot(const unsigned int percent, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t failedreadframes, const uint64_t verifiedframes, const uint64_t failedverifiedframes, const uint64_t unverifiedframes, const size_t size, const qulonglong mintime, const qulonglong maxtime);

};

}

QT_END_NAMESPACE

#endif
