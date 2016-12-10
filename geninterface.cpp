#include "geninterface.h"



GenInterface::GenInterface()
{
   
}

void GenInterface::convert(const std::vector<std::string> &newInterfaces, 
                           const std::vector<std::string> &modInterfaces)
{
   for ( auto &name : newInterfaces ) {
      std::cout << name << std::endl ;
   }
   for ( auto &name : modInterfaces ) {
      std::cout << name << std::endl ;
   }
}

void GenInterface::setPWorkbook(QXlsx::Workbook *value)
{
   pWorkbook = value;
}

void GenInterface::initSheet()
{
   // ÉèÖÃ±³¾°É«
   
}
