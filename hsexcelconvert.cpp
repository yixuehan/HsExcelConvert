#include "hsexcelconvert.h"


HsExcelConvert::HsExcelConvert()
{
   m_confFile.open("./etc/setting.info", std::ios::binary | std::ios::in | std::ios::out ) ;
   boost::property_tree::read_info( m_confFile, m_ptConf ) ;
  
}

void HsExcelConvert::setDatatypeFile(const std::string &fileName)
{
   boost::property_tree::read_xml(fileName, m_ptDatatype) ;
}

void HsExcelConvert::setStdfieldFile(const std::string &fileName)
{
   boost::property_tree::read_xml(fileName, m_ptStdfield) ;
}

void HsExcelConvert::setDocument(const std::string &strDocument)
{
   m_strDocument = strDocument;
}

void HsExcelConvert::modConf(const std::string &oriKey, const std::string &oriValue, const std::string &newKey, const std::string &newValue)
{
   
}


void HsExcelConvert::setUserName(const std::string &userName)
{
   m_userName = userName;
}


void HsExcelConvert::setModifyNo(const std::string &value)
{
   modifyNo = value;
}

void HsExcelConvert::convert()
{
   genInterface.convert(m_newInterfaceFileNames, m_modInterfaceFileNames);
}


void HsExcelConvert::addNewInterfaceFile(const std::string &fileName)
{
   m_newInterfaceFileNames.push_back(fileName);
}

void HsExcelConvert::addModInterfaceFile(const std::string &fileName)
{
   m_modInterfaceFileNames.push_back(fileName);
}


