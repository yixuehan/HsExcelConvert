#include "hsexcelconvert.h"
#include <algorithm>
#include <boost/property_tree/xml_parser.hpp>


HsExcelConvert::HsExcelConvert()
{

}

bool HsExcelConvert::setDatatypeFile(const std::string &fileName)
{
   m_ptDatatype.clear();
   boost::property_tree::read_xml(fileName, m_ptDatatype) ;

   return true ;
}

bool HsExcelConvert::addNewInterfaceFile(const std::string &fileName)
{
   m_newInterfaceFileNames.push_back(fileName);
   return true ;
}

bool HsExcelConvert::addModInterfaceFile(const std::string &fileName)
{
   m_modInterfaceFileNames.push_back(fileName);
   return true ;
}


