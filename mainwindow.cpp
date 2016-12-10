#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QColor>
#include <QtXlsx/QtXlsx>
#include <QFileInfo>
//#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   setUi.setupUi(&setDialog);
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

void MainWindow::on_set(bool)
{
   if ( QDialogButtonBox::Ok == setDialog.exec()) {
      hsConvert.setUserName(setUi.userNameEdit->text().toStdString());
      hsConvert.setModifyNo(setUi.modifyNoEdit->text().toStdString());
   }
}

void MainWindow::on_newInterface_clicked()
{
    QFileDialog fileDialog ;
    fileDialog.setDirectory(QString::fromLocal8Bit("E:/恒生电子/svn_new/trunk/Sources/DevCodes/领投宝/业务逻辑"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.exec() ;
    QFileInfo file ; 
    for ( auto pathName : fileDialog.selectedFiles() ) {
       file.setFile(pathName) ;
       auto fileName = file.fileName();
       
       if ( ui->newInterfaceList->findItems(fileName, Qt::MatchExactly).count()
            == 0 ) {
          ui->newInterfaceList->addItem(fileName);
          hsConvert.addNewInterfaceFile(fileName.toStdString()) ;
       }
    }
}

void MainWindow::on_toResource_clicked()
{ 
    hsConvert.convert() ;
}

void MainWindow::on_settingList_clicked(const QModelIndex &index)
{
    QMessageBox msg ;
    msg.exec() ;
}

void MainWindow::on_newInterfaceList_doubleClicked(const QModelIndex &index)
{
   QMessageBox msg ;
   msg.setText(ui->newInterfaceList->currentItem()->text());
   msg.exec() ;
}

void MainWindow::on_modInterface_clicked()
{
   QFileDialog fileDialog ;
   fileDialog.setDirectory(QString::fromLocal8Bit("E:/恒生电子/svn_new/trunk/Sources/DevCodes/领投宝/业务逻辑"));
   fileDialog.setFileMode(QFileDialog::ExistingFiles);
   fileDialog.exec() ;
   QFileInfo file ; 
   for ( auto pathName : fileDialog.selectedFiles() ) {
      file.setFile(pathName) ;
      auto fileName = file.fileName();
      auto baseName = file.baseName();
      bool bFind = false ;
      for ( int i = 0; i < ui->modInterfaceTab->count(); ++i ) {
         if ( ui->modInterfaceTab->widget(i)->accessibleName() == baseName ) {
            bFind = true ;
         }
      }
      if ( !bFind ) {
         auto index = ui->modInterfaceTab->addTab(new QWidget(), baseName) ;
         ui->modInterfaceTab->widget(index)->setAccessibleName(baseName);
         hsConvert.addNewInterfaceFile( fileName.toLocal8Bit().toStdString() ) ;
      }
   }
}
