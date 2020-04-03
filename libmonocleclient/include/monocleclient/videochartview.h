// videochartview.h
// 

#ifndef IDNUONLE5ZXCZZXCKLZJCKZXCXDSDS3TFS
#define IDNUONLE5ZXCZZXCKLZJCKZXCXDSDS3TFS

///// Includes /////

#include <boost/asio.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QGridLayout>
#include <QtCharts>
#include <QWidget>
#include <vector>

#include "view.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Recording;

///// Classes /////

class VideoChartView : public View
{
 Q_OBJECT

 public:

  VideoChartView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const boost::shared_ptr<client::Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& tracks, const QResource* arial);
  ~VideoChartView();

  virtual VIEWTYPE GetViewType() const override { return VIEWTYPE_MONOCLECHART; }

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

 protected:

  virtual void timerEvent(QTimerEvent* event) override;

 private:

  static void ControlStreamEnd(const uint64_t streamtoken, const uint64_t requestindex, const monocle::ErrorCode error, void* callbackdata);
  static void ControlStreamResult(const uint64_t streamtoken, const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results, void* callbackdata);

  void SendImage();
  void CloseConnections();

  boost::shared_ptr<client::Device> device_;
  QSharedPointer<Recording> recording_;
  std::vector< QSharedPointer<RecordingTrack> > tracks_;

  QWidget* widget_;
  QGridLayout* layout_;
  QChartView chart_;

  std::vector<monocle::client::Connection> streamsconnections_;

 private slots:

};

}

#endif
