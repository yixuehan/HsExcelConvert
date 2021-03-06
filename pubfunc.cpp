#include "pubfunc.h"

const char *toCStr(const QString &qStr)
{
   static string str ;
   str = qStr.toLocal8Bit().toStdString();
   return str.c_str() ;
}

const char *toCStr(const char *str)
{
   return str ;
}


QString toQStr(const string &str)
{
   return QString::fromLocal8Bit(str.c_str()) ;
}

const char *toCStr(const string &str)
{
   static string _str ;
   _str = str;
   return _str.c_str();
}


void readPtreeFromUtf8(const string &fileName, boost::property_tree::ptree &pt)
{ 
   pt.clear();
   ifstream ifs(fileName) ;
   if ( !ifs ) {
      return ;
   }
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
