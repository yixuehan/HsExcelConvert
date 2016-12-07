#ifndef HSEXCELCONVERT_H
#define HSEXCELCONVERT_H
#include <vector>
#include <map>
#include <string>
#include <boost/property_tree/ptree.hpp>


class HsExcelConvert
{
public:
   HsExcelConvert();

   bool addNewInterfaceFile(const std::string &fileName ) ;
   bool addModInterfaceFile(const std::string &fileName ) ;
   bool setDatatypeFile(const std::string &fileName ) ;

private:
   struct stdField
   {
      std::string name ;
      std::string type ;
   };

   std::vector<std::string> m_newInterfaceFileNames ;
   std::vector<std::string> m_modInterfaceFileNames ;
   std::map<std::string, std::string> m_datatypes ;
   std::map<std::string, stdField> m_stdfields ;
   boost::property_tree::ptree m_ptDatatype ;
   boost::property_tree::ptree m_ptStdfield ;
};

#endif // HSEXCELCONVERT_H
