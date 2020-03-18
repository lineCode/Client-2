// managelayoutwindow.h
//

#ifndef ID2VVC49ODFKDJXFXFXFXFXXXS19WKDN9G
#define ID2VVC49ODFKDJXFXFXFXFXXXS19WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
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

  ManageLayoutWindow(QWidget* parent, const boost::optional<uint64_t>& token);
  virtual ~ManageLayoutWindow();

 private:

  const boost::optional<uint64_t> token_;

  Ui::ManageLayoutWindow ui_;

  std::vector<monocle::client::Connection> connections_;

 private slots:

  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
