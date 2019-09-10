// options.h
//

#ifndef ID4BB1V4R6E4JCYX92KZFMJMK1MZIKOHBJ
#define ID4BB1V4R6E4JCYX92KZFMJMK1MZIKOHBJ

///// Includes /////

#include <QString>
#include <vector>

#include "singleton.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class Options : public Singleton<Options>
{
  Q_OBJECT

public:

  Options();
  virtual ~Options();

  void Load();
  void Save();

  inline void SetDefaultVideoWindowWidth(const unsigned int defaultvideowindowwidth) { defaultvideowindowwidth_ = defaultvideowindowwidth; }
  inline unsigned int GetDefaultVideoWindowWidth() const { return defaultvideowindowwidth_; }

  inline void SetDefaultVideoWindowHeight(const unsigned int defaultvideowindowheight) { defaultvideowindowheight_ = defaultvideowindowheight; }
  inline unsigned int GetDefaultVideoWindowHeight() const { return defaultvideowindowheight_; }

  inline void SetUpdateFrequency(const unsigned int updatefrequency) { updatefrequency_ = updatefrequency; }
  inline unsigned int GetUpdateFrequency() const { return updatefrequency_; }
  
  inline void SetDefaultShowToolbar(const bool defaultshowtoolbar) { defaultshowtoolbar_ = defaultshowtoolbar; }
  inline bool GetDefaultShowToolbar() const { return defaultshowtoolbar_; }

  inline void SetCheckForUpdateOnStartup(const bool checkforupdateonstartup) { checkforupdateonstartup_ = checkforupdateonstartup; }
  inline bool GetCheckForUpdateOnStartup() const { return checkforupdateonstartup_; }

  inline void SetShowTokens(const bool showtokens) { showtokens_ = showtokens; }
  inline bool GetShowTokens() const { return showtokens_; }

  inline void SetHideMediaCloseDialog(const bool hidemediaclosedialog) { hidemediaclosedialog_ = hidemediaclosedialog; }
  inline bool GetHideMediaCloseDialog() const { return hidemediaclosedialog_; }

  inline void SetHideVideoWindowCloseDialog(const bool hidevideowindowclosedialog) { hidevideowindowclosedialog_ = hidevideowindowclosedialog; }
  inline bool GetHideVideoWindowCloseDialog() const { return hidevideowindowclosedialog_; }

  inline void SetHideMainWindowCloseDialog(const bool hidemainwindowclosedialog) { hidemainwindowclosedialog_ = hidemainwindowclosedialog; }
  inline bool GetHideMainWindowCloseDialog() const { return hidemainwindowclosedialog_; }

  inline void SetInfoTextFormat(const std::string& infotextformat) { infotextformat_ = infotextformat; }
  inline const std::string& GetInfoTextFormat() const { return infotextformat_; }

  inline void SetShowInfo(const bool showinfo) { showinfo_ = showinfo; }
  inline bool GetShowInfo() const { return showinfo_; }

  inline void SetStretchVideo(const bool stretchvideo) { stretchvideo_ = stretchvideo; }
  inline bool GetStretchVideo() const { return stretchvideo_; }

  inline void SetMaxCUDADecodersPerDevice(const unsigned int maxcudadecodersperdevice) { maxcudadecodersperdevice_ = maxcudadecodersperdevice; }
  inline unsigned int GetMaxCUDADecodersPerDevice() const { return maxcudadecodersperdevice_; }

  inline void SetMinimiseToTray(const bool minimisetotray) { minimisetotray_ = minimisetotray; }
  inline bool GetMinimiseToTray() const { return minimisetotray_; }

  inline void SetAlwaysShowTray(const bool alwaysshowtray) { alwaysshowtray_ = alwaysshowtray; }
  inline bool GetAlwaysShowTray() const { return alwaysshowtray_; }

  inline void SetBackupDirectory(const QString& backupdirectory) { backupdirectory_ = backupdirectory; }
  inline const QString& GetBackupDirectory() const { return backupdirectory_; }

  inline void SetExportDirectory(const QString& exportdirectory) { exportdirectory_ = exportdirectory; }
  inline const QString& GetExportDirectory() const { return exportdirectory_; }

 private:

  unsigned int defaultvideowindowwidth_;
  unsigned int defaultvideowindowheight_;
  unsigned int updatefrequency_;
  bool defaultshowtoolbar_;
  bool checkforupdateonstartup_;
  bool showtokens_;
  bool hidemediaclosedialog_;
  bool hidevideowindowclosedialog_;
  bool hidemainwindowclosedialog_;

  std::string infotextformat_;
  bool showinfo_;
  bool stretchvideo_;
  unsigned int maxcudadecodersperdevice_;

  bool minimisetotray_;
  bool alwaysshowtray_;

  QString backupdirectory_;
  QString exportdirectory_;

};

}

#endif
