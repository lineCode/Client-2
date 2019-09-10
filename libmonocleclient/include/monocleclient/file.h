// recording.h
//

#ifndef IDLIY0KUASDFDSJFKDSFSMVZZRUCV1EPVI
#define IDLIY0KUASDFDSJFKDSFSMVZZRUCV1EPVI

///// Includes /////

#include <QObject>

///// Declarations /////

namespace monocle
{

enum class FileMonitorState : int8_t;
enum class FileState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Classes /////

class File : public QObject
{
 Q_OBJECT

 public:

  File(const uint64_t token, const QString& path, const QString& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate);
  ~File();

  void StateChanged(const monocle::FileState state);
  void MonitorStateChanged(const monocle::FileMonitorState monitorstate);
  
  inline uint64_t GetToken() const { return token_; }
  inline void SetPath(const QString& path) { path_ = path; }
  inline const QString& GetPath() const { return path_; }
  inline void SetMountPoint(const QString& mountpoint) { mountpoint_ = mountpoint; }
  inline const QString& GetMountPoint() const { return mountpoint_; }
  inline void SetNumChunks(const uint64_t numchunks) { numchunks_ = numchunks; }
  inline uint64_t GetNumChunks() const { return numchunks_; }
  inline void SetChunkSize(const uint64_t chunksize) { chunksize_ = chunksize; }
  inline uint64_t GetChunkSize() const { return chunksize_; }
  inline void SetAutomount(const bool automount) { automount_ = automount; }
  inline bool GetAutomount() const { return automount_; }
  inline monocle::FileState GetState() const { return state_; }
  inline monocle::FileMonitorState GetMonitorState() const { return monitorstate_; }
  
 private:

  uint64_t token_;
  QString path_;
  QString mountpoint_;
  uint64_t numchunks_;
  uint64_t chunksize_;
  bool automount_;
  monocle::FileState state_;
  monocle::FileMonitorState monitorstate_;
 
};

}

#endif
