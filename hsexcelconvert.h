#ifndef HSEXCELCONVERT_H
#define HSEXCELCONVERT_H
#include <vector>
#include <map>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <algorithm>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <QMessageBox>
#include <boost/format.hpp>
#include <QtXlsx/QtXlsx>
#include <iostream>
#include <QString>
using namespace std ;

#include "geninterface.h"

class HsExcelConvert
{
public:
   HsExcelConvert(shared_ptr<boost::property_tree::ptree> pSetting);
   
//   // 新增的接口文件
//   void addNewInterfaceFile(const string &fileName ) ;
//   // 需要修改的接口文件
//   void addModInterfaceFile(const string &fileName ) ;
   // 设置数据类型文件
   void setDatatypeFile(const string &fileName ) ;
   // 设置标准字段文件
   void setStdfieldFile(const string &fileName) ;
   // 读取资源转换类的配置文件
   void readConf();

   // 设置文档名
   void setDocument(const string &strDocument);

   // 修改配置信息
   void modConf( const string &oriKey, const string &oriValue,
                 const string &newKey, const string &newValue) ;

   // 设置用户名
   void setUserName(const string &userName);
   // 设置修改单编号 
   void setModifyNo(const string &value);
   
   void convert() ;

   void setPSetting(const shared_ptr<boost::property_tree::ptree> &value);
   
   void setInterface(map<string, InterfacePtree> mapInterface) ;
private:

   
   string m_userName ;
   string modifyNo ;

   string m_strDocument ;  // 目标表格文件名
   
   boost::property_tree::ptree m_ptDatatype ;  // 数据类型
   boost::property_tree::ptree m_ptStdfield ;  // 标准字段
   
   shared_ptr<boost::property_tree::ptree> pSetting ;  // 全局配置信息
   
   // xlsx文档
   QXlsx::Document *pDocument ;
   // 转换资源类
   GenInterface genInterface ;
};

#endif // HSEXCELCONVERT_H
