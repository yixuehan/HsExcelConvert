#include "geninterface.h"
#include <boost/exception/exception.hpp>
#include <QtXlsx/QtXlsx>
#include <QMessageBox>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <QFileInfo>
#include <QRegExp>

GenInterface::GenInterface(shared_ptr<boost::property_tree::ptree> pSetting)
   :usedRow(0),
    usedColumn(0)
{
   this->pSetting = pSetting ;
}

void GenInterface::convert()//const vector<string> &newInterfaces, 
                           //const vector<string> &modInterfaces)
{
   initSheet();
   usedRow = 1 ;
   strCurrIAdd = "" ;
   strCurrOAdd = "" ;
   for ( auto iter : mapInterface ) {
      convert(iter.first);
      ++usedRow ;
   }
   ++usedRow ;
   usedColumn = 2 ;
   // �����޸�˵��
   auto value_optional = ptInterfaceConf.get_optional<string>("����˵��.merge");
   shared_ptr<QXlsx::CellRange> pRange ;
   if ( value_optional ) {
      vector<string> v;
      boost::algorithm::split( v, *value_optional, boost::is_any_of(":"), boost::token_compress_on) ;
      pRange =  shared_ptr<QXlsx::CellRange>(new QXlsx::CellRange(usedRow, std::stoi(v[0]), usedRow, std::stoi(v[1]))) ;
   }
   auto pFmt = getDefaultExcelFormat() ;
   auto v = getColor("applyBackgroundColor") ;
   pFmt->setHorizontalAlignment(QXlsx::Format::AlignLeft);
   pFmt->setVerticalAlignment(QXlsx::Format::AlignTop);
   pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
   pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("keyFontName").c_str()));
   pFmt->setFontSize(ptInterfaceConf.get<int>("applyFontSize"));
   v = getColor("modFontColor") ;
   pFmt->setFontColor(QColor(v[0], v[1], v[2]));
   pFmt->setFontBold(true);
   pWorksheet->setRowHeight(usedRow, usedRow, 96) ;
   auto value = specialValue("����˵��", "" ) ;
   setValue(value, pRange, pFmt);
   
}

void GenInterface::readInterfaceConf()
{
   // ��ȡĿ���ļ���ʽ����
   auto value = pSetting->get<string>("interfaceConf");
   boost::property_tree::read_info(value, ptInterfaceConf) ;
   //boost::property_tree::write_info("test.info", ptInterfaceConf) ;
   // ��ȡӳ���ϵ�ļ�����
   value = pSetting->get<string>("ori2InterfaceConf");
   boost::property_tree::read_info(value, ptOri2InterfaceConf);
   // ��ȡ���������ļ�
   value = ptInterfaceConf.get<string>("datatypeConf") ;
   readPtreeFromUtf8(value, ptDatatypeConf);
   // ��ȡ��׼�ֶ��ļ�
   value = ptInterfaceConf.get<string>("stdfieldConf") ;
   readPtreeFromUtf8(value, ptStdfieldConf);
   string strName, strDesc, strType ;
   string strTypeName, strTypeOrcl ;
   // ת�����������ļ���ʽ
   map<string, string> mapDatatype ;
   for ( auto node : ptDatatypeConf.get_child("hsdoc")) {
      if ( "userType" != node.first) continue ;
      strTypeName = node.second.get<string>("<xmlattr>.name") ;
      strTypeOrcl = node.second.get<string>("database.map.<xmlattr>.value") ;
      if( "HsDate" == strTypeName ) {
         strTypeOrcl = "N8" ;
      }
      boost::algorithm::replace_all(strTypeOrcl, "varchar2", "C") ;
      boost::algorithm::replace_all(strTypeOrcl, "varchar", "C") ;
      boost::algorithm::replace_all(strTypeOrcl, "char", "C") ;
      boost::algorithm::replace_all(strTypeOrcl, "number", "N") ;
      boost::algorithm::replace_all(strTypeOrcl, ",", ".") ;
      boost::algorithm::erase_all(strTypeOrcl, "(") ;
      boost::algorithm::erase_all(strTypeOrcl, ")") ;
      boost::algorithm::erase_all(strTypeOrcl, ".0") ;
      mapDatatype[strTypeName] = strTypeOrcl ;
   }
   
   for ( auto node : ptStdfieldConf.get_child("hsdoc")) {
      if ( "stdfield" != node.first ) continue ;
      strName = node.second.get<string>("<xmlattr>.name") ;
      strDesc = node.second.get<string>("<xmlattr>.cname") ;
      strType = node.second.get<string>("<xmlattr>.type") ;
      strType = mapDatatype[strType] ;
      mapStdfields[strName] = {strDesc, strType} ;
   }
   
}

void GenInterface::setPWorksheet(QXlsx::Worksheet *value)
{
   pWorksheet = value;
}

void GenInterface::setSetting(shared_ptr<boost::property_tree::ptree> pPtree)
{
   pSetting = pPtree ;
}

void GenInterface::keyValueConvert(const pair_type &ptNode)
{
   // ���µ�һ�п�ʼ
   usedRow += 1 ;
   usedColumn = 2 ;
   QMessageBox msg ;
   //auto _column = 0 ;
   shared_ptr<QXlsx::CellRange> pRange ;
   for ( auto node : ptNode.second) {
      // ��������
      pRange = nullptr ;
      
      setKey(node.first) ;
      ++usedColumn ;
      // ����ֵ��������Ƿ���Ҫ�ϲ�
      //msg.setText(QString::fromLocal8Bit(node.first.c_str())) ;
      if ( node.second.get_child_optional("merge") ) {
         vector<string> v;
         boost::algorithm::split( v, node.second.get_child("merge").get_value<string>(), boost::is_any_of(":"), boost::token_compress_on) ;
         pRange =  shared_ptr<QXlsx::CellRange>(new QXlsx::CellRange(usedRow, std::stoi(v[0]), usedRow, std::stoi(v[1]))) ;
      }
      auto value = node.second.get_value<string>();
      
      msg.setText(toQStr(node.first + ":" + value));
      //msg.exec();
      if ( "" == value ) {
         auto valueOptional = ptOri2InterfaceConf.get_optional<string>(node.first) ;
         if ( valueOptional ) {
            value = ptOri->get<string>(*valueOptional) ;
         }
      }
      shared_ptr<QXlsx::Format> pFmt = nullptr ;
      if ( "�޸ļ�¼" == value ) {
         pFmt = getDefaultExcelFormat() ;
         auto v = getColor("modFontColor") ;
         pFmt->setFontColor(QColor(v[0], v[1], v[2]));
      }
      value = specialValue(node.first, value) ;
      setValue( value, pRange, pFmt ) ;
      if ( "���ܺ�" == node.first ) {
         if ( bNew) {
            if ( "" == strNewInterface ) {
               strNewInterface = "�����ӿ� " + value + " " ;
            }
            else {
               strNewInterface += value + " " ;
            }
         }
         else {
            if ( "" == strModInterface ) {
               strModInterface = "\r\n                    �޸Ľӿ� " + value + " " ;
            }
            else {
               strModInterface += value + " " ;
            }
         }
      }
      
      //msg.setText(toQStr(node.first + ":" + value)) ;
      
      ++usedColumn ; 
   }
}

void GenInterface::keyConvert(const pair_type &ptNode)
{
   // ���µ�һ�п�ʼ
   usedRow += 1 ;
   usedColumn = 2 ;
   shared_ptr<QXlsx::CellRange> pRange ;
   for ( auto node : ptNode.second ) {
      // ����ֵ��������Ƿ���Ҫ�ϲ�
      pRange = nullptr; 
      auto childOptional = node.second.get_child_optional("merge") ;
      if ( childOptional ) {
         vector<string> v;
         boost::algorithm::split( v, childOptional->get_value<string>(), boost::is_any_of(":"), boost::token_compress_on) ;
         pRange =  shared_ptr<QXlsx::CellRange>(new QXlsx::CellRange(usedRow, std::stoi(v[0]), usedRow, std::stoi(v[1]))) ;
      }
      setKey( node.first, pRange ) ;
      ++usedColumn ; ;
   }
}

void GenInterface::valueConvert(const pair_type &ptNode)
{
   QMessageBox msg ;
   string strParam = "import" ;
   auto childNode = ptNode.second.get_child_optional(strParam) ;
   if ( !childNode ) {
      strParam = "export" ;
      childNode = ptNode.second.get_child_optional(strParam) ;
   }
   
  // msg.setText(toQStr(ptOri->get<string>("hsdoc.basic.basic.<xmlattr>.description"))) ;
  // msg.exec();
   for ( auto oriNode : ptOri->get_child(ptOri2InterfaceConf.get<string>(strParam))) {
      // ���µ�һ�п�ʼ
      usedRow += 1 ;
      usedColumn = 2 ;
      string strArg ;
      string strFlag ;
      string value ;
      auto pFmt = getDefaultExcelFormat();
      auto v = getColor("docBackgroundColor") ;
      bool bAdd = oriNode.second.get<bool>("<xmlattr>.bAdd") ;

      if ( bAdd && !bNew ) {
         auto v2 = getColor("modFontColor") ;
         pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
         pFmt->setHorizontalAlignment(QXlsx::Format::AlignLeft);
         pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
         pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("keyFontName").c_str()));
         pFmt->setBorderStyle(QXlsx::Format::BorderNone);
         pWorksheet->writeString(usedRow, 1, QString::fromLocal8Bit("ADD"), *pFmt) ;
         pFmt->setBorderStyle(QXlsx::Format::BorderThin);
      }
      for ( auto node : *childNode ) {
         value = "" ;
         shared_ptr<QXlsx::Format> pFmt = nullptr ;
         auto valueOptional = oriNode.second.get_optional<string>(ptOri2InterfaceConf.get<string>(node.first)) ;
         if ( valueOptional ) {
            value = *valueOptional ;
         }
         if ( "����" == node.first ) {
            value = mapStdfields[strArg].type ;
         }
         if ( "˵��" == node.first ) {
            value = mapStdfields[strArg].name ;
         }
         //msg.setText( toQStr(node.first + ":" + ptOri2InterfaceConf.get<string>(node.first) + ":" + value) ) ;
         //msg.exec();
         // �����flag
         if ( "�������" == node.first || "�������" == node.first) {
            auto v = getColor("keyBackgroundColor") ;
            pFmt = getDefaultExcelFormat();
            pFmt->setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
            if ( bNew || bAdd ) {
               auto v2 = getColor("modFontColor") ;
               pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
            }
            pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("keyFontName").c_str()));
            strArg = *(oriNode.second.get_optional<string>(ptOri2InterfaceConf.get<string>("������"))) ;
            if ( bAdd && "�������" == node.first ) {
               if ( "" == strCurrIAdd ) {
                  strCurrIAdd = "������� " + strArg + " " ;
               }
               else {
                  strCurrIAdd += strArg + " " ;
               }
            }
            if ( bAdd && "�������" == node.first ) {
               if ( "" == strCurrOAdd ) {
                  strCurrOAdd = "�������� " + strArg + " " ;
               }
               else {
                  strCurrOAdd += strArg + " " ;
               }
            }
            
            strFlag = value ;
         }
         if ( "����" == node.first ) {
            pFmt = getDefaultExcelFormat();
            if ( bNew || bAdd ) {
               auto v2 = getColor("modFontColor") ;
               pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
            }
            if ( "import" == strParam && "D" != strFlag ) {
              value = oriNode.second.get<bool>("<xmlattr>.bNecessary") ? "Y" : "N" ;
              qDebug() << toQStr(value) ;
            }
            pFmt->setHorizontalAlignment(QXlsx::Format::AlignHCenter);
         }
         //cout << "strArg:" << strArg << endl ;
         
         value = specialValue(node.first, value, strArg, strFlag ) ;
         setValue( value, nullptr, pFmt, bAdd ) ;
         ++usedColumn ;
      }
   }
   
}

void GenInterface::setKey(const string &strKey, shared_ptr<QXlsx::CellRange> pRange /* = nullptr*/, 
                          shared_ptr<QXlsx::Format> pFmt /* = nullptr*/,
                          bool bAdd/* = false*/)
{
   if ( nullptr == pFmt ) {
      pFmt = getDefaultExcelFormat();
      auto v = getColor("keyBackgroundColor") ;
      if ( bNew || bAdd ) {
         auto v2 = getColor("modFontColor") ;
         pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
      }
      pFmt->setHorizontalAlignment(QXlsx::Format::AlignHCenter);
      pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
      pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("keyFontName").c_str()));
   }
   if ( nullptr == pRange )
      pWorksheet->writeString(usedRow, usedColumn, QString::fromLocal8Bit(strKey.c_str()), *pFmt) ;
   else {
      pWorksheet->writeString(pRange->firstRow(), pRange->firstColumn(), QString::fromLocal8Bit(strKey.c_str()), *pFmt ) ;
      pWorksheet->mergeCells(*pRange, *pFmt) ;
      usedColumn = pRange->lastColumn();
      cout << strKey << ":" << usedColumn << endl ;
   }
}

void GenInterface::setValue(const string &strValue, shared_ptr<QXlsx::CellRange> pRange /* = nullptr*/, 
                            shared_ptr<QXlsx::Format> pFmt /* = nullptr*/,
                            bool bAdd /*= false*/)
{
   if ( nullptr == pFmt ) {
      pFmt = getDefaultExcelFormat();
   }
   if ( bNew || bAdd ) {
      auto v2 = getColor("modFontColor") ;
      pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
   }
   if ( nullptr == pRange )
      pWorksheet->writeString(usedRow, usedColumn, QString::fromLocal8Bit(strValue.c_str()), *pFmt) ;
   else {
      pWorksheet->writeString(pRange->firstRow(), pRange->firstColumn(), QString::fromLocal8Bit(strValue.c_str()), *pFmt ) ;
      pWorksheet->mergeCells(*pRange, *pFmt) ;
      usedColumn = pRange->lastColumn();
      // cout << strValue << ":" << usedColumn << endl ;
   }
}

string GenInterface::specialValue(const string &strOriKey, const string &strOriValue,
                                  const string &strOriArg/* = ""*/,
                                  const string &strOriFlag/* = "" */) const
{
   QDate d = QDate::currentDate() ;
   QFileInfo file(toQStr(currFileName)) ;
   string strKey = strOriKey ;
   string strValue = strOriValue ;
   string strArg = strOriArg ;
   string strFlag = strOriFlag ;
   static string strOpenAPI ;
   if ( "��������" == strKey || "OpenAPI��������" == strKey ) {
      strValue = toCStr(d.toString(QString::fromLocal8Bit("yyyyMMdd"))) ;
   }
//   else if ( "���ܺ�" == strKey ) {
//      //strNewInterfaceList += (" " + strValue) ;
//   }
   else if ( "��������" == strKey ) {
      strValue = toCStr(file.baseName()) ;
      strValue = strValue.substr( strValue.find_last_of('_') + 1 );
   }
   else if ( "Ӣ����" == strKey ) {
      auto pos = strValue.find("CNST_FUNCID_EXT") ;
      if ( string::npos == pos ) {
         if ( string::npos == (pos = strValue.find("CNST_FUNCID_")) ) {
            return "";//throw std::exception(toCStr(string("�����ù淶��Ӣ����[") + toCStr(file.baseName()) + "]")) ;
         }
         else {
            pos += "CNST_FUNCID_"s.length() ;
         }
      }
      else {
         pos += "CNST_FUNCID_EXT"s.length() ;
      }
      
      strValue = strValue.substr(pos) ;
      strValue = "func_" + boost::algorithm::to_lower_copy(strValue) ;
   }
   else if ( "OpenAPI���ܺ�" == strKey ) {
      auto pos = strValue.find("CNST_FUNCID_EXT") ;
      if ( string::npos == pos ) {
         if ( string::npos == (pos = strValue.find("CNST_FUNCID_")) ) {
            return "";//throw std::exception(toCStr(string("�����ù淶��Ӣ����[") + toCStr(file.baseName()) + "]")) ;
         }
         else {
            pos += "CNST_FUNCID_"s.length() ;
         }
      }
      else {
         pos += "CNST_FUNCID_EXT"s.length() ;
      }
      strValue = strValue.substr(pos) ;
      strOpenAPI = strValue = boost::algorithm::to_lower_copy(strValue) ;
   }
   else if ("ҵ��Χ" == strKey || "OpenAPIҵ������" == strKey ) {
      strValue = toCStr(file.baseName()) ;
      auto pos1 = strValue.find_first_of('_');
      auto pos2 = strValue.find_first_of('_', pos1+1) ;
      strValue = strValue.substr(pos1+1, pos2-pos1-1) ;
   }
   else if ("����ʾ��" == strKey ) {
      boost::format fmt(strValue) ;
      fmt % strOpenAPI ;
      strValue = fmt.str() ;
   }
   else if ( "��ȷ����ʾ��" == strKey ) {
      boost::format fmt(strValue) ;
      bool bExists = false ;
      for( auto &out : ptOri->get_child("hsdoc.parameter.export") ) {
         fmt % out.second.get<string>("<xmlattr>.name") ;
         bExists = true ;
         break ;
      }
      if ( !bExists ) {
         strValue = "" ;
      }
      else {
         strValue = fmt.str() ;
      }
   }
   else if ( "�������" == strKey || "�������" == strKey) {
      if ( "D" == strValue ) {
         strValue = "TD" ;
      }
      if ( "" == strValue ) {
         strValue = "TH" ;
      }
      if ( "password" == strArg || "user_token" == strArg ) {
         strValue = "THD" ;
      }
   }
   else if ( "����" == strKey ) {
      if ( "D" == strFlag ) {
         strValue = "Y" ;
         if ( "password" == strArg || "password_type" == strArg ) {
            strValue = "N" ;
         }
      }
   }
   else if( "����˵��" == strKey ) {
      strValue = "����˵����\r\n    "s
               + toCStr(d.toString(QString::fromLocal8Bit("yyyyMMdd"))) + " " + 
                 pSetting->get<string>("userName") + " ";
      
      strValue += strNewInterface ;
      
      strValue += strModInterface  ;
      
      strValue += "\r\n    �޸ĵ�:" + pSetting->get<string>("modifyNo") ;
   }
   else if ( "�޸ļ�¼" == strValue ) {
      if ( bNew ) {
         strValue = toCStr(d.toString(QString::fromLocal8Bit("yyyyMMdd"))) + " "s +
                    pSetting->get<string>("userName") + " ����" ;
      }
      else { 
         strValue = toCStr(d.toString(QString::fromLocal8Bit("yyyyMMdd"))) + " "s +
                    pSetting->get<string>("userName") + strCurrIAdd  + " "  + strCurrOAdd ;
      }
   }
   return strValue ;
   
}

void GenInterface::setInterface(map<string, InterfacePtree> mapInterface)
{
   this->mapInterface = mapInterface ;
}

void GenInterface::initSheet()
{
   QMessageBox msg ;
   // ���ñ���ɫ
   auto v = getColor("docBackgroundColor") ;
   QXlsx::Format fmt ;

   fmt.setPatternBackgroundColor(QColor(v[0], v[1], v[2]));
   //QXlsx::CellRange range(1,1,65535,65535) ;
   //QXlsx::CellReference reference()
   //pWorksheet->write
   for ( int i = 1; i < 1000; ++i ) {
      for ( int j = 1; j < 40; ++j ) {
         pWorksheet->write(i, j, QVariant(), fmt) ;
      }
   }
   // �����п�
   pWorksheet->setColumnWidth(1, 1, 4) ;
   pWorksheet->setColumnWidth(2, 2, 16) ;
   pWorksheet->setColumnWidth(3, 3, 35) ;
   pWorksheet->setColumnWidth(4, 4, 17) ;
   pWorksheet->setColumnWidth(5, 5, 32) ;
   pWorksheet->setColumnWidth(6, 6, 17.5) ;
   pWorksheet->setColumnWidth(7, 7, 15) ;
}

void GenInterface::convert(const string &name)
{
   QMessageBox msg ;
   msg.setText( toQStr(name.c_str()) + ":" + QString::number(usedRow));
   // ��¼��ǰԴ�ļ�
   currFileName = name ;
   // ��ȡԴ�ļ�����
   // readPtreeFromUtf8(name, ptOri);
   ptOri = &(mapInterface[currFileName].pt) ;
   bNew = mapInterface[currFileName].bNew ;
   string str ;
   if ( bNew ) {
      // ����add
      str = "ADD" ;
   }
   else {
      str = "MOD" ;
   }
   auto pFmt = getDefaultExcelFormat();
   auto v = getColor("docBackgroundColor") ;
   //if ( bNew ) {
      auto v2 = getColor("modFontColor") ;
      pFmt->setFontColor(QColor(v2[0], v2[1], v2[2]));
   //}
   pFmt->setHorizontalAlignment(QXlsx::Format::AlignLeft);
   pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
   pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("keyFontName").c_str()));
   pFmt->setBorderStyle(QXlsx::Format::BorderNone);
   pWorksheet->writeString(usedRow+1, 1, QString::fromLocal8Bit(str.c_str()), *pFmt) ;
   for ( auto &node : ptInterfaceConf.get_child("resourceInterface")){
      
      // keyValue��ʽת��
      if ( "keyValue"s == node.first ) {
         keyValueConvert(node) ;
      }
      // key��ʽת��
      else if ( "key"s == node.first ) {
         keyConvert(node) ;
      }
      // value��ʽת��
      else if ( "value"s == node.first ) {
         
         valueConvert(node) ;
      }
   }
}

vector<int> GenInterface::getColor(const string &colorConf) const
{
   QMessageBox msg ;
   vector<int> vInt ;
   std::vector<string> vStr ;
   string backColor = ptInterfaceConf.get<string>(colorConf) ;
   boost::algorithm::split(vStr, backColor, boost::is_any_of(" "), boost::token_compress_on) ;
   for ( auto value : vStr ) {
      vInt.push_back(std::stoi(value));
   }
   if ( vInt.size() < 3 ) {
      msg.setText( toQStr("[" + colorConf + "]���ò��淶")) ;
      msg.exec() ;
      vInt.resize(3);
   }
   return vInt ;
}

void GenInterface::readPtreeFromUtf8(const string &fileName, boost::property_tree::ptree &pt)
{ 
   pt.clear();
   ifstream ifs(fileName) ;
   ifs.seekg(0, ios::end) ;
   size_t filesize = ifs.tellg();
   char *p = new char[filesize+1] ;
   boost::shared_array<char> pArray(p) ;
   
   memset(pArray.get(), 0, filesize+1) ;
   ifs.seekg(0, ios::beg);
   ifs.read(pArray.get(), filesize) ;
   auto outstr = boost::locale::conv::between(pArray.get(),"gbk", "utf8") ;

   std::stringstream iostr(outstr) ;
   
   boost::property_tree::read_xml(iostr, pt);
   
}

std::shared_ptr<QXlsx::Format> GenInterface::getDefaultExcelFormat()
{
   // Ĭ����Ϊֵ���͵ĸ�ʽ
   auto v = getColor("valueBackgroundColor") ;
   auto pFmt = shared_ptr<QXlsx::Format>(new QXlsx::Format);
   pFmt->setHorizontalAlignment(QXlsx::Format::AlignLeft);
   pFmt->setPatternBackgroundColor(QColor(v[0], v[1], v[2])) ;
   pFmt->setFontName(QString::fromLocal8Bit(ptInterfaceConf.get<string>("valueFontName").c_str()));
   pFmt->setFontSize(ptInterfaceConf.get<int>("fontSize"));
   pFmt->setBorderStyle(QXlsx::Format::BorderThin);
   return pFmt ;
}

