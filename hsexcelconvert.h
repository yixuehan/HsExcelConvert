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

#include "geninterface.h"

class HsExcelConvert
{
public:
   HsExcelConvert();
   
   // 新增的接口文件
   void addNewInterfaceFile(const std::string &fileName ) ;
   // 需要修改的接口文件
   void addModInterfaceFile(const std::string &fileName ) ;
   // 设置数据类型文件
   void setDatatypeFile(const std::string &fileName ) ;
   // 设置标准字段文件
   void setStdfieldFile(const std::string &fileName) ;

   // 设置文档名
   void setDocument(const std::string &strDocument);

   // 修改配置信息
   void modConf( const std::string &oriKey, const std::string &oriValue,
                 const std::string &newKey, const std::string &newValue) ;

   // 设置用户名
   void setUserName(const std::string &userName);
   // 设置修改单编号 
   void setModifyNo(const std::string &value);
   
   void convert() ;

private:
   struct stdField
   {
      std::string name ;
      std::string type ;
   };
   
   std::string m_userName ;
   std::string modifyNo ;
   
   std::fstream m_confFile ;  // 配置文件名

   std::string m_strDocument ;  // 目标表格文件名
   std::vector<std::string> m_newInterfaceFileNames ;  // 新增的接口文件名
   std::vector<std::string> m_modInterfaceFileNames ;  // 需要修改的接口文件名
   
   std::map<std::string, stdField> m_stdFields ;  // 标准字段对应的描述及类型
   boost::property_tree::ptree m_ptDatatype ;  // 数据类型
   boost::property_tree::ptree m_ptStdfield ;  // 标准字段
   
   boost::property_tree::ptree m_ptConf ;  // 全局配置信息
   
   // 转换资源类
   GenInterface genInterface ;
};

#endif // HSEXCELCONVERT_H
