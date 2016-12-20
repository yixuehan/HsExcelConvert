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

void GenInterface::setPWorksheet(QXlsx::Worksheet *value)
{
   pWorksheet = value;
}


void GenInterface::initSheet()
{
   // ÉèÖÃ±³¾°É«
   int r = 204,
       g = 255,
       b = 255;
   QXlsx::Format fmt ;


   fmt.setPatternBackgroundColor(QColor(r,g,b));
   for ( i = 0; i < 65536; ++i ) {
      for ( j = 0; j < 65536; ++j ) {
         pWorksheet->write(i, j, QVariant(), fmt) ;
      }
   }
}
