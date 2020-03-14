// managelayoutwindow.h
//

#ifndef ID2VVC49ODFKDJXFXFXFXFXXXS19WKDN9G
#define ID2VVC49ODFKDJXFXFXFXFXXXS19WKDN9G

///// Includes /////

#include <monocleprotocol/monocleprotocol.hpp>
#include <QDialog>
#include <random>
#include <vector>

#include "ui_managelayoutwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class ManageLayoutWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageLayoutWindow(QWidget* parent);
  virtual ~ManageLayoutWindow();

 private:

  uint64_t GetWindowToken(const std::vector<monocle::LAYOUTWINDOW>& windows) const;

  Ui::ManageLayoutWindow ui_;

  mutable std::random_device rd_;

  //TODO std::vector<monocle::client::Connection> connections_;

 private slots:

  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
