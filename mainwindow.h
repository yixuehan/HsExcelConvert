#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_setdialog.h"
#include "ui_interfaceconvert.h"
#include "ui_resultset.h"
#include "hsexcelconvert.h"
#include "pubfunc.h"
#include <QButtonGroup>

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

   void on_clear_clicked();
   
   void on_save_clicked();
   
   void on_iNecessaryButton_clicked();
   
   void on_iAddButton_clicked();
   
   void on_oAddButton_clicked();
   
   void on_oResultSetButton_clicked();
   
   void on_radioNew_clicked();
   
   void on_radioMod_clicked();
   
   void on_buttonBox_accepted();
   
   void onRadiobuttonSelected1(int) ;
   
   void on_iNecessary_doubleClicked(const QModelIndex &index);
   
   void on_iAdd_doubleClicked(const QModelIndex &index);
   
   void on_oAdd_doubleClicked(const QModelIndex &index);
   
private:
   template <typename T>
   const char * getSetting( const T &str )
   {
      static string _str ;
      _str = pSetting->get<string>(toCStr(str), "") ;
      return _str.c_str();
   }
   
   template <typename T1, typename T2>
   void putSetting( const T1 &key, const T2 &value )
   {
      pSetting->put(toCStr(key), toCStr(value)) ;
   }
   template <typename T1, typename T2>
   void addSetting( const T1 &key, const T2 &value )
   {
      pSetting->add(toCStr(key), toCStr(value)) ;
   }
   
   void addFile( const QString &pathName, bool bAdd = true ) ;
   
   void eraseNew(const string &name) ;
   
private:
   Ui::MainWindow *ui;
   QDialog setDialog ;
   Ui::SetDialog setUi ;
   QDialog interfaceDialog ;
   Ui::interfaceConvert interfaceUi ;
   QDialog resultSetDialog ;
   Ui::resultSet resultSetUi ;
   
   shared_ptr<boost::property_tree::ptree> pSetting ;
   HsExcelConvert hsConvert ;
   string strSettingFile ;
   
   // 需要转换的接口
   map<string, InterfacePtree> mapInterface ;
   // 对应接口的历史配置
   map<string, InterfacePtree> mapHisInterface ;
   // 记录当前文件的key值
   string strCurrKey ;
   QButtonGroup btnGrpInterface ;
   
};

#endif // MAINWINDOW_H
