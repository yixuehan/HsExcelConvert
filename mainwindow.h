#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "ui_setdialog.h"

#include "hsexcelconvert.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private slots:
   void on_toClient_clicked();
   
   void on_set(bool) ;

   void on_newInterface_clicked();

   void on_toResource_clicked();
   
   
   void on_settingList_clicked(const QModelIndex &index);
   
   void on_newInterfaceList_doubleClicked(const QModelIndex &index);
   
   void on_modInterface_clicked();
   
private:
   Ui::MainWindow *ui;
   QDialog setDialog ;
   Ui::SetDialog setUi ;
   HsExcelConvert hsConvert ;
};

#endif // MAINWINDOW_H
