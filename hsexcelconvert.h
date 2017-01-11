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
   
//   // �����Ľӿ��ļ�
//   void addNewInterfaceFile(const string &fileName ) ;
//   // ��Ҫ�޸ĵĽӿ��ļ�
//   void addModInterfaceFile(const string &fileName ) ;
   // �������������ļ�
   void setDatatypeFile(const string &fileName ) ;
   // ���ñ�׼�ֶ��ļ�
   void setStdfieldFile(const string &fileName) ;
   // ��ȡ��Դת����������ļ�
   void readConf();

   // �����ĵ���
   void setDocument(const string &strDocument);

   // �޸�������Ϣ
   void modConf( const string &oriKey, const string &oriValue,
                 const string &newKey, const string &newValue) ;

   // �����û���
   void setUserName(const string &userName);
   // �����޸ĵ���� 
   void setModifyNo(const string &value);
   
   void convert() ;

   void setPSetting(const shared_ptr<boost::property_tree::ptree> &value);
   
   void setInterface(map<string, InterfacePtree> mapInterface) ;
private:

   
   string m_userName ;
   string modifyNo ;

   string m_strDocument ;  // Ŀ�����ļ���
   
   boost::property_tree::ptree m_ptDatatype ;  // ��������
   boost::property_tree::ptree m_ptStdfield ;  // ��׼�ֶ�
   
   shared_ptr<boost::property_tree::ptree> pSetting ;  // ȫ��������Ϣ
   
   // xlsx�ĵ�
   QXlsx::Document *pDocument ;
   // ת����Դ��
   GenInterface genInterface ;
};

#endif // HSEXCELCONVERT_H
