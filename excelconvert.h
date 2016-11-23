#ifndef EXCELCONVERT_H
#define EXCELCONVERT_H

#include <QDialog>

namespace Ui {
class ExcelConvert;
}

class ExcelConvert : public QDialog
{
   Q_OBJECT

public:
   explicit ExcelConvert(QWidget *parent = 0);
   ~ExcelConvert();

private:
   Ui::ExcelConvert *ui;
};

#endif // EXCELCONVERT_H
