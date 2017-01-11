#ifndef GENINTERFACE_H
#define GENINTERFACE_H
#include <QtXlsx/QtXlsx>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/locale.hpp>
#include <locale>
#include <sstream>
#include <boost/shared_array.hpp>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include "pubfunc.h"
#include <QMainWindow>
using namespace std ;


//Q_UNUSED

//QT_MOC_LITERAL

class GenInterface
{
public:
   GenInterface(shared_ptr<boost::property_tree::ptree> pSetting);
   void convert() ;
   
   void readInterfaceConf() ;
   void setPWorksheet(QXlsx::Worksheet *value);
   void setSetting( shared_ptr<boost::property_tree::ptree> pPtree ) ;
   // 转换功能函数
   typedef std::pair<const std::string,boost::property_tree::basic_ptree<string,std::string,std::less<std::string>>> pair_type;
   void keyValueConvert( const pair_type &ptNode ) ;
   void keyConvert( const pair_type &ptNode ) ;
   void valueConvert( const pair_type &ptNode ) ;
   // 设置属性和值
   void setKey( const string &strKey, shared_ptr<QXlsx::CellRange> pRange = nullptr, 
                shared_ptr<QXlsx::Format> pFmt = nullptr,
                bool bAdd = false);
   void setValue( const string &strValue, shared_ptr<QXlsx::CellRange> pRange = nullptr, 
                  shared_ptr<QXlsx::Format> pFmt = nullptr,
                  bool bAdd = false) ;
   // 特殊字段处理
   string specialValue( const string& strKey, const string &strValue,
                        const string &strOriArg = "",
                        const string &strOriFlag = "") const ;
   void setInterface(map<string, InterfacePtree> mapInterface) ;
private :
   struct stdField
   {
      string name ;
      string type ;
   };
   QXlsx::Worksheet *pWorksheet ;
   shared_ptr<boost::property_tree::ptree> pSetting ;
   boost::property_tree::ptree ptInterfaceConf ;
   boost::property_tree::ptree ptOri2InterfaceConf ;
   boost::property_tree::ptree ptDatatypeConf ;
   boost::property_tree::ptree ptStdfieldConf ;
   boost::property_tree::ptree *ptOri ;
   //string strCurrOri ;
   map<string, InterfacePtree> mapInterface ;
   
   map<string, stdField> mapStdfields ;  // 标准字段对应的描述及类型
   
   int usedRow ;
   int usedColumn ;
   // 记录当前转换源文件
   string currFileName ;
   // 记录当前模式
   // 记录当前新增出入参
   string strCurrIAdd ;
   string strCurrOAdd ;
   // 记录新增及修改的接口
   string strNewInterface ;
   string strModInterface ;
   bool bNew ;
   //mutable string strNewInterfaceList ;
   //string strModInterfaceList ;
private:
   void initSheet() ;
   void convert( const string &name ) ;
   vector<int> getColor(const string &colorConf) const ;
   void readPtreeFromUtf8( const string &fileName, boost::property_tree::ptree &pt ) ;
   std::shared_ptr<QXlsx::Format> getDefaultExcelFormat() ;
};

#endif // GENINTERFACE_H
