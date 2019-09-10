// media.cpp
//

///// Includes /////

#include "monocleclient/media.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Media::Media(const QString& path) :
  path_(path),
  progress_(std::make_shared<float>(0.0f)),
  ret_(0),
  timer_(-1)
{

}

Media::~Media()
{
  Destroy();

}

void Media::Init()
{
  Destroy();

  std::lock_guard<std::mutex> lock(mutex_);
  *progress_ = 0.0f;
  retfuture_ = media_.Init(path_.toStdString(), progress_);
  ret_.reset();
  timer_ = startTimer(std::chrono::milliseconds(100));
}

void Media::Destroy()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (timer_ != -1)
  {
    killTimer(timer_);
    timer_ = -1;
  }
  media_.Destroy();
}

int Media::Read(const uint64_t offset, const uint64_t size, char* buffer)
{
  std::lock_guard<std::mutex> lock(mutex_);
  return media_.Read(offset, size, buffer);
}

void Media::timerEvent(QTimerEvent* event)
{
  emit Progress(*progress_);
  if (retfuture_.valid() && (retfuture_.wait_for(std::chrono::seconds::zero()) == std::future_status::ready))
  {
    ret_ = retfuture_.get();
    emit Finished(*ret_);
    killTimer(timer_);
    timer_ = -1;
  }
}

}
