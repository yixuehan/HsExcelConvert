#ifndef GENINTERFACE_H
#define GENINTERFACE_H
#include <QtXlsx/QtXlsx>

#include <iostream>
#include <vector>
#include <string>

class GenInterface
{
public:
   GenInterface();
   void convert(const std::vector<std::string> &newInterfaces,
                const std::vector<std::string> &modInterfaces) ;


   
   void setPWorksheet(QXlsx::Worksheet *value);

private:
   QXlsx::Worksheet *pWorksheet ;
   
   
private:
   void initSheet() ;
   
};

#endif // GENINTERFACE_H
