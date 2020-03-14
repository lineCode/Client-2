// mapview.h
// 

#ifndef IDNUONLE54656556566666DFSFDSDS3TFS
#define IDNUONLE54656556566666DFSFDSDS3TFS

///// Includes /////

#include "view.h"

#include <boost/shared_ptr.hpp>
#include <QNetworkAccessManager>
#include <QNetworkReply>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Map;

///// Classes /////

class MapView : public View
{
 Q_OBJECT

 public:

  MapView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool stretch, const boost::shared_ptr<client::Device>& device, const QSharedPointer<Map>& map, const QResource* arial);
  ~MapView();

  virtual VIEWTYPE GetViewType() const override { return VIEWTYPE_MAP; }

  virtual void GetMenu(QMenu& parent) override;

  virtual double GetAspectRatio() const override;

  virtual bool GetImage(ImageBuffer& imagebuffer) override;

  virtual int64_t GetTimeOffset() const override { return 0; }

  virtual void FrameStep(const bool forwards) override;
  virtual void Play(const uint64_t time, const boost::optional<uint64_t>& numframes) override;
  virtual void Pause(const boost::optional<uint64_t>& time) override;
  virtual void Stop() override;
  virtual void Scrub(const uint64_t time) override;

  inline const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  inline const QSharedPointer<Map>& GetMap() const { return map_; }

 private:

  void SendMap(const QByteArray& data);
  void RequestMap();
  QString GetMapFilename() const;

  boost::shared_ptr<client::Device> device_;
  QSharedPointer<Map> map_;

  QNetworkAccessManager* networkaccessmanager_;
  QNetworkReply* reply_;

 private slots:

  void AuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);
  void HTTPError(QNetworkReply::NetworkError code);
  void HTTPFinished();
  void HTTPDownloadProgress(qint64 received, qint64 total);
  void MapChanged(const QSharedPointer<Map>& map);

};

}

#endif
