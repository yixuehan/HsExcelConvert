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
   strFileName += "精英汇系统" ;
   strFileName +=  boost::algorithm::erase_all_copy(pSetting->get<string>("userName"), " ") ;
   strFileName += "申请公共资源(" ;
   
   //转换周边接口
   if ( !pDocument->addSheet(toQStr("周边接口")) )
   {
      msg.setText(toQStr("增加[周边接口]表单失败"));
      msg.exec();
      return ;
   }
   strFileName += "周边接口";
   genInterface.setPWorksheet(pDocument->currentWorksheet());
   genInterface.convert();
   
   strFileName += "$账户周边" ;
   strFileName += ")修改单：" ;
   strFileName += pSetting->get<string>("modifyNo") + ".xlsx" ;
   strFileName = "C:\\generate\\" + strFileName ;
   pDocument->saveAs(toQStr(strFileName));
   delete pDocument ;
   msg.setText(toQStr("转换["+ strFileName + "]完成"));
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






