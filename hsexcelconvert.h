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
   
   // �����Ľӿ��ļ�
   void addNewInterfaceFile(const std::string &fileName ) ;
   // ��Ҫ�޸ĵĽӿ��ļ�
   void addModInterfaceFile(const std::string &fileName ) ;
   // �������������ļ�
   void setDatatypeFile(const std::string &fileName ) ;
   // ���ñ�׼�ֶ��ļ�
   void setStdfieldFile(const std::string &fileName) ;

   // �����ĵ���
   void setDocument(const std::string &strDocument);

   // �޸�������Ϣ
   void modConf( const std::string &oriKey, const std::string &oriValue,
                 const std::string &newKey, const std::string &newValue) ;

   // �����û���
   void setUserName(const std::string &userName);
   // �����޸ĵ���� 
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
   
   std::fstream m_confFile ;  // �����ļ���

   std::string m_strDocument ;  // Ŀ�����ļ���
   std::vector<std::string> m_newInterfaceFileNames ;  // �����Ľӿ��ļ���
   std::vector<std::string> m_modInterfaceFileNames ;  // ��Ҫ�޸ĵĽӿ��ļ���
   
   std::map<std::string, stdField> m_stdFields ;  // ��׼�ֶζ�Ӧ������������
   boost::property_tree::ptree m_ptDatatype ;  // ��������
   boost::property_tree::ptree m_ptStdfield ;  // ��׼�ֶ�
   
   boost::property_tree::ptree m_ptConf ;  // ȫ��������Ϣ
   
   // ת����Դ��
   GenInterface genInterface ;
};

#endif // HSEXCELCONVERT_H
