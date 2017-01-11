#ifndef PUBFUNC_H
#define PUBFUNC_H
#include <QString>
#include <string>
#include <memory>
#include <QtXlsx/QtXlsx>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_array.hpp>
#include <boost/locale.hpp>
using namespace std ;

const char *toCStr(const QString &str);

const char *toCStr(const char *str);

QString toQStr(const string &str);

const char *toCStr(const string &str);

void readPtreeFromUtf8(const string &fileName, boost::property_tree::ptree &pt);

enum InterfaceType{iNew, iMod} ;

struct InterfacePtree
{
   InterfacePtree()
      :bClear(false)
   {
   }
   string pathName ;
   InterfaceType iType ;  // 接口是新增还是修改
   bool bClear ;  // 接口还有没有未处理完的字段
   bool bResultSet ;
   bool bNew ;
   boost::property_tree::ptree pt ;
};

#endif // PUBFUNC_H
