#include "excelconvert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   ExcelConvert w;
   w.show();

   return a.exec();
}
