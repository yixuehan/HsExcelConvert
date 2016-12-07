#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
   Ui::MainWindow *ui;
   HsExcelConvert convert ;

};

#endif // MAINWINDOW_H
