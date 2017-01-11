#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
//#include <boost/any.hpp>
int main(int argc, char *argv[])
{
   //QApplication::addLibraryPath(".");
   QApplication a(argc, argv);
   QMessageBox msgBox ;
   //wchar_t wc ;
   try {
//      QDir dir ;
//      msgBox.setText(dir.currentPath()) ;
//      msgBox.exec();
//      boost::property_tree::ptree ptSource ;
//      boost::property_tree::read_xml("E:/恒生电子/svn_new/trunk/Sources/DevCodes/领投宝/公共资源/datatypes.xml", ptSource) ;
//      boost::property_tree::write_info("xml2info2.info", ptSource);
      MainWindow w;
      w.show();
      return a.exec();
   }
   catch ( boost::property_tree::info_parser_error &ec ) {

      boost::format str( "文件%1%在%2%行解析出错.%3%") ;
      str % ec.filename() % ec.line() % ec.message() ;
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch ( boost::property_tree::xml_parser_error &ec ) {

      boost::format str( "文件%1%在%2%行解析出错.%3%") ;
      str % ec.filename() % ec.line() % ec.message() ;
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch ( boost::property_tree::ptree_bad_path &ec ) {
      boost::format str( "%1%") ;
      str % ec.what() ;
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch ( boost::property_tree::ptree_bad_data &ec ) {
      boost::format str( "%1%") ;
      str % ec.what();
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch ( boost::bad_any_cast ec)
   {
      boost::format str("%1%") ;
      str % ec.what() ;
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch(std::exception &e)
   {
      boost::format str("%1%") ;
      str % e.what();
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }
   catch(...)
   {
      boost::format str("Unknown exception") ;
      msgBox.setText(toQStr(str.str())) ;
      msgBox.exec() ;
   }

   return -1 ;
}
