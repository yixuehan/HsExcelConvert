#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_setdialog.h"
#include <QDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   QObject::connect(ui->set, SIGNAL(triggered(bool)),
                    this, SLOT(on_set(bool))) ;
}

MainWindow::~MainWindow()
{
   delete ui;
}



void MainWindow::on_toClient_clicked()
{

}

void MainWindow::on_set(bool bSelect)
{
   ui->set->setEnabled(!bSelect);
   QMessageBox msg ;
   msg.setText(QString::number(bSelect));
   msg.show();
   msg.exec();
   QDialog dialog ;
   Ui::SetDialog setDialog ;
   setDialog.setupUi(&dialog);
   dialog.show() ;
   if ( QDialogButtonBox::Ok == dialog.exec()) {

   }
}
