// layout.h
//

#ifndef IDLIY0KUASASDASDSADADAZZZZUCV1EPVI
#define IDLIY0KUASASDASDSADADAZZZZUCV1EPVI

///// Includes /////

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class LayoutView
{
 public:

  LayoutView(const uint64_t token, const int32_t x, const int32_t y, const int32_t width, const int32_t height);

  uint64_t token_; // This is the token of the recording or map, not of this entity
  int32_t x_;
  int32_t y_;
  int32_t width_;
  int32_t height_;

};

class LayoutWindow
{
 public:

  LayoutWindow(const uint64_t token, const bool mainwindow, const bool maximised, const int32_t screenx, const int32_t screeny, const int32_t screenwidth, const int32_t screenheight, const int32_t x, const int32_t y, const int32_t width, const int32_t height, const uint32_t gridwidth, const uint32_t gridheight, const std::vector< QSharedPointer<LayoutView> >& maps, const std::vector< QSharedPointer<LayoutView> >& recordings);

  uint64_t token_;
  bool mainwindow_;
  bool maximised_;
  int32_t screenx_;
  int32_t screeny_;
  int32_t screenwidth_;
  int32_t screenheight_;
  int32_t x_;
  int32_t y_;
  int32_t width_;
  int32_t height_;
  uint32_t gridwidth_;
  uint32_t gridheight_;
  std::vector< QSharedPointer<LayoutView> > maps_;
  std::vector< QSharedPointer<LayoutView> > recordings_;

};

class Layout : public QObject
{
 Q_OBJECT

 public:

  Layout(const uint64_t token, const QString& name, const std::vector< QSharedPointer<LayoutWindow> >& windows);
  ~Layout();

  inline uint64_t GetToken() const { return token_; }
  inline const QString& GetName() const { return name_; }
  inline const std::vector< QSharedPointer<LayoutWindow> >& GetWindows() const { return windows_; }

 private:

  uint64_t token_;
  QString name_;
  std::vector< QSharedPointer<LayoutWindow> > windows_;

};

}

#endif
