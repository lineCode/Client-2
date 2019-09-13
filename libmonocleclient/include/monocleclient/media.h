// media.h
//

#ifndef IDFMKGW308423804DSKSDFSFSFDSFDC609
#define IDFMKGW308423804DSKSDFSFSFDSFDC609

///// Includes /////

#include <future>
#include <mutex>
#include <QEnableSharedFromThis>
#include <QObject>
#include <QString>
#include <stdint.h>
#include <vector>

#include <file/fileread.hpp>

///// Namespaces /////

namespace client
{

///// Classes /////

class Media : public QObject, public QEnableSharedFromThis<Media>
{
 Q_OBJECT

 public:

  Media(const QString& path);
  ~Media();

  void Init();
  void Destroy();

  int Read(const uint64_t offset, const uint64_t size, uint8_t* buffer);

  const QString& GetPath() const { return path_; }
  const file::FileRead& GetMedia() const { return media_; }
  inline float GetProgress() const { return *progress_; }
  inline const boost::optional<int>& GetRet() const { return ret_; }

 protected:

  virtual void timerEvent(QTimerEvent* event) override;

  
 private:

  QString path_;

  mutable std::mutex mutex_;
  file::FileRead media_;
  std::shared_ptr<float> progress_;
  std::future<int> retfuture_;
  boost::optional<int> ret_;

  int timer_;

 signals:

  void Progress(const float progress);
  void Finished(const int ret);
 
 
 private slots:

 
 
};

}

#endif
