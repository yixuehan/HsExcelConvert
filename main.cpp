#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QMessageBox msgBox ;

   try {

      MainWindow w;
      w.show();
      return a.exec();
   }
   catch ( boost::property_tree::info_parser_error &ec ) {

      boost::format str( "文件[%s]在[%d]行解析出错.[%s]\n") ;
      str % ec.filename() % ec.line() % ec.message() ;
      msgBox.setText(QString::fromLocal8Bit(str.str().c_str())) ;
      msgBox.exec() ;
   }
   catch ( boost::property_tree::xml_parser_error &ec ) {

      boost::format str( "文件[%s]在[%d]行解析出错.[%s]\n") ;
      str % ec.filename() % ec.line() % ec.message() ;
      msgBox.setText(QString::fromLocal8Bit(str.str().c_str())) ;
      msgBox.exec() ;
   }
   return -1 ;
}
