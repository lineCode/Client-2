// mainwindow.h
//

#ifndef ID0S1Q2OOFBBXUDF678NLLU9WKGR3IPTHW
#define ID0S1Q2OOFBBXUDF678NLLU9WKGR3IPTHW

///// Includes /////

#include <boost/filesystem/path.hpp>
#include <cuda.h>
#include <memory>
#include <QDir>
#include <QMainWindow>
#include <QResource>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <QVector>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>
#include <wsdiscover/wsdiscoverclient.hpp>

#ifdef _WIN32
  #include <WinSock2.h>
#endif

#include "checkforupdate.h"
#include "devicemgr.h"
#include "log.h"
#include "mediamgr.h"
#include "shortcutmgr.h"
#include "ui_mainwindow.h"
#include "videowidgetsmgr.h"
#include "videowindow.h"
#include "videowindowmgr.h"

///// Defines /////

// Message to the log
#define LOG_EVENT(message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_EVENT, __FILE__, __LINE__, __func__, message);
#define LOG_MESSAGE(message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_MESSAGE, __FILE__, __LINE__, __func__, message);
#define LOG_WARNING(message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_WARNING, __FILE__, __LINE__, __func__, message);
#define LOG_FATAL(message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_FATAL, __FILE__, __LINE__, __func__, message);

#define LOG_THREAD_EVENT(message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_EVENT), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(QString, message));
#define LOG_THREAD_MESSAGE(message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_MESSAGE), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(QString, message));
#define LOG_THREAD_WARNING(message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_WARNING), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(QString, message));
#define LOG_THREAD_FATAL(message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_FATAL), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(QString, message));

#define LOG_EVENT_SOURCE(source, message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_EVENT, __FILE__, __LINE__, __func__, source, message);
#define LOG_MESSAGE_SOURCE(source, message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_MESSAGE, __FILE__, __LINE__, __func__, source, message);
#define LOG_WARNING_SOURCE(source, message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_WARNING, __FILE__, __LINE__, __func__, source, message);
#define LOG_FATAL_SOURCE(source, message) MainWindow::Instance()->GetLog().Message(client::LOGLEVEL_FATAL, __FILE__, __LINE__, __func__, source, message);

#define LOG_THREAD_EVENT_SOURCE(source, message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_EVENT), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define LOG_THREAD_MESSAGE_SOURCE(source, message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_MESSAGE), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define LOG_THREAD_WARNING_SOURCE(source, message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_WARNING), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define LOG_THREAD_FATAL_SOURCE(source, message) QMetaObject::invokeMethod(&MainWindow::Instance()->GetLog(), "Message", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_FATAL), Q_ARG(const char*, __FILE__), Q_ARG(int, __LINE__), Q_ARG(const char*, __func__), Q_ARG(boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));

// Message to the gui
#define GUI_EVENT(message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_EVENT, boost::shared_ptr<Device>(), message);
#define GUI_MESSAGE(message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_MESSAGE, boost::shared_ptr<Device>(), message);
#define GUI_WARNING(message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_WARNING, boost::shared_ptr<Device>(), message);
#define GUI_FATAL(message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_FATAL, boost::shared_ptr<Device>(), message);

#define GUI_THREAD_EVENT(message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_EVENT), Q_ARG(const boost::shared_ptr<client::Device>&, boost::shared_ptr<client::Device>()), Q_ARG(QString, message));
#define GUI_THREAD_MESSAGE(message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_MESSAGE), Q_ARG(const boost::shared_ptr<client::Device>&, boost::shared_ptr<client::Device>()), Q_ARG(QString, message));
#define GUI_THREAD_WARNING(message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_WARNING), Q_ARG(const boost::shared_ptr<client::Device>&, boost::shared_ptr<client::Device>()), Q_ARG(QString, message));
#define GUI_THREAD_FATAL(message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_FATAL), Q_ARG(const boost::shared_ptr<client::Device>&, boost::shared_ptr<client::Device>()), Q_ARG(QString, message));

#define GUI_EVENT_SOURCE(source, message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_EVENT, source, message);
#define GUI_MESSAGE_SOURCE(source, message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_MESSAGE, source, message);
#define GUI_WARNING_SOURCE(source, message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_WARNING, source, message);
#define GUI_FATAL_SOURCE(source, message) MainWindow::Instance()->GetLogTable()->Log(client::LOGLEVEL_FATAL, source, message);

#define GUI_THREAD_EVENT_SOURCE(source, message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_EVENT), Q_ARG(const boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define GUI_THREAD_MESSAGE_SOURCE(source, message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_MESSAGE), Q_ARG(const boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define GUI_THREAD_WARNING_SOURCE(source, message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_WARNING), Q_ARG(const boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));
#define GUI_THREAD_FATAL_SOURCE(source, message) QMetaObject::invokeMethod(MainWindow::Instance()->GetLogTable(), "Log", Qt::QueuedConnection, Q_ARG(client::LOGLEVEL, client::LOGLEVEL_FATAL), Q_ARG(const boost::shared_ptr<client::Device>, source), Q_ARG(QString, message));

// Message to the log and the gui
#define LOG_GUI_EVENT(message) LOG_EVENT(message) GUI_EVENT(message)
#define LOG_GUI_MESSAGE(message) LOG_MESSAGE(message) GUI_MESSAGE(message)
#define LOG_GUI_WARNING(message) LOG_WARNING(message) GUI_WARNING(message)
#define LOG_GUI_FATAL(message) LOG_FATAL(message) GUI_FATAL(message)

#define LOG_GUI_THREAD_EVENT(message) LOG_THREAD_EVENT(message) GUI_THREAD_EVENT(message)
#define LOG_GUI_THREAD_MESSAGE(message) LOG_THREAD_MESSAGE(message) GUI_THREAD_MESSAGE(message)
#define LOG_GUI_THREAD_WARNING(message) LOG_THREAD_WARNING(message) GUI_THREAD_WARNING(message)
#define LOG_GUI_THREAD_FATAL(message) LOG_THREAD_FATAL(message) GUI_THREAD_FATAL(message)

#define LOG_GUI_EVENT_SOURCE(source, message) LOG_EVENT_SOURCE(source, message) GUI_EVENT_SOURCE(source, message)
#define LOG_GUI_MESSAGE_SOURCE(source, message) LOG_MESSAGE_SOURCE(source, message) GUI_MESSAGE_SOURCE(source, message)
#define LOG_GUI_WARNING_SOURCE(source, message) LOG_WARNING_SOURCE(source, message) GUI_WARNING_SOURCE(source, message)
#define LOG_GUI_FATAL_SOURCE(source, message) LOG_FATAL_SOURCE(source, message) GUI_FATAL_SOURCE(source, message)

#define LOG_GUI_THREAD_EVENT_SOURCE(source, message) LOG_THREAD_EVENT_SOURCE(source, message) GUI_THREAD_EVENT_SOURCE(source, message)
#define LOG_GUI_THREAD_MESSAGE_SOURCE(source, message) LOG_THREAD_MESSAGE_SOURCE(source, message) GUI_THREAD_MESSAGE_SOURCE(source, message)
#define LOG_GUI_THREAD_WARNING_SOURCE(source, message) LOG_THREAD_WARNING_SOURCE(source, message) GUI_THREAD_WARNING_SOURCE(source, message)
#define LOG_GUI_THREAD_FATAL_SOURCE(source, message) LOG_THREAD_FATAL_SOURCE(source, message) GUI_THREAD_FATAL_SOURCE(source, message)

///// Enumerations /////

enum MOUSESTATE
{
  MOUSESTATE_SELECT,
  MOUSESTATE_FINDMOTION,
  MOUSESTATE_FINDOBJECT,
  MOUSESTATE_COLOURPICKER
};

///// Namespaces /////

namespace client
{

///// Declarations /////

class Layout;

///// Structures /////

struct CUDADEVICE
{
  CUDADEVICE(const CUdevice device, const CUcontext context);

  CUdevice device_;
  CUcontext context_;
};

///// Classes /////

class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:

  static void Create(const uint32_t numioservices, const uint32_t numioservicethreads);
  static void Destroy();
  static MainWindow* Instance() { return instance_; }

  MainWindow(const uint32_t numioservices, const uint32_t numioservicethreads);
  virtual ~MainWindow();

  inline utility::IoServicePool& GetIOServicePool() { return ioservicepool_; }
  inline boost::asio::io_service& GetGUIIOService() { return guiioservice_; }

  inline const QIcon& GetCameraIcon() const { return cameraicon_; }

  void SetDiscoveryHelper(const bool discoveryhelper);

  void ShortMonthName(const int mon, std::vector<char>& buffer) const;
  void ShortWeekDayName(const int mon, std::vector<char>& buffer) const;

  size_t GetNumCUDADevices() const { return cudadevices_.size(); }
  const std::vector<CUDADEVICE>& GetCUDADevices() const { return cudadevices_; }
  CUcontext GetNextCUDAContext();

  inline Log& GetLog() { return log_; }

  const utility::PublicKey& GetPublicKey() const { return publickey_; }

  void UpdateTray();

  void ShowHideDocks();

  QColor GetRandomHSVColour();
  QVector4D GetRandomHSVColour4D();

  const utility::Version& GetVersion() const { return version_; }
  ShortcutMgr& GetShortcutMgr() { return shortcutmgr_; }
  DeviceMgr& GetDeviceMgr() { return devicemgr_; }
  MediaMgr& GetMediaMgr() { return mediamgr_; }
  VideoWidgetsMgr& GetVideoWidgetsMgr() { return videowidgetsmgr_; }
  VideoWindowMgr& GetVideoWindowMgr() { return videowindowmgr_; }

  LogTable* GetLogTable() { return ui_.logtable; }
  VideoWidget* GetVideoWidget() { return ui_.videowidget; }
  QWidget* GetToolbar() { return ui_.widgettoolbar; }
  QMenu* GetMenuWindow() { return ui_.menuwindow; }
  QPushButton* GetButtonPlay() { return ui_.buttonplay; }
  QPushButton* GetButtonPause() { return ui_.buttonpause; }
  QPushButton* GetButtonStop() { return ui_.buttonstop; }
  QPushButton* GetButtonZoomIn() { return ui_.buttonzoomin; }
  QPushButton* GetButtonZoomOut() { return ui_.buttonzoomout; }
  PlaybackWidget* GetPlaybackWidget() { return ui_.playbackwidget; }

  bool IsDeviceTreeFilterHidden();
  void ShowDeviceTreeFilter(const bool show);

  bool IsLocationTreeFilterHidden();
  void ShowLocationTreeFilter(const bool show);

  void ResetMouseState();
  MOUSESTATE GetMouseState() const;

  void SetColourPickerColour(const QVector3D& colour);
  QVector3D GetColourPickerColour() const;

 protected:

  virtual void changeEvent(QEvent* event) override;
  virtual void closeEvent(QCloseEvent* event) override;
  virtual void dragEnterEvent(QDragEnterEvent* event) override;
  virtual void dragMoveEvent(QDragMoveEvent* event) override;
  virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
  virtual void dropEvent(QDropEvent* event) override;
  virtual void timerEvent(QTimerEvent* event) override;

 private:

  static MainWindow* instance_;

  uint32_t GridWidth(const std::vector< QSharedPointer<LayoutWindow> >& layouts);
  uint32_t GridHeight(const std::vector< QSharedPointer<LayoutWindow> >& layouts);
  void AcceptDrop(QDragMoveEvent* event);
  int LoadLanguage(const QString& language);
  void LoadDefaultLanguage();
  bool SelectLanguage(const QString& language);
  void UnselectAllLanguages();
  void ToolbarUpdated();
  void DiscoverCallback(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);
  void SaveNewCameras() const;

  const utility::Version version_;
  const QDir translationsdirectory_;
  const boost::filesystem::path licensepath_;
  const QResource* arial_;
  const QIcon showfullscreen_;
  const QIcon cameraicon_;

  std::random_device rd_;
  mutable std::mt19937 gen_;
  std::uniform_real_distribution<double> hcolordist_;
  std::uniform_real_distribution<double> scolordist_;
  std::uniform_real_distribution<double> vcolordist_;

  Ui::MainWindow ui_;

  std::string shortmonthnames_[12];
  std::string shortweekdaynames_[7];

  std::vector<CUDADEVICE> cudadevices_;

  utility::IoServicePool ioservicepool_;
  boost::asio::io_service guiioservice_;

  Log log_;

  utility::PublicKey publickey_;

  QTranslator* monocletranslator_;
  QTranslator* qttranslator_;
  QString currentlanguage_;

  QSystemTrayIcon* tray_;

  ShortcutMgr shortcutmgr_;
  DeviceMgr devicemgr_;
  MediaMgr mediamgr_;

  VideoWidgetsMgr videowidgetsmgr_;
  VideoWindowMgr videowindowmgr_;

  CheckForUpdate checkforupdate_;

  QVector3D colourpickercolour_;

  boost::shared_ptr<onvif::wsdiscover::WsDiscoverClient> discover_;

  int discoverytimer_;
  int iotimer_;

  std::vector<uint64_t> newdeviceidentifiers_; // A list of devices we have asked the user to add
  std::vector< std::pair<bool, QString> > newcameras_; // A list of cameras we have asked the user to add <Save to disk, address>


 private slots:

  void LanguageChanged(QAction* action);
  void LayoutAdded(const QSharedPointer<Layout>& layout);
  void MapViewCreated(const QSharedPointer<MapView>& mapview);
  void VideoViewCreated(const QSharedPointer<VideoView>& videoview);
  void ViewDestroyed(const QSharedPointer<View>& view);
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void TrayRestore();
  void TrayMaximise();
  void UpdateAvailable(bool updateavailable, const std::vector<UpdateVersion>& versions, const UpdateVersion& latestversion);
  void on_actionnewvideowindow_triggered();
  void on_actionadddevice_triggered();
  void on_actionimportfile_triggered();
  void on_actionoptions_triggered();
  void on_actioncheckforupdate_triggered();
  void on_actionabout_triggered();
  void on_actiondevices_triggered();
  void on_actionlog_triggered();
  void on_actionplayback_triggered();
  void on_actiontoolbar_triggered();
  void on_actionsavecurrentlayout_triggered();
  void on_actionsavecurrentlayoutas_triggered();
  void on_actionfindmotion_toggled();
  void on_actionfindobject_toggled();
  void on_actioncolourpicker_toggled();
  void on_editdevicetreefilter_textChanged(const QString& text);
  void on_editlocationtreefilter_textChanged(const QString& text);

};

}

#endif
