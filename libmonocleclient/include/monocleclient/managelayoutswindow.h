// managelayoutswindow.h
//

#ifndef ID2VVC45ASDSDSDSDSDSDDSDAJD9WKDN9G
#define ID2VVC45ASDSDSDSDSDSDDSDAJD9WKDN9G

///// Includes /////

#include "ui_managelayoutswindow.h"

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Layout;

///// Classes /////

class ManageLayoutsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageLayoutsWindow(QWidget* parent);
  ~ManageLayoutsWindow();

 private:

  void AddLayout(const QSharedPointer<client::Layout>& layout);

  Ui::ManageLayoutsWindow ui_;

  std::vector<monocle::client::Connection> removelayoutconnections_;

 private slots:

  void LayoutAdded(const QSharedPointer<client::Layout>& layout);
  void LayoutChanged(const QSharedPointer<client::Layout>& layout);
  void LayoutRemoved(const uint64_t token);
  void on_tablelayouts_itemSelectionChanged();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
