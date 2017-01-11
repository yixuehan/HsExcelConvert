#-------------------------------------------------
#
# Project created by QtCreator 2016-12-07T10:19:28
#
#-------------------------------------------------

QT       += core gui xlsx
CONFIG += c++1y
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HsExcelConvert
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hsexcelconvert.cpp \
    geninterface.cpp \
    pubfunc.cpp

HEADERS  += mainwindow.h \
    hsexcelconvert.h \
    geninterface.h \
    pubfunc.h

FORMS    += mainwindow.ui \
    setdialog.ui \
    interfaceconvert.ui \
    resultset.ui


INCLUDEPATH += "D:/boost_1_62_0/include/boost-1_62"

CONFIG(debug, debug|release) {
#LIBS += "D:/boost_1_62_0/lib/debug/boost_filesystem-vc140-mt-gd-1_62.dll"
LIBS += "D:/boost_1_62_0/lib/debug/libboost_locale-vc140-mt-gd-1_62.lib"
LIBS += "D:/boost_1_62_0/lib/debug/libboost_regex-vc140-mt-gd-1_62.lib"
} else {
LIBS += "D:/boost_1_62_0/lib/release/libboost_locale-vc140-mt-1_62.lib"
LIBS += "D:/boost_1_62_0/lib/release/libboost_regex-vc140-mt-1_62.lib"
}

#win32:contains(QMAKE_CC, cl){
#   QMAKE_CXXFLAGS += -Zc:wchar_t
#   QMAKE_CXXFLAGS -= -Zc:wchar_t-
#}


