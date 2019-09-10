// manageonvifmediacreateprofilewindow.h
//

#ifndef ID3WCG5VGL3565656565666F08M34KC42
#define ID3WCG5VGL3565656565666F08M34KC42

///// Includes /////

#include <boost/optional.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>

#include "ui_manageonvifmediacreateprofilewindow.h"

///// Declarations /////

namespace onvif
{
namespace media
{
class MediaClient;
}
}

///// Namespaces /////

namespace client
{

///// Classes /////

class ManageONVIFMediaCreateProfileWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaCreateProfileWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaCreateProfileWindow();

  inline const boost::optional<onvif::Profile>& GetProfile() const { return profile_; }

 protected:



 private:

  Ui::ManageONVIFMediaCreateProfileWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection createprofileconnection_;

  boost::optional<onvif::Profile> profile_;


 private slots:
 
  void on_buttonok_clicked();

};

}

#endif
