#include "excelconvert.h"
#include "ui_excelconvert.h"

ExcelConvert::ExcelConvert(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::ExcelConvert)
{
   ui->setupUi(this);
}

ExcelConvert::~ExcelConvert()
{
   delete ui;
}
