#include "hsexcelconvert.h"
#include "pubfunc.h"
#include <boost/algorithm/string.hpp>
#include <QDate>



HsExcelConvert::HsExcelConvert(shared_ptr<boost::property_tree::ptree> pSetting)
   :genInterface(pSetting)
{  
   this->pSetting = pSetting;
}

void HsExcelConvert::setDatatypeFile(const string &fileName)
{
   boost::property_tree::read_xml(fileName, m_ptDatatype) ;
}

void HsExcelConvert::setStdfieldFile(const string &fileName)
{
   boost::property_tree::read_xml(fileName, m_ptStdfield) ;
}

void HsExcelConvert::readConf()
{
   genInterface.readInterfaceConf();
}

void HsExcelConvert::setDocument(const string &strDocument)
{
   m_strDocument = strDocument;
}



void HsExcelConvert::setUserName(const string &userName)
{
   m_userName = userName;
}


void HsExcelConvert::setModifyNo(const string &value)
{
   modifyNo = value;
}

void HsExcelConvert::convert()
{
   QMessageBox msg ;
   pDocument = new QXlsx::Document;
   string strFileName ;
   QDate d ;
   strFileName = toCStr(d.toString(Qt::ISODate));
   strFileName += "��Ӣ��ϵͳ" ;
   strFileName +=  boost::algorithm::erase_all_copy(pSetting->get<string>("userName"), " ") ;
   strFileName += "���빫����Դ(" ;
   
   //ת���ܱ߽ӿ�
   if ( !pDocument->addSheet(toQStr("�ܱ߽ӿ�")) )
   {
      msg.setText(toQStr("����[�ܱ߽ӿ�]��ʧ��"));
      msg.exec();
      return ;
   }
   strFileName += "�ܱ߽ӿ�";
   genInterface.setPWorksheet(pDocument->currentWorksheet());
   genInterface.convert();
   
   strFileName += "$�˻��ܱ�" ;
   strFileName += ")�޸ĵ���" ;
   strFileName += pSetting->get<string>("modifyNo") + ".xlsx" ;
   strFileName = "C:\\generate\\" + strFileName ;
   pDocument->saveAs(toQStr(strFileName));
   delete pDocument ;
   msg.setText(toQStr("ת��["+ strFileName + "]���"));
   msg.exec();
}

void HsExcelConvert::setPSetting(const shared_ptr<boost::property_tree::ptree> &value)
{
   pSetting = value;
}

void HsExcelConvert::setInterface(map<string, InterfacePtree> mapInterface)
{
   genInterface.setInterface(mapInterface) ;
}






