// videowidgetsmgr.h
// This class does not own any of the pointers, it just a place to keep them all together

#ifndef IDCYOYQZU08ZS2PX3DXDCZLD3GQ2UM2LEZ
#define IDCYOYQZU08ZS2PX3DXDCZLD3GQ2UM2LEZ

///// Includes /////

#include <QObject>
#include <QPoint>
#include <QResource>
#include <vector>

#include "videowidget.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Map;
class Media;
class View;

///// Classes /////

class VideoWidgetsMgr : public QObject
{
 Q_OBJECT

 public:

  VideoWidgetsMgr(const QResource* arial, const QIcon& showfullscreen);
  ~VideoWidgetsMgr();

  void Init();

  void CreateMapView(const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map);
  void CreateMediaView(const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex);
  void CreateVideoView(const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track);
  void CreateVideoChartView(const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& tracks);

  const std::vector<VideoWidget*>& GetVideoWidgets() const { return videowidgets_; }

  uint64_t GetNumViews(const std::vector<VIEWTYPE>& viewtypes) const;

  void ResetViews();

  std::vector< QSharedPointer<View> > GetViews(); // Returns all video views from all video widgets

  void SetUpdateFrequency(const int frequency);

  inline const QWeakPointer<View>& GetSelectionView() const { return selectionview_; }
  inline const QPoint& GetSelectionViewRect() const { return selectionviewrect_; }

  QSharedPointer<View> GetLastSelectedView() const;

  void VideoWidgetCreated(VideoWidget* videowidget);
  void VideoWidgetDestroyed(VideoWidget* videowidget);

  void MouseEnterEvent(QEvent* event);
  void MouseLeaveEvent(QEvent* event);
  void MouseMoveEvent(QMouseEvent* event);
  void MousePressEvent(QMouseEvent* event);
  void MouseReleaseEvent(QMouseEvent* event);

 private:

  QSharedPointer<View> GetLastSelection() const; // Work backwards through the selection stack, search for an existing view
  void Swap(QSharedPointer<View>& view1, QSharedPointer<View>& view2);
  bool MoveView(VideoWidget* videowidget, QSharedPointer<View>& view, QPoint location);
  std::vector< QSharedPointer<View> > GetSelectedViews() const;
  VideoWidget* GetVideoWidget(const QPoint& globalpos); // Gets the top level VideoWidget at this global location
  boost::optional<QPoint> GetEmptyVideoLocation(VideoWidget* videowidget, unsigned int width, unsigned int height); // Find a width x height element where no video view exists
  void Select(QSharedPointer<View>& view, bool select); // We must use this internally to select items, because it managed the selectedstack_ member
  void UnselectAll();
  void ColourPicker(const QSharedPointer<View>& view, const QPoint& pos);

  const QResource* arial_;
  const QIcon showfullscreen_;

  std::vector<VideoWidget*> videowidgets_; // First element should always be the main windows video widget, and should never be removed

  QWeakPointer<View> selectionview_; // This is the view that has been last selected by a mousepress down event(but not yet selected)
  QPoint selectionviewrect_; // The rect we are currently selecting
  QPoint selectionpoint_; // The relative point in the selectedview that was pressed
  std::vector< QWeakPointer<View> > selectedstack_; // Represents the order of selected items(most recent at top)

 signals:

  void MapViewCreated(const QSharedPointer<MapView>& mapview);
  void MediaViewCreated(const QSharedPointer<MediaView>& mediaview);
  void VideoViewCreated(const QSharedPointer<VideoView>& videoview);
  void VideoChartViewCreated(const QSharedPointer<VideoChartView>& videochartview);
  void ViewDestroyed(const QSharedPointer<View>& view);
  void Selected(QSharedPointer<View>& view, bool select);

};

}

#endif
