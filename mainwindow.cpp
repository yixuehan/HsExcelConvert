#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QColor>
#include <QtXlsx/QtXlsx>
#include <QFileInfo>
#include <QDir>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <QDebug>
#include <QDir>
//#include <boost/filesystem.hpp>
//#include <boost/locale.hpp>


MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   strSettingFile("conf/setting.info"),
   pSetting(new boost::property_tree::ptree),
   hsConvert(pSetting)
{
   // UI设置
   ui->setupUi(this);
   setUi.setupUi(&setDialog);
   interfaceUi.setupUi(&interfaceDialog);
   this->setWindowFlags(this->windowFlags() | Qt::WindowMinMaxButtonsHint) ;
   interfaceDialog.setWindowFlags( interfaceDialog.windowFlags() | Qt::WindowMinMaxButtonsHint);
   resultSetUi.setupUi(&resultSetDialog);
   resultSetDialog.setWindowFlags( interfaceDialog.windowFlags() | Qt::WindowMinMaxButtonsHint);
   interfaceUi.radioHide->hide();
   QObject::connect(ui->set, SIGNAL(triggered(bool)),
                    this, SLOT(on_set(bool))) ;
   
 
   boost::property_tree::read_info(strSettingFile, *pSetting) ;
   // 配置->配置名->配置内容
   setUi.userNameEdit->setText(toQStr(getSetting("userName"))) ;
   setUi.modifyNoEdit->setText(toQStr(getSetting("modifyNo"))) ;
   hsConvert.readConf();
   // 恢复上次打开时的界面
   auto child_optional = pSetting->get_child_optional("newInterface") ;
   if ( child_optional ) {
      for ( auto &node : *child_optional) {
         addFile(toQStr(node.second.get_value<string>()), false) ;
      }
   }
   QObject::connect(interfaceUi.iNecessaryButton, SIGNAL(clicked()),
                    this, SLOT(on_iNecessaryButton_clicked())) ;
   QObject::connect(interfaceUi.iAddButton, SIGNAL(clicked()),
                    this, SLOT(on_iAddButton_clicked())) ;
   QObject::connect(interfaceUi.oAddButton, SIGNAL(clicked()),
                    this, SLOT(on_oAddButton_clicked())) ;
   QObject::connect(interfaceUi.oResultSetButton, SIGNAL(clicked()),
                    this, SLOT(on_oResultSetButton_clicked())) ;
   QObject::connect(interfaceUi.buttonBox, SIGNAL(accepted()),
                    this, SLOT(on_buttonBox_accepted())) ;
   QObject::connect(interfaceUi.radioMod, SIGNAL(clicked()),
                    this, SLOT(on_radioMod_clicked())) ;
   QObject::connect(interfaceUi.radioNew, SIGNAL(clicked()),
                    this, SLOT(on_radioNew_clicked())) ;
   btnGrpInterface.addButton(interfaceUi.radioNew, 0);
   btnGrpInterface.addButton(interfaceUi.radioMod, 1);
   btnGrpInterface.addButton(interfaceUi.radioHide, 2);
   
   QObject::connect(&btnGrpInterface, SIGNAL(buttonClicked(int)),
                    this, SLOT(onRadiobuttonSelected1(int))) ;
   
   QObject::connect(interfaceUi.iNecessary, SIGNAL(doubleClicked(QModelIndex)),
                    this, SLOT(on_iNecessary_doubleClicked(const QModelIndex &))) ;
   QObject::connect(interfaceUi.iAdd, SIGNAL(doubleClicked(QModelIndex)),
                    this, SLOT(on_iAdd_doubleClicked(const QModelIndex &))) ;
   QObject::connect(interfaceUi.oAdd, SIGNAL(doubleClicked(QModelIndex)),
                    this, SLOT(on_oAdd_doubleClicked(const QModelIndex &))) ;
            
}

MainWindow::~MainWindow()
{
   // 保存历史配置
   boost::property_tree::write_info(strSettingFile, *pSetting);
   QDir dir(toQStr("his")) ;
   if (! dir.exists()) {
      dir.mkdir(dir.currentPath() + toQStr("/"s + "his")) ;
   }
      
   for ( auto &pt : mapHisInterface ) {
      boost::property_tree::write_info("his/"s + pt.first + ".his", pt.second.pt ) ;
   }
   delete ui;
}



void MainWindow::on_toClient_clicked()
{

}

void MainWindow::on_set(bool)
{
   auto ret = setDialog.exec();
   if ( 1 == ret) {
      putSetting("userName", setUi.userNameEdit->text());
      putSetting("modifyNo", setUi.modifyNoEdit->text());
   }
   boost::property_tree::write_info(strSettingFile, *pSetting) ;
}

void MainWindow::on_newInterface_clicked()
{
   QFileDialog fileDialog ;
   fileDialog.setDirectory(getSetting("interfacePath"));
   fileDialog.setFileMode(QFileDialog::ExistingFiles);
   if ( QFileDialog::Accepted == fileDialog.exec() ) {
      for ( auto pathName : fileDialog.selectedFiles() ) {
         addFile(pathName);
      }
   }
}



void MainWindow::on_settingList_clicked(const QModelIndex &index)
{
   
}

void MainWindow::on_newInterfaceList_doubleClicked(const QModelIndex &index)
{
   // 清除
   auto fileName = ui->newInterfaceList->currentItem()->text();
   strCurrKey = toCStr(fileName) ;
   if ( mapInterface[strCurrKey].bClear ) {
      if ( mapInterface[strCurrKey].bNew ) {
         interfaceUi.radioNew->setChecked(true);
         interfaceUi.radioNew->clicked();
      }
      else {
         interfaceUi.radioMod->setChecked(true);
         interfaceUi.radioMod->clicked();
      }
   }
   else {
      interfaceUi.radioHide->setChecked(true);
      interfaceUi.radioHide->clicked();
   }
   
   interfaceUi.bDelete->setCheckState( Qt::Unchecked ) ;
   interfaceUi.iNecessary->clear();
   interfaceUi.iAdd->clear();
   interfaceUi.oAdd->clear();
   interfaceUi.iList->clear();
   interfaceUi.oList->clear();

   // 将出参入参加入相应的控件中
   interfaceUi.iList->clear();
   auto iChilds = mapInterface[strCurrKey].pt.get_child_optional("hsdoc.parameter.import") ;
   if ( iChilds ) {
      for ( auto iChild : *iChilds ) {
         auto strItem = toQStr(iChild.second.get<string>("<xmlattr>.name")) ;
         interfaceUi.iList->addItem(strItem);
         // 如果是必输项则增加到必输项UI中，如果是新增入参就增加到新增UI中
         auto Necessary = iChild.second.get_optional<bool>("<xmlattr>.bNecessary") ;
         if ( Necessary ) {
            if ( *Necessary ) {
               if ( interfaceUi.iNecessary->findItems(strItem, Qt::MatchExactly).count() == 0 )
                  interfaceUi.iNecessary->addItem(strItem);           
            }
         }
         Necessary = iChild.second.get_optional<bool>("<xmlattr>.bAdd") ;
         if ( Necessary ) {
            if ( *Necessary ) {
               if ( interfaceUi.iAdd->findItems(strItem, Qt::MatchExactly).count() == 0 )
                  interfaceUi.iAdd->addItem(strItem);
            }
         }
      }
   }
   // 出参
   interfaceUi.oList->clear() ;
   iChilds = mapInterface[strCurrKey].pt.get_child_optional("hsdoc.parameter.export") ;
   if ( iChilds ) {
      for ( auto iChild : *iChilds ) {
         auto strItem = toQStr(iChild.second.get<string>("<xmlattr>.name")) ;
         interfaceUi.oList->addItem(strItem);
         // 如果是新增出参就增加到新增UI中
         auto optNew = iChild.second.get_optional<bool>("<xmlattr>.bAdd") ;
         if ( optNew ) {
            if ( *optNew ) {
               if ( interfaceUi.oAdd->findItems(strItem, Qt::MatchExactly).count() == 0 )
                  interfaceUi.oAdd->addItem(strItem);
            }
         }
      }
   }
   
   if ( 1 == interfaceDialog.exec() ) {
      if ( interfaceUi.bDelete->checkState() == Qt::Checked ) {
         // 从界面中删除
         auto pItem = ui->newInterfaceList->takeItem(index.row());
         ui->newInterfaceList->removeItemWidget(pItem);
         delete pItem ;
         // 从转换列表中删除
         eraseNew(toCStr(fileName));
      }
      else {
         // 将选择的结果保存在历史配置中
         auto &pt = mapHisInterface[strCurrKey].pt ;
         for ( int i = 0; i < interfaceUi.iList->count(); ++i ) {
            string strKey = toCStr(interfaceUi.iList->item(i)->text()) ;
            bool bNecessary = interfaceUi.iNecessary->findItems(toQStr(strKey), Qt::MatchExactly).count() != 0 ;
            bool bAdd = interfaceUi.iAdd->findItems(toQStr(strKey), Qt::MatchExactly).count() != 0 ;
            pt.put("import."s + strKey, bNecessary) ;
            auto &childs = mapInterface[strCurrKey].pt.get_child("hsdoc.parameter.import") ;
            //if ( childs ) {
               for ( auto &child : childs ) {
                  if ( child.second.get<string>("<xmlattr>.name") == strKey ) {
                     child.second.put("<xmlattr>.bNecessary", bNecessary ) ;
                     child.second.put("<xmlattr>.bAdd", bAdd ) ;
                  }
               }
            //}
            // cout << toCStr(interfaceUi.iList->item(i)->text()) << endl ;
         }
         for ( int i = 0; i < interfaceUi.oList->count(); ++i ) {
            string strKey = toCStr(interfaceUi.oList->item(i)->text()) ;
            bool bAdd = interfaceUi.oAdd->findItems(toQStr(strKey), Qt::MatchExactly).count() != 0 ;
            pt.put("export."s + strKey, "") ;
            auto &childs = mapInterface[strCurrKey].pt.get_child("hsdoc.parameter.export") ;
            //if ( childs ) {
               for ( auto &child : childs ) {
                  if ( child.second.get<string>("<xmlattr>.name") == strKey ) {
                     child.second.put("<xmlattr>.bAdd", bAdd ) ;
                  }
               }
            //}
            // cout << toCStr(interfaceUi.iList->item(i)->text()) << endl ;
         }
         //boost::property_tree::write_info("test.his", mapInterface[strCurrKey].pt ) ; 
      }
   }
}

void MainWindow::on_clear_clicked()
{
   ui->newInterfaceList->clear();
   mapInterface.clear();
   mapHisInterface.clear();
   //ui->modInterfaceList->clear();
   auto child_optional = pSetting->get_child_optional("newInterface") ;
   if ( child_optional ) {
      child_optional->clear();
   }
}

void MainWindow::addFile( const QString &pathName, bool bAdd/* = true*/ )
{
   QFileInfo file ; 
   file.setFile(pathName) ;
   auto baseName = file.baseName();
   if ( ui->newInterfaceList->findItems(baseName, Qt::MatchExactly).count()
        == 0 ) {
      ui->newInterfaceList->addItem(baseName);
      string strKey = toCStr(baseName) ;
      if ( bAdd ) {
         addSetting("newInterface.file", toCStr(pathName)); 
      }
      auto iter = mapInterface.find(toCStr(baseName)) ;
      if ( mapInterface.end() == iter ) {
         
         mapInterface[strKey].pathName = toCStr(pathName) ;
         // 加载源文件
         readPtreeFromUtf8(toCStr(pathName), mapInterface[strKey].pt) ;
         // 对IO参数特殊处理
         for ( auto &node : mapInterface[strKey].pt.get_child("hsdoc.parameter.export")) {
            if ( "IO" == node.second.get<string>("<xmlattr>.flag") ) {
               // 加入入参中
               boost::property_tree::ptree pt ;
               pt.put("<xmlattr>.name", node.second.get<string>("<xmlattr>.name")) ;
               pt.put("<xmlattr>.flag", "") ;
               pt.put("<xmlattr>.comment", "") ;
               mapInterface[strKey].pt.get_child("hsdoc.parameter.import").add_child("stdFieldQuote", pt) ;  
               // 去除IO标志
               node.second.put("<xmlattr>.flag", "") ;
            }
         }
         mapInterface[strKey].bResultSet = mapInterface[strKey].pt.get<bool>("hsdoc.basic.basic.<xmlattr>.returnResultSet") ;
         // 读取历史信息his/XXX.his
         
         boost::property_tree::ptree hisPt ;
         QFileInfo filePt ;
         filePt.setFile(toQStr("his/"s + toCStr(baseName) + ".his"));
         if ( filePt.exists() ) {
            boost::property_tree::read_info("his/"s + toCStr(baseName) + ".his", hisPt);
         }
         // 增加入参 : 从历史文件中读取相应的字段信息（必输项，原字段）,默认不在原字段的字段为新增入参
    
            auto childs = &(mapInterface[strKey].pt.get_child("hsdoc.parameter.import")) ;
            for ( auto &child : *childs ) {
               auto strName = child.second.get<string>("<xmlattr>.name") ;
               auto optHisNecessary = hisPt.get_optional<bool>("import." + strName) ;
               if ( child.second.get<string>("<xmlattr>.flag") != "D" ) {
                  child.second.put("<xmlattr>.bNecessary", optHisNecessary ? *optHisNecessary : false) ;
                  child.second.put("<xmlattr>.bAdd", !optHisNecessary) ;
               }
               if ( !optHisNecessary) {
                  hisPt.put<bool>("import." + strName, false ) ;
               }
            }
         
         // 记录出参

            childs = &(mapInterface[strKey].pt.get_child("hsdoc.parameter.export")) ;
            for ( auto &child : *childs ) {
               auto strName = child.second.get<string>("<xmlattr>.name") ;
               qDebug() << toQStr(strName) ; 
               auto optHisName = hisPt.get_optional<string>("export." + strName) ;
               child.second.put<bool>("<xmlattr>.bAdd", !optHisName) ; 
               if ( !optHisName ) {
                  hisPt.put("export." + strName, "" ) ;
               }
            }
         //boost::property_tree::write_info("test.his", hisPt) ;
         mapHisInterface[strKey].pt = std::move(hisPt) ;
      }
      
   }
}

void MainWindow::on_save_clicked()
{
   putSetting("userName", toCStr(setUi.userNameEdit->text())) ;
   putSetting("modifyNo", toCStr(setUi.modifyNoEdit->text())) ;
}

void MainWindow::eraseNew(const string &name)
{
   auto child_optional = pSetting->get_child_optional("newInterface") ;
   QFileInfo file ;
   mapInterface.erase(name) ;
   mapHisInterface.erase(name);
   if (child_optional ) {
      auto &child = pSetting->get_child("newInterface") ;
      for ( auto iter = child.begin();
                 iter != child.end();
                 ++iter) {
         cout << iter->second.get_value<string>() << endl ;
      }
      cout << "------------------------" << endl ;
      for ( auto iter = child.begin();
            iter != child.end();
            ++iter) {
         file.setFile(toQStr(iter->second.get_value<string>()));
         
         if ( file.baseName() == toQStr(name) ) {
            child.erase(iter) ;
            break ;
         }
      }
   }
}

void MainWindow::on_toResource_clicked()
{ 
   
   QMessageBox msgBox ;
   for ( auto iter = mapInterface.begin();
         iter != mapInterface.end();
         ++iter ) {
      if ( !(iter->second.bClear) ) {
         msgBox.setText(toQStr("请选择接口【"s + iter->first + "】为新增还是修改")) ;
         msgBox.exec();
         return ;
      }
   }
   try {

      hsConvert.setInterface(mapInterface) ;
      hsConvert.convert() ;
   }
   catch ( boost::property_tree::info_parser_error &ec ) {

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
}

void MainWindow::on_iNecessaryButton_clicked()
{

   if ( interfaceUi.iList->currentRow() < 0 ) return ;
   auto strFind = interfaceUi.iList->currentItem()->text() ;
   auto &iNecessary = interfaceUi.iNecessary ;
   if ( iNecessary->findItems(strFind, Qt::MatchExactly).count() == 0 ) {
      iNecessary->addItem(strFind);
   }
}

void MainWindow::on_iAddButton_clicked()
{

   if ( interfaceUi.iList->currentRow() < 0 ) return ;
   auto strFind = interfaceUi.iList->currentItem()->text() ;
   auto &iAdd = interfaceUi.iAdd ;
   if ( iAdd->findItems(strFind, Qt::MatchExactly).count() == 0 ) {
      iAdd->addItem(strFind);
   }
}

void MainWindow::on_oAddButton_clicked()
{
   
   if ( interfaceUi.oList->currentRow() < 0 ) return ;
   auto strFind = interfaceUi.oList->currentItem()->text() ;
   auto &oAdd = interfaceUi.oAdd ;
   if ( oAdd->findItems(strFind, Qt::MatchExactly).count() == 0 ) {
      oAdd->addItem(strFind);
   }
}

void MainWindow::on_oResultSetButton_clicked()
{
   resultSetUi.resultSetText->clear();
   if ( 1 == resultSetDialog.exec() ) {
      try {
         interfaceUi.oList->clear();
         string strText = toCStr(resultSetUi.resultSetText->toPlainText()) ;
         //strRet = System.Text.RegularExpressions.Regex.Replace(strRet, @",[ ]?[^,]*?\(.*?as", ",");
         // 去除换行
         boost::algorithm::replace_all(strText, "\n", "") ;
         
         // 去除达式取别名
         strText = boost::regex_replace(strText, boost::regex(",[ ]?[^,]*?\\(.*?as"), ",") ;
         cout << "strText:" << strText << endl ;
         // 处理每组的字段
         vector<string> vStdfield ;
         boost::algorithm::split(vStdfield, strText, boost::is_any_of(","), boost::token_compress_on ) ;
         for ( auto &str : vStdfield ) {
            vector<string> arr ;
            boost::algorithm::split(arr, str, boost::is_any_of(" "), boost::token_compress_on ) ;
            // 取每组最后一个字段
            str = arr[arr.size()-1] ;
            // 去掉x.的形式
            boost::algorithm::split(arr, str, boost::is_any_of("."), boost::token_compress_on ) ;
            str = arr[arr.size()-1] ;
         }
         mapInterface[strCurrKey].pt.get_child("hsdoc.parameter.export").clear() ;
         for ( auto str : vStdfield ) {
            if ( interfaceUi.oList->findItems(toQStr(str),Qt::MatchExactly).count() == 0 )
               interfaceUi.oList->addItem(toQStr(str));
            // 加入出参
            boost::property_tree::ptree pt ;
            pt.put("<xmlattr>.name", str) ;
            pt.put("<xmlattr>.flag", "") ;
            pt.put("<xmlattr>.comment", "") ;
            mapInterface[strCurrKey].pt.get_child("hsdoc.parameter.export").add_child("stdFieldQuote", pt) ;    
         }
      }
      catch (... ) {
         QMessageBox msg ;
         msg.setText(toQStr("转换失败"));
         msg.exec() ;
      }
   }   
}

void MainWindow::on_radioNew_clicked()
{
    interfaceUi.iAddButton->setEnabled(false);
    interfaceUi.oAddButton->setEnabled(false);
    interfaceUi.iAdd->setEnabled(false);
    interfaceUi.oAdd->setEnabled(false);
}

void MainWindow::on_radioMod_clicked()
{
   interfaceUi.iAddButton->setEnabled(true);
   interfaceUi.oAddButton->setEnabled(true);
   interfaceUi.iAdd->setEnabled(true);
   interfaceUi.oAdd->setEnabled(true);
}

void MainWindow::on_buttonBox_accepted()
{
   QMessageBox msg ;
//   msg.setText(QString::number(btnGrpInterface.checkedId())) ;
//   msg.exec();
   if (interfaceUi.bDelete->checkState() != Qt::Checked) {
      if ( btnGrpInterface.checkedId() == 2 ) {
         msg.setText(toQStr("请选择是新增接口还是修改的接口"));
         msg.exec();
         return ;
      }
      if (mapInterface[strCurrKey].bResultSet && interfaceUi.oList->count() == 0 ) {
         msg.setText(toQStr("请设置结果集字段"));
         msg.exec();
         return ;
      }
      mapInterface[strCurrKey].bNew = (btnGrpInterface.checkedId() == 0) ;
      mapInterface[strCurrKey].bClear = true ;
   }
   

   interfaceDialog.accept();
}

void MainWindow::onRadiobuttonSelected1(int)
{
   btnGrpInterface.checkedId();
}

void MainWindow::on_iNecessary_doubleClicked(const QModelIndex &index)
{
   auto pItem = interfaceUi.iNecessary->takeItem(index.row());
   interfaceUi.iNecessary->removeItemWidget(pItem);
   delete pItem ;
}

void MainWindow::on_iAdd_doubleClicked(const QModelIndex &index)
{
   auto pItem = interfaceUi.iAdd->takeItem(index.row());
   interfaceUi.iAdd->removeItemWidget(pItem);
   delete pItem ;
}

void MainWindow::on_oAdd_doubleClicked(const QModelIndex &index)
{
   auto pItem = interfaceUi.oAdd->takeItem(index.row());
   interfaceUi.oAdd->removeItemWidget(pItem);
   delete pItem ;
}
